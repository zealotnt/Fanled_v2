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


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
Void ResetHandler(Void *param)
{
	DEBUG_INFO("Jump back to Bootloader\r\n");
	mtBootloaderCoreReset();
}

mtErrorCode_t mtFanledApiRequestFirmwareUpgrade(UInt8 *msgIn,
                                                UInt16 msgInLen,
                                                UInt8 *msgOut,
                                                UInt16 *msgOutLen)
{
	mtBootloaderRequestUpgrade();
	mtSerialCmdDataLinkCallbackRegister(ResetHandler);
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareDownload(UInt8 *msgIn,
                                          UInt16 msgInLen,
                                          UInt8 *msgOut,
                                          UInt16 *msgOutLen)
{
	static Int32 packet_id = -1;
	mtDownloadCmdPacket_t *pCmd = (mtDownloadCmdPacket_t *)msgIn;
	mtDownloadResponsePacket_t *pRsp = (mtDownloadResponsePacket_t *)msgOut;

	/* Check input param */

	if (pCmd->PacketNumber <= packet_id)
	{
		/* Packet already written, ignore it */
		goto exit;
	}

	/* Set output parameters */
	pRsp->PacketNumber = pCmd->PacketNumber;
	pRsp->Status = API_COMMAND_EXECUTE_SUCCESS;

	/* Write to flash */

exit:
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiFirmwareChecksum(UInt8 *msgIn,
                                          UInt16 msgInLen,
                                          UInt8 *msgOut,
                                          UInt16 *msgOutLen)
{
	mtChecksumPacket_t *pCmd = (mtChecksumPacket_t *)msgIn;

	/* Check input param */

	/* Calculate self checksum */

	/* Compare it with host's command */

	return MT_SUCCESS;
}
/************************* End of File ****************************************/


