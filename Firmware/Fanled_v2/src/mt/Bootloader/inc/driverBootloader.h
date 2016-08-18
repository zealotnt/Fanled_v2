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
#define FLASH_TOTAL_SIZE		(0x20000)
#define FLASH_BOOTLOADER_SIZE	(0xC000)

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
void mtBlInitFlash(void);
void mtBlFlashWrite(uint32_t address, uint32_t data);
void mtBlJumpToApp(uint32_t appOffset, uint32_t vtorOffset);
void mtBlEraseAppFw(void);
uint32_t retAppPage(uint32_t relativePage);

FLASH_Status testWriteDummyDataToFlash(uint32_t startPage);


#endif /* TEMPLATE_H_ */
