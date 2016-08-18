/********** Include section ***************************************************/
#include "../inc/mtSerialHandler.h"
#include "../inc/mtSerialCmdParser.h"
#include "../inc/mtSerialPorting.h"

#include <string.h>
/********** Local Constant and compile switch definition section **************/

/********** Local Type definition section *************************************/

/********** Local Macro definition section ************************************/
#define DEBUG_READERAPI_HANDLER_PRINT(...)			printf(__VA_ARGS__)
#define DEBUG_READERAPI_HANDLER_NOTIFY_BAD(...)		DEBUG_SERIAL_PRINT_NOTIFY_BAD(__VA_ARGS__)

/********** Local (static) variable definition section ************************/

static const mtSerialAppAPIHandler gSerialReaderHandlerTable[] =
{
	{0, 0, Null},
};
#define SERIAL_APP_API_TABLE_LEN		MT_ARRAY_SIZE(gSerialReaderHandlerTable)

static mtSerialAppAPIHandler *pReaderUsingTable = (mtSerialAppAPIHandler *)&gSerialReaderHandlerTable;

/********** Local (static) function declaration section ***********************/
static UInt32 mtSerial_FindTableItem(
							Const mtSerialAppAPIHandler table[],
							UInt32 dwTableLen,
							UInt8 bCmd,
							UInt8 bControl);

/********** Local function definition section *********************************/

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

/********** Global function definition section ********************************/
/**
 * @Function: mtSerialProcessCmdPacket
 */
mtErrorCode_t mtSerialProcessCmdPacket(UInt8 *pMsgIn, UInt16 wMsgInLen, UInt8 *pMsgOut, UInt16 *pwMsgOutLen)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	serialCmdData_t *pDat = (serialCmdData_t *)pMsgIn;
	UInt32 dwCmdIdx = 0;

	dwCmdIdx = mtSerial_FindTableItem(pReaderUsingTable, SERIAL_APP_API_TABLE_LEN,
							pDat->CmdCode, pDat->ControlCode);

	switch(dwCmdIdx)
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
			gSerialReaderHandlerTable[dwCmdIdx].fnHandler(pMsgIn, wMsgInLen, pMsgOut, pwMsgOutLen);
			break;
	}
exit:
	return retVal;
}

/**
 * EOF
 */
