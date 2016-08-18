/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2013          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED_SD
#define _DISKIO_DEFINED_SD

#define _USE_WRITE	1	/* 1: Enable disk_write function */
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

#include "diskio.h"
#include "integer.h"
#include <stm32f10x.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "Porting/inc/mtSPI.h"
#include "App/inc/SystemConfig.h"
#include "Utility/inc/mtDelay.h"

#define FATFS_USE_SDIO                 		0
#define FATFS_SPI                			SPI2
#define FATFS_CS_RCC       				 	RCC_APB2Periph_GPIOB
#define FATFS_CS_PORT        				GPIOB
#define FATFS_CS_PIN        				GPIO_Pin_12

#ifndef FATFS_USE_DETECT_PIN
#define FATFS_USE_DETECT_PIN				0
#endif

#ifndef FATFS_USE_WRITEPROTECT_PIN
#define FATFS_USE_WRITEPROTECT_PIN			0
#endif

#if FATFS_USE_DETECT_PIN > 0
#ifndef FATFS_USE_DETECT_PIN_PIN
#define FATFS_USE_DETECT_PIN_RCC			RCC_AHB1Periph_GPIOB
#define FATFS_USE_DETECT_PIN_PORT			GPIOB
#define FATFS_USE_DETECT_PIN_PIN			GPIO_Pin_6
#endif
#endif

#if FATFS_USE_WRITEPROTECT_PIN > 0
#ifndef FATFS_USE_WRITEPROTECT_PIN_PIN
#define FATFS_USE_WRITEPROTECT_PIN_RCC		RCC_AHB1Periph_GPIOB
#define FATFS_USE_WRITEPROTECT_PIN_PORT		GPIOB
#define FATFS_USE_WRITEPROTECT_PIN_PIN		GPIO_Pin_7
#endif
#endif

#define FATFS_CS_LOW						GPIO_ResetBits(FATFS_CS_PORT, FATFS_CS_PIN); 	//FATFS_CS_PORT->BSRRH = FATFS_CS_PIN
#define FATFS_CS_HIGH						GPIO_SetBits(FATFS_CS_PORT, FATFS_CS_PIN);  	//FATFS_CS_PORT->BSRRL = FATFS_CS_PIN

/*---------------------------------------*/
/* Prototypes for disk control functions */
DSTATUS FATFS_SD_disk_initialize(void);
DSTATUS FATFS_SD_disk_status(void);
DRESULT FATFS_SD_disk_read(BYTE* buff, DWORD sector, UINT count);
DRESULT FATFS_SD_disk_write(const BYTE* buff, DWORD sector, UINT count);
DRESULT FATFS_SD_disk_ioctl(BYTE cmd, void* buff);

#endif

