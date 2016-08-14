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
#include "mtInclude.h"
#include "UartHandler/inc/mtCcidHandler.h"
#include "UartHandler/inc/mtUartHandler.h"
#include "UartHandler/inc/mtProtocolDriver.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define CMD_HANDLE_SIZE                 100
#define TIMEOUT							10
#define APDU_SIZE						261
#define QES	272

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/

/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
const mtCcidHandler_t gCcidHandlerTable[] =
{
	{ (UInt8) CCID_TEST_ECHO,	mlsFanledApi_Echo},
	{ (UInt8) 0x00,				(fnCcidHandler) Null},
};

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
static fnCcidHandler SeekHandler(const mtCcidHandler_t *handlerTable, UInt8 bMessageType);


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
static fnCcidHandler SeekHandler(const mtCcidHandler_t *handlerTable, UInt8 bMessageType)
{
	UInt8 idx = 0;

	while (handlerTable[idx].bMessageType != 0x00)
	{
		if (bMessageType == handlerTable[idx].bMessageType)
		{
			return handlerTable[idx].fnHandler;
		}
		idx ++;
	}

	return (fnCcidHandler) Null;
}

/* check & process CCID normal message
 * - check data length
 * - process cmdQueue with input condition
 */
static mtErrorCode_t checkCcidNormalMessage(UInt8 *CcidMgs, UInt32 CcidMgsLen)
{
	mtErrorCode_t errorCode = MT_SUCCESS;
	UInt32 apduLen = 0x00 | ((CcidMgs[1] << 0) & 0xff) | ((CcidMgs[2] << 8)
		& 0xff00) | ((CcidMgs[3] << 16) & 0xff0000) | ((CcidMgs[4] << 24)
		& 0xff000000);
	//check length of message
	if (apduLen != (CcidMgsLen - 10))
	{
		errorCode = MT_ERR_BAD_FORMAT;
	}
	return errorCode;
}


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
mtErrorCode_t mlsFanledApi_Echo(UInt8 *msgIn, UInt16 msgInLen, UInt8 *msgOut, UInt16 *msgOutLen)
{
	memcpy(msgOut, msgIn, msgInLen);
	*msgOutLen = msgInLen;
	return MT_SUCCESS;
}

int32_t mtCcidWriteData(uint8_t *pData, uint32_t length)
{
	mtUartWriteBuf(pData, length);
	return 0;
}

int32_t checkCcidUARTQueue(uint8_t *flag, uint8_t *cmd, uint16_t cmdLen)
{
	mtErrorCode_t errorCode = MT_SUCCESS;
	fnCcidHandler ccidHandler = Null;
	UInt8 rsp[CMD_HANDLE_SIZE];
	UInt16 rspLen;
	if (*flag == CCID_UART_MSG_DONE)
	{
		*flag = CCID_UART_MSG_WAITING;
		/*2. Check is CcidMessage valid?*/
		errorCode = checkCcidNormalMessage(cmd, cmdLen);
		if (errorCode != MT_SUCCESS)
		{
			return MT_ERROR;
		}
		/*3. Find ccidHandler to handler CcidMessage */
		ccidHandler = SeekHandler(gCcidHandlerTable, cmd[0]);
		if (ccidHandler != Null)
		{
			ccidHandler(cmd, cmdLen, rsp, &rspLen);
			mtCcidWriteData(rsp, rspLen);
		}
		else
		{
			return MT_ERROR;
		}
	}
	else
	{
		return MT_ERROR;
	}
	return errorCode;	
}

/************************* End of File ****************************************/
