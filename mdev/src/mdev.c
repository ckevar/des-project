#include "mdev.h"

// 
#define MAJOR_NONE 		-1
#define DEV_NO_OWNER 	0

#ifndef NUM_MODS
# define NUM_MODS		10
#endif

/* _IO Definitons */
#define MDEV_READ_OP _IOC(_IOC_READ, _IO_MDEV_TYPE_OP, 0, 0)
#define MDEV_WRITE_OP _IOC(_IOC_WRITE, _IO_MDEV_TYPE_OP, 0, 0)
#define MDEV_IOCTRL_OP _IOC(_IOC_WRITE, _IO_MDEV_TYPE_OP, 1, 0)

/* Useful macros */
#define MOD_EXISTANCE(ma, mi) if (ma > NUM_MODS) return -1;\
							  if (mi > DDH_INFO[ma].num_minors) return -1

#define MOD_OWNERSHIP(fn, ma, mi) if ((DDH_INFO[ma].minor[mi].owner == DEV_NO_OWNER) ||\
									 (DDH_INFO[ma].minor[mi].owner != (uint32_t)fn)) return -1

struct mod_info {
	int16_t major;
	uint8_t num_minors;
	struct mdev *minor;
	/* This field const struct is under test, it can be place here or it
	 * can be placed inside struct mdev. */
	const struct mod_operations *ops;
};

struct mod_info DDH_INFO[NUM_MODS];

/**
 * ## mdev_register_region
 * 
 * Allocates a module inside the Dev Driver Handler Data --DDH_INFO-- array as long
 * as there's room. On success, it returns the *MAYOR* assigned to the driver, 
 * otherwise it returns -1.
 * 
 * As seen, upon calling this function accepts a *potential_major*, this may reduce 
 * the time spent for allocation but does not guarantee that that's gonna be the
 * *MAYOR*. In order to know the assigned *MAYOR*, the driver code can implement an
 * interface that returns its *MAYOR*, i.e.:
 * 
 * ``` c
 * #define MOD_UART uart_get_major()
 * static int16_t uart_major;
 * 
 * void uart_init() {
 * 		uart_major = ddh_register(0)
 * 		// Handle Error
 * }
 * // ...
 * int16_t uart_get_major() {
 * 	return uart_major
 * }
 * ```
 */


/**
 * ## mdev_core_init()
 * 
 * Initializes:
 * 1. Dev Drive Handler Data --DDH_INFO-- as fully available.
 * 2. <Add here everything that be initialized>
 * */
void mdev_core_init() {
	for (uint16_t ma = 0; ma < NUM_MODS; ma++) {
		DDH_INFO[ma].major = MAJOR_NONE;
		DDH_INFO[ma].num_minors = 0;
	}
}


// SECTION: mdev_init
/**
 * ## mdev_init()
 * Loads the operations
 * */

void mdev_init_semaphores(uint8_t num_minors, struct mdev *minor) {
	uint8_t i;
	for (i = 0; i < num_minors; i++) {
		InitSem(minor[i].sem, 1);
		minor[i].owner = DEV_NO_OWNER;
	}
}

int8_t mdev_init(uint8_t num_mi, struct mdev *dev) {
	mdev_init_semaphores(num_mi, dev);
	return 0;
}
// END SECTION: mdev_init 




// SECTION: mdev_open() 
/**
 * ## open_ownership
 * 
 * If you're sure that there wont be concurrency on the devs then it's ok to comment
 * the sempahore parts supported by ErikaOS. Keep in mind that if the dev is later
 * */
int32_t acquire_ownership(uint32_t owner, int16_t ma, uint8_t mi) {
	WaitSem(&DDH_INFO[ma].minor[mi].sem); 			// Erika Supports

	if (DDH_INFO[ma].minor[mi].owner) {
		PostSem(&DDH_INFO[ma].minor[mi].sem);  		// Erika Supports
		return -1;
	}

	DDH_INFO[ma].minor[mi].owner = owner;
	PostSem(&DDH_INFO[ma].minor[mi].sem); 			// Erika Supports
	return 0;
}



/**
 * ## dev_open(const char *caller_unit, int32_t dev)
 * 
 * All devs are EXCLUSIVE (O_EXCL), they can only be linked to a single caller 
 * function and live in the scope of the c module caller.
 * */
int32_t mdev_open(const char *caller_unit, int32_t dev) {
	int16_t ma = MAJOR(dev);
	uint8_t mi = MINOR(dev);

	MOD_EXISTANCE(ma, mi);

	if (acquire_ownership((uint32_t) caller_unit, ma, mi) < 0) return -1; 		// Erika Supports

	return dev;
}

/* END SECTION mdev_open() */


/* mdev_close */
int32_t release_ownership(uint32_t owner, int16_t ma, uint8_t mi) {
	WaitSem(&DDH_INFO[ma].minor[mi].sem); 			// Erika Supports
	DDH_INFO[ma].minor[mi].owner = DEV_NO_OWNER;
	PostSem(&DDH_INFO[ma].minor[mi].sem); 			// Erika Supports
}

int32_t mdev_close(const char *caller_unit, int32_t dev) {
	int16_t ma = MAJOR(dev);
	uint8_t mi = MINOR(dev);

	MOD_EXISTANCE(ma, mi);
	MOD_OWNERSHIP(caller_unit, ma, mi);

	(*DDH_INFO[ma].ops->close)(mi);

	release_ownership((uint32_t) caller_unit, ma, mi);

	return 0;
}

/* END mdev_close */

/* SECTION: mdev_seek */
void mdev_seek(uint32_t *cursor, uint32_t offset, uint32_t max, uint8_t whence) {
	switch(whence) {
	case SEEK_SET: *cursor = offset; return;
	case SEEK_CUR: *cursor += offset; return;
	case SEEK_END: *cursor = max - offset; return;
	}
}

/* SECTION: mdev_ops */
int32_t execute_op(int16_t ma, uint8_t mi, void *buf, uint32_t ioc) {
	uint16_t size = _IOC_SIZE(ioc);
	uint32_t op = ioc & ~(_IOC_SIZEMASK << _IOC_SIZESHIFT);

	uint32_t tem_read = MDEV_READ_OP;
	uint32_t tem_write = MDEV_WRITE_OP;
	uint32_t tem_ioctrl = MDEV_IOCTRL_OP;

	switch(op) {

	case MDEV_READ_OP: 
		return (*DDH_INFO[ma].ops->read) == NULL ?\
		 -1 : \
		  (*DDH_INFO[ma].ops->read)(mi, buf, size);

	case MDEV_WRITE_OP: 
		return (*DDH_INFO[ma].ops->write) == NULL ?\
		-1 : \
		(*DDH_INFO[ma].ops->write)(mi, buf, size);

	case MDEV_IOCTRL_OP: 
		return  (*DDH_INFO[ma].ops->unlocked_ioctrl) == NULL ?\
		-1 : \
		(*DDH_INFO[ma].ops->unlocked_ioctrl)(mi, (uint32_t) buf, size);
	}

	return -1;
}

int32_t mdev_ops(const char *caller_unit, uint32_t ioc, int32_t dev, void *buf) {
	int16_t ma = MAJOR(dev);
	uint8_t mi = MINOR(dev);
	MOD_EXISTANCE(ma, mi);
	return execute_op(ma, mi, buf, ioc);
}

/* END SECTION: mdev_ops */
