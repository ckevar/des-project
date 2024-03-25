#ifndef SDIO_STM32f4x_CONSTS_H
#define SDIO_STM32f4x_CONSTS_H

/* Mask for Reserved flags on SDIO Regs */
#define SDIO_CLKCR_MSK 	0x00007EFF
#define SDIO_CMD_MSK	0x00007FFF
#define SDIO_CLR_STA	0x00C007FF
#define SDIO_R1_MSK 	0xFF000000

/* SDIO Support */
#define SDIO_OCR_v32_33	0x00100000
#define SDIO_OCR_v33_34	0x00200000

/* CLCK Conf */
#define SDIO_CLK_400kHz 0x76
#define SDIO_CLK_28MHz 	0

#endif
