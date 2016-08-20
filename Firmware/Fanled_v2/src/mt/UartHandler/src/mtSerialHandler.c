/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.c
**
** Module name       : template
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
#include "../inc/mtSerialHandler.h"
#include "../inc/mtSerialCmdParser.h"
#include "../inc/mtSerialPorting.h"
#include "FanledAPI/inc/mtFanledAPICode.h"

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
static const mtSerialAppAPIHandler gSerialReaderHandlerTable[] =
{
	{CMD_CODE_BASIC,	CTR_CODE_GETVERSION,		mtFanledApiGetFirmwareVersion},
	{CMD_CODE_BASIC, 	CTR_CODE_PROTOCOL_TEST, 	mtFanledApiProtocolTest},
	{0, 0, Null},
};
#define SERIAL_APP_API_TABLE_LEN		MT_ARRAY_SIZE(gSerialReaderHandlerTable)

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
static UInt32 mtSerial_FindTableItem(
    Const mtSerialAppAPIHandler table[],
    UInt32 dwTableLen,
    UInt8 bCmd,
    UInt8 bControl);


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
/**
 * @Function: mtSerial_FindTableItem
 */
static UInt32 mtSerial_FindTableItem(
    Const mtSerialAppAPIHandler table[],
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