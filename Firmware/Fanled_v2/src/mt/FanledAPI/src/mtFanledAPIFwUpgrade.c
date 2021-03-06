/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPIFwUpgrade.c
**
** Module name       : FanledAPI
**
**
** Summary:
**
**= History ====================================================================
** @date 	Feb 23, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include <string.h>

#include "mtInclude.h"
#include "../inc/mtFanledAPICode.h"
#include "Bootloader/inc/driverBootloader.h"
#include "UartHandler/inc/mtSerialCmdParser.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
Int32 app_packet_id = -1;
Int32 bl_packet_id = -1;
extern mtLastError_t gLastErr;

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
Void ResetHandler(Void *param)
{
	DEBUG_INFO("Jump back to Bootloader \r\n\r\n");
	mtBootloaderCoreReset();
}

Void JumpToApp(Void *param)
{
	DEBUG_INFO("Bootloader upgrade ok, jump to app \r\n");
	mtBootloaderJumpToApp(FLASH_APP_START_ADDRESS, FLASH_BOOTLOADER_SIZE);
}

/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
mtErrorCode_t mtFanledApiBlGetLastErr(UInt8 *msgIn,
                                      UInt16 msgInLen,
                                      UInt8 *msgOut,
                                      UInt16 *msgOutLen)
{
	*msgOutLen = 4;
	msgOut[3] = gLastErr;
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiRequestFirmwareUpgrade(UInt8 *msgIn,
                                                UInt16 msgInLen,
                                                UInt8 *msgOut,
                                                UInt16 *msgOutLen)
{
#if (FANLED_APP)
	mtBootloaderRequestUpgrade();
	mtSerialCmdDataLinkCallbackRegister(ResetHandler);
#endif
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareEraseBl(UInt8 *msgIn,
                                         UInt16 msgInLen,
                                         UInt8 *msgOut,
                                         UInt16 *msgOutLen)
{
	DEBUG_INFO("Get erase bootloader fw request\r\n");
	bl_packet_id = -1;
	mtBootloaderEraseBlFw();
	mtBootloaderWriteUpgradeBKPValue(BKP_PATTERN_UPGRADING);
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareEraseApp(UInt8 *msgIn,
                                          UInt16 msgInLen,
                                          UInt8 *msgOut,
                                          UInt16 *msgOutLen)
{
	DEBUG_INFO("Get erase app fw request\r\n");
	app_packet_id = -1;
	mtBootloaderEraseAppFw();
	mtBootloaderWriteUpgradeBKPValue(BKP_PATTERN_UPGRADING);
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareDownloadBl(UInt8 *msgIn,
                                            UInt16 msgInLen,
                                            UInt8 *msgOut,
                                            UInt16 *msgOutLen)
{
	mtDownloadCmdPacket_t *pCmd = (mtDownloadCmdPacket_t *)msgIn;
	mtDownloadResponsePacket_t *pRsp = (mtDownloadResponsePacket_t *)msgOut;
	UInt32 write_add;

	/* Check input param */
	if (bl_packet_id >= pCmd->PacketNumber)
	{
		/* Packet already written, ignore it */
		goto exit;
	}
	if (pCmd->PacketLen > MAX_DOWNLOAD_PACKET_LEN)
	{
		goto exit;
	}

	/* Set output parameters */
	pRsp->PacketNumber = pCmd->PacketNumber;
	pRsp->Status = API_COMMAND_EXECUTE_SUCCESS;
	bl_packet_id = pCmd->PacketNumber;

	if (MAX_DOWNLOAD_PACKET_LEN == pCmd->PacketLen)
	{
		write_add = (MAX_DOWNLOAD_PACKET_LEN * pCmd->PacketNumber) + FLASH_BOOTLOADER_START_ADDRESS;
	}
	else
	{
		write_add = (MAX_DOWNLOAD_PACKET_LEN * (pCmd->PacketNumber - 1)) + pCmd->PacketLen + FLASH_BOOTLOADER_START_ADDRESS;
	}

	/* Write to flash */
	DEBUG_INFO("Writing bl_packet_id %d\r\n", pCmd->PacketNumber);
	mtBootloaderFlashWriteBuff(write_add, (uint32_t *)pCmd->PacketData, pCmd->PacketLen / 4);

exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareDownloadApp(UInt8 *msgIn,
                                             UInt16 msgInLen,
                                             UInt8 *msgOut,
                                             UInt16 *msgOutLen)
{
	mtDownloadCmdPacket_t *pCmd = (mtDownloadCmdPacket_t *)msgIn;
	mtDownloadResponsePacket_t *pRsp = (mtDownloadResponsePacket_t *)msgOut;
	UInt32 write_add;

	/* Check input param */
	if (app_packet_id >= pCmd->PacketNumber)
	{
		/* Packet already written, ignore it */
		goto exit;
	}
	if (pCmd->PacketLen > MAX_DOWNLOAD_PACKET_LEN)
	{
		goto exit;
	}

	/* Set output parameters */
	pRsp->PacketNumber = pCmd->PacketNumber;
	pRsp->Status = API_COMMAND_EXECUTE_SUCCESS;
	app_packet_id = pCmd->PacketNumber;

	if (MAX_DOWNLOAD_PACKET_LEN == pCmd->PacketLen)
	{
		write_add = (MAX_DOWNLOAD_PACKET_LEN * pCmd->PacketNumber) + FLASH_APP_START_ADDRESS;
	}
	else
	{
		write_add = (MAX_DOWNLOAD_PACKET_LEN * (pCmd->PacketNumber - 1)) + pCmd->PacketLen + FLASH_APP_START_ADDRESS;
	}

	/* Write to flash */
	DEBUG_INFO("Writing app_packet_id %d\r\n", pCmd->PacketNumber);
	mtBootloaderFlashWriteBuff(write_add, (uint32_t *)pCmd->PacketData, pCmd->PacketLen / 4);

exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareChecksumBl(UInt8 *msgIn,
                                            UInt16 msgInLen,
                                            UInt8 *msgOut,
                                            UInt16 *msgOutLen)
{
	mtChecksumPacket_t *pCmd = (mtChecksumPacket_t *)msgIn;

	/* Calculate self checksum */
	UInt32 self_crc32 = mtBootloaderFlashCalculateCRC32((UInt8 *)FLASH_BOOTLOADER_START_ADDRESS, pCmd->FirmwareSize);

	/* Compare it with host's command */
	if (self_crc32 != pCmd->CRC32)
	{
		msgOut[2] = API_CHECK_CRC_ERROR;
		goto exit;
	}

	mtBootloaderWriteUpgradeBKPValue(BKP_PATTERN_OK_JUMP_TO_APP);
	mtSerialCmdDataLinkCallbackRegister(ResetHandler);
exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareChecksumApp(UInt8 *msgIn,
                                             UInt16 msgInLen,
                                             UInt8 *msgOut,
                                             UInt16 *msgOutLen)
{
	mtChecksumPacket_t *pCmd = (mtChecksumPacket_t *)msgIn;

	/* Calculate self checksum */
	UInt32 self_crc32 = mtBootloaderFlashCalculateCRC32((UInt8 *)FLASH_APP_START_ADDRESS, pCmd->FirmwareSize);

	/* Compare it with host's command */
	if (self_crc32 != pCmd->CRC32)
	{
		msgOut[2] = API_CHECK_CRC_ERROR;
		goto exit;
	}

	mtBootloaderSaveBackupCRC32Value(pCmd->CRC32);
	mtBootloaderSaveBackupFwLenValue(pCmd->FirmwareSize);
	mtBootloaderWriteUpgradeBKPValue(BKP_PATTERN_OK_JUMP_TO_APP);
	mtSerialCmdDataLinkCallbackRegister(JumpToApp);

exit:
	return MT_SUCCESS;
}
/************************* End of File ****************************************/


