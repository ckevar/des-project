#ifndef CDEV_H
#define CDEV_H

#include <stdint.h>
#include "ee.h"
#include "kernel/sem/inc/ee_sem.h"	/* ErikaOS for semaphores */						
#include "mdev_t.h"
#include "ioctrl.h"


#define _IO_MDEV_TYPE_OP 	'o'
#define _IO_MDEV_TYPE_CONF 	'c'
// Public Definitions
#define open(x) mdev_open(__FILE__, x)
#define close(x) mdev_close(__FILE__, x)
// #define read(x, y, z) mdev_read(__FILE__, x, y, z)
// #define write(x, y, z) mdev_write(__FILE__, x, y, z)
// #define ioctrl(x, y, z) mdev_ioctrl(__FILE__, x, y, z)
#define read(x, y, z) mdev_ops(__FILE__, _IOC(_IOC_READ, _IO_MDEV_OP_TYPE, 0, z), x, y)
#define write(x, y, z) mdev_ops(__FILE__, _IOC(_IOC_WRITE, _IO_MDEV_OP_TYPE, 0, z), x, y)
#define ioctrl(x, y, z) mdev_ops(__FILE__, _IOC(_IOC_WRITE, _IO_MDEV_OP_TYPE, 1, z), x, y)

struct mod_operations {
	int16_t (*open)(uint8_t mi);
	int16_t (*close)(uint8_t mi);
	int16_t (*read)(uint8_t mi, void *buf, uint16_t size);
	int16_t (*write)(uint8_t mi, void *buf, uint16_t size);
	int16_t (*unlocked_ioctrl)(uint8_t mi, uint32_t cmd, uint32_t arg);
};


struct mdev {
	uint32_t owner;
	SemType sem;
};

int16_t mdev_register_region(int16_t potential_ma, uint8_t num_minors);

/**
 * ## mdev_core_init()
 * Initializes:
 * 1. Dev Drive Handler Data --DDH_DATA-- as fully available.
 * 
 * */
void mdev_core_init();

/**
 * ## Module creation functions:
 * A module is a set of data structure that manages similar devices. Keep in
 * mind that devices vary among application and harware. For example, the LED
 * driver uses 3 out of the four leds on the STM32 discovery board. (it should
 * only use 1, the green one) Because the remaining one are linked to the LCD
 * of the _discover more_ board.
 * */
int8_t mdev_init(uint8_t mi, struct mdev *dev);

/* Device Handlers */
int32_t mdev_open(const char *file, int32_t dev);
int32_t mdev_close(const char *file, int32_t dev);
void mdev_seek(uint32_t *cursor, uint32_t offset, uint32_t max, uint8_t whence);
int32_t mdev_ops(const char *file, uint32_t rop, int32_t dev, void *buf);
#endif
