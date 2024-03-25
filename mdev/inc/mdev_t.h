#ifndef MDEV_T_H
#define MDEV_T_H

/* BEGIN: linux/kdev_t.h */

#define MINORBITS		20 // only for 32-bits machine and above
#define MINORMASK		((1U << MINORBITS) - 1)

#define MAJOR(dev)		((uint32_t) ((dev) >> MINORBITS))
#define MINOR(dev)		((uint32_t) ((dev) & MINORMASK))
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))

/* END: linux/kdev_t.h */

/* Seek Flags */
#define SEEK_SET		0
#define SEEK_CUR 		1
#define SEEK_END 		2

/* Open Mode Flags */

/*
#define O_RDONLY 		0b0001
#define O_WRONLY 		0b0010
#define O_NONBLOCK 		0b0100
#define O_EXCL 			0b1000
#define O_RDWR 			(O_RDONLY | O_WRONLY)
*/

#endif
