/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.h
**
** Module name       : template
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef _DRIVER_BOOTLOADER_H_
#define _DRIVER_BOOTLOADER_H_

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include "Utility/inc/mtPrintfFormat.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define FLASH_Status		uint8_t
#define FLASH_COMPLETE		0

/* User provided definition */
#define FLASH_START_ADDRESS		0x08000000
#define FLASH_TOTAL_SIZE		(0x10000)		/* 64KB flash size */
#define FLASH_BOOTLOADER_SIZE	(0x5000)		/* 20KB Bootloader */

#define FLASH_APP_START_ADDRESS	(FLASH_START_ADDRESS + FLASH_BOOTLOADER_SIZE)
#define FLASH_APP_END_ADDRESS	(FLASH_APP_START_ADDRESS + FLASH_TOTAL_SIZE - FLASH_BOOTLOADER_SIZE)

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
void mtBootloaderInitFlash(void);
mtErrorCode_t mtBootloaderFlashWrite(uint32_t address, uint32_t data);
mtErrorCode_t mtBootloaderFlashWriteBuff(uint32_t address, uint32_t buff[], uint32_t len);
void mtBootloaderJumpToApp(uint32_t appOffset, uint32_t vtorOffset);
void mtBootloaderEraseAppFw(void);
uint32_t retAppPage(uint32_t relativePage);

FLASH_Status testWriteDummyDataToFlash(uint32_t startPage);


#endif /* TEMPLATE_H_ */
