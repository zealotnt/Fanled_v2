/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : driverBootloader.h
**
** Module name       : Bootloader
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

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
/* User provided definition */
#define FLASH_START_ADDRESS		0x08000000
#define FLASH_TOTAL_SIZE		(0x10000)		/* 64KB flash size */
#define FLASH_BOOTLOADER_SIZE	(0x4000)		/* 16KB Bootloader */

#define FLASH_APP_START_ADDRESS	(FLASH_START_ADDRESS + FLASH_BOOTLOADER_SIZE)
#define FLASH_APP_END_ADDRESS	(FLASH_APP_START_ADDRESS + FLASH_TOTAL_SIZE - FLASH_BOOTLOADER_SIZE)

#define BKP_PATTERN_JUMP_TO_APP			0xff00
#define BKP_PATTERN_UPGRADING			0x5678
#define BKP_PATTERN_REQ_UPGRADE			0x0000

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
typedef enum
{
	ERR_NONE				= 0x00,
	ERR_HARD_FAULT 			= 0x01,
	ERR_BKP_CLEAR			= 0x02,
	ERR_APP_CRC32_FAIL 		= 0x03,
} mtLastError_t;

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
Void mtBootloaderCoreReset();
Void mtBootloaderRequestUpgrade();
Void mtBootloaderWriteUpgradeBKPValue(UInt16 value);
Void mtBootloaderSaveBackupCRC32Value(UInt32 crc32);
Void mtBootloaderSaveBackupFwLenValue(UInt16 len);
Bool mtBootloaderCheckFwUpgardeRequest();
void mtBootloaderJumpToApp(uint32_t appOffset, uint32_t vtorOffset);
UInt32 mtBootloaderFlashCalculateCRC32(UInt8 *start_add, UInt16 len);
void mtBootloaderEraseAppFw(void);


#endif /* TEMPLATE_H_ */
