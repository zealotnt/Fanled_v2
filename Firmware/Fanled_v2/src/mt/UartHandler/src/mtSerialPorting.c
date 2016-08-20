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
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"

#include "mtInclude.h"
#include "../inc/mtSerialPorting.h"
#include "../inc/mtSerialCmdParser.h"
#include "../inc/mtProtocolDriver.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define DEFAULT_INTER_BYTE_INTERVAL					50

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
extern serialQueuePayload_t gQueuePayload;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
/**
 * @Function: mtSerialPort_InterByteTimerReload
 */
Void mtSerialPort_InterByteTimerReload(UInt32 dwMsTimeOut)
{
	mtInterByteTimer_Reload(dwMsTimeOut);
}

/**
 * @Function: mtSerialPort_InterByteTimerDisable
 */
Void mtSerialPort_InterByteTimerDisable(Void)
{
	mtInterByteTimer_Disable();
}

/**
 * @Function: mtSerialPort_InterByteTimerInit
 */
mtErrorCode_t mtSerialPort_InterByteTimerInit(Void)
{
	mtInterByteTimer_Init();
	return MT_SUCCESS;
}

/**
 * @Function: mtSerialPort_InterByteTimerInit
 */
Void mtSerialPort_InterByteTimerRegister(pTimerTimeoutHandler *timer, pTimerTimeoutHandler fncHandler)
{
	if (fncHandler != Null)
	{
		*timer = fncHandler;
	}
}

/**
 * @Function: mtSerialPort_GetBaurate
 */
mtBaudrate_t mtSerialPort_GetBaurate(Void)
{
	mtBaudrate_t baud;
	/* For faster interrupt time, return default interval inter-byte timeout */
	baud = BR_9600;
	/* This method return exact value of baudrate --> but takes alot of times --> can't work with 460800
	 * return mtBluefinSerialGetBaudIndex();
	 */
	return baud;
}

/**
 * @Function: mtSerialPort_InterByteTimerGetInverval
 */
UInt32 mtSerialPort_InterByteTimerGetInverval(Void)
{
	/* In case can't find anything, just set the interval as default */
	return DEFAULT_INTER_BYTE_INTERVAL;
}

/**
 * @Function: mtSerialPort_Write
 */
Void mtSerialPort_Write(UInt8 *pData, UInt16 wLenToWrite)
{
	mtUartWriteBuf(pData, wLenToWrite);
}

/**
 * @Function: mtSerialPort_FlushRxData
 */
Void mtSerialPort_FlushRxData(Void)
{
	/* Call this function to fix bug: when trying to send packet with wrong baud rate
	 * --> There still bytes in RX FIFO
	 * --> Can't reiceive full packet in next "right" baud rate packet */
}

Int32 mtSerialPort_GetRxFifo(Void)
{
	return 0;
}

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
/* inter-byte timer */
void mtBluetoothInterbyteTimerHandler(void)
{
	mtInterByteTimer_Disable();
	mtSerialCmd_InterByteTimeOutHandling(&gQueuePayload);
}

/**
 * @Function: mtUARTRcvHandler
 */
void mtBluetoothRcvHandler(void)
{
	static UInt32 dwTotalDataLen = 0;
	UInt8 bCharRev;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		bCharRev = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);

		if (mtSerialCmdRcvStateHandling((UInt8)bCharRev, &gQueuePayload, &dwTotalDataLen) != ROUTINE_RET_NO_CHANGE)
		{
			gQueuePayload.type = DATA_TYPE;
			gQueuePayload.Done = True;
		}
	}

	if (USART_GetITStatus(USART1, USART_IT_ORE) == SET)
	{
		USART_ClearFlag(USART1, USART_IT_ORE);
	}

	/** We're done */
	return;
}

/************************* End of File ****************************************/
