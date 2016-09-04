/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledHandler.c
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
#include "mtInclude.h"
#include "UartHandler/inc/mtSerialCmdParser.h"
#include "UartHandler/inc/mtSerialPorting.h"
#include "../inc/mtFanledHandler.h"
#include "../inc/mtFanledAPICode.h"
#include "../inc/mtFanledAPISdCard.h"
#include "../inc/mtFanledAPIBasic.h"
#include "../inc/mtFanledAPIFwUpgrade.h"

#include <string.h>

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
const mtSerialAppAPIHandler gSerialReaderHandlerTable[] =
{
#if (FANLED_BOOTLOADER)
	{CMD_CODE_BASIC,	CTR_CODE_BL_GET_LAST_ERR,	mtFanledApiBlGetLastErr},
	{CMD_CODE_BASIC,	CTR_CODE_UPGRADEFW,			mtFanledApiRequestFirmwareUpgrade},
	{CMD_CODE_BASIC,	CTR_CODE_FW_DOWNLOAD,		mtFanledApiFirmwareDownloadApp},
	{CMD_CODE_BASIC,	CTR_CODE_FW_CHECKSUM,		mtFanledApiFirmwareChecksumApp},
	{CMD_CODE_BASIC,	CTR_CODE_ERASE_APP,			mtFanledApiFirmwareEraseApp},

	{CMD_CODE_BASIC,	CTR_CODE_FW_DOWNLOAD_BL,		mtFanledApiFirmwareDownloadBl},
	{CMD_CODE_BASIC,	CTR_CODE_FW_CHECKSUM_BL,		mtFanledApiFirmwareChecksumBl},
	{CMD_CODE_BASIC,	CTR_CODE_ERASE_BL,				mtFanledApiFirmwareEraseBl},

#elif (FANLED_APP)
	{CMD_CODE_BASIC,	CTR_CODE_UPGRADEFW,			mtFanledApiRequestFirmwareUpgrade},
	{CMD_CODE_BASIC,	CTR_CODE_GETSET_RTC,		mtFanledApiGetSetRTC},
	{CMD_CODE_BASIC,	CTR_CODE_HARDFAULT,			mtFanledApiHardFault},

	{CMD_CODE_SD,	CTR_CODE_LIST_FILE,				mtFanledApiSdListFile},
	{CMD_CODE_SD,	CTR_CODE_INSPECT_FILE,			mtFanledApiSdInspect},
	{CMD_CODE_SD,	CTR_CODE_READ_FILE,				mtFanledApiSdReadFile},
	{CMD_CODE_SD,	CTR_CODE_DELETE_FILE,			mtFanledApiSdDeleleFile},
	{CMD_CODE_SD,	CTR_CODE_WRITE_FILE,			mtFanledApiSdWriteFile},

#endif
	/* Common API for both Bootloader and Application */
	{CMD_CODE_BASIC,	CTR_CODE_GETVERSION,		mtFanledApiGetFirmwareVersion},
	{CMD_CODE_BASIC, 	CTR_CODE_PROTOCOL_TEST, 	mtFanledApiProtocolTest},
	{CMD_CODE_BASIC, 	CTR_CODE_GET_DBG_VAR, 		mtFanledApiGetDbgVar},
	{0, 0, Null},
};
#define SERIAL_APP_API_TABLE_LEN		MT_ARRAY_SIZE(gSerialReaderHandlerTable)


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
static UInt32 mtSerial_FindTableItem(Const mtSerialAppAPIHandler table[],
                                     UInt32 dwTableLen,
                                     UInt8 bCmd,
                                     UInt8 bControl);

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
/**
 * @Function: mtSerial_FindTableItem
 */
static UInt32 mtSerial_FindTableItem(Const mtSerialAppAPIHandler table[],
                                     UInt32 dwTableLen,
                                     UInt8 bCmd,
                                     UInt8 bControl)
{
	UInt32 dwIdx;
	Bool bFoundCmdCode = False;

	for (dwIdx = 0; dwIdx < dwTableLen; dwIdx++)
	{
		if ((table[dwIdx].CmdCode == bCmd))
		{
			bFoundCmdCode = True;
			if ((table[dwIdx].ControlCode == bControl))
			{
				/*found an item, break and return*/
				break;
			}

		}
	}

	if (dwIdx >= dwTableLen)
	{
		if (bFoundCmdCode == True)
		{
			/*item not found, but Command Code is matched */
			dwIdx = 0xfff0;
		}
		else
		{
			/*item not found, neither Control Code nor Command Code is matched */
			dwIdx = 0xffff;
		}
	}

	return dwIdx;
}

/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
/**
 * @Function: mtSerialProcessCmdPacket
 */
mtErrorCode_t mtSerialProcessCmdPacket(UInt8 *pMsgIn, UInt16 wMsgInLen, UInt8 *pMsgOut, UInt16 *pwMsgOutLen)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	serialCmdData_t *pDat = (serialCmdData_t *)pMsgIn;
	UInt32 dwCmdIdx = 0;

	dwCmdIdx = mtSerial_FindTableItem(gSerialReaderHandlerTable, SERIAL_APP_API_TABLE_LEN,
	                                  pDat->CmdCode, pDat->ControlCode);

	switch (dwCmdIdx)
	{
		/* Control not supported by CTRU application */
		case 0xffff:
		{
			UInt8 *pDat = (UInt8 *)pMsgOut;
			pDat[0] = 0xFF;
			pDat[1] = 0x7F;
			*pwMsgOutLen = 2;
			goto exit;
		}

		/* Command not supported by any CTRU application */
		case 0xfff0:
		{
			UInt8 *pDat = (UInt8 *)pMsgOut;
			UInt8 *pDatIn = (UInt8 *)pMsgIn;
			pDat[0] = pDatIn[0];
			pDat[1] = 0x7F;
			*pwMsgOutLen = 2;
			goto exit;
		}
		default:
			mtFanledApiSetResponseCode(pMsgIn, pMsgOut, pwMsgOutLen);
			gSerialReaderHandlerTable[dwCmdIdx].fnHandler(pMsgIn, wMsgInLen, pMsgOut, pwMsgOutLen);
			break;
	}
exit:
	return retVal;
}

/************************* End of File ****************************************/
