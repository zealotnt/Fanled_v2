/********** Include section ***************************************************/
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

/********** Local Constant and compile switch definition section **************/
#define DEFAULT_INTER_BYTE_INTERVAL					50

/********** Local Type definition section *************************************/

/********** Local Macro definition section ************************************/

/********** Local (static) variable definition section ************************/

/********** Local (static) function declaration section ***********************/

/********** Global variable definition section ********************************/
const mtBaudTimerIntervalTable_t mtCommBaudTimerLookUpTable[] =
{
	{ BR_9600,		50 	},
	{ BR_19200,		40	},
	{ BR_115200,	30	},
	{ BR_230400,	20	},
	{ BR_460800,	20	},
};

#define NUM_OF_BAUD_AVAIL		sizeof(mtCommBaudTimerLookUpTable)	\
								/ sizeof(mtBaudTimerIntervalTable_t)

extern serialQueuePayload_t gQueuePayload;

/********** Local function definition section *********************************/

/**
 * @Function: mtSerialPort_InterByteTimerReload
 */
mtErrorCode_t mtSerialPort_InterByteTimerReload(UInt32 dwMsTimeOut)
{
	mtInterByteTimer_Reload(dwMsTimeOut);
	return MT_SUCCESS;
}

/**
 * @Function: mtSerialPort_InterByteTimerDisable
 */
mtErrorCode_t mtSerialPort_InterByteTimerDisable(/*mtTimerID_t *timerId*/Void)
{
	mtInterByteTimer_Disable();
	return MT_SUCCESS;
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
mtErrorCode_t mtSerialPort_InterByteTimerRegister(pTimerTimeoutHandler *timer, pTimerTimeoutHandler fncHandler)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	if (fncHandler != Null)
	{
		*timer = fncHandler;
	}
	else
	{
		DEBUG_SERIAL_ERROR_PRINT("Null handler function");
		retVal = MT_ERROR;
	}
	return retVal;
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
	UInt32 dwIdx;
	mtBaudrate_t curBaud = mtSerialPort_GetBaurate();

	for(dwIdx = 0; dwIdx < NUM_OF_BAUD_AVAIL; dwIdx++)
	{
		if(curBaud == mtCommBaudTimerLookUpTable[dwIdx].baud)
		{
			return mtCommBaudTimerLookUpTable[dwIdx].inteval;
		}
	}
	/* In case can't find anything, just set the interval as minimum */
	return DEFAULT_INTER_BYTE_INTERVAL;
}

/* inter-byte timer */
void mtBluetoothInterbyteTimerHandler(void)
{
	TIM3->SR = 0;
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

	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		bCharRev = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);

		if (mtSerialCmdRcvStateHandling((UInt8)bCharRev, &gQueuePayload, &dwTotalDataLen) != ROUTINE_RET_NO_CHANGE)
		{
			gQueuePayload.type = DATA_TYPE;
			mtSerialCmdDataLinkHandlingThread(gQueuePayload);
		}
	}

	/** We're done */
	return;
}
/********** Global function definition section ********************************/

/**
 * @Function: mtUartSerialInit
 */
mtErrorCode_t mtUartSerialInit(Void)
{
	return MT_SUCCESS;
}

/**
 * @Function: mtSerialPort_Write
 */
mtErrorCode_t mtSerialPort_Write(UInt8 *pData, UInt16 wLenToWrite)
{
	mtUartWriteBuf(pData, wLenToWrite);
	return MT_SUCCESS;
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
/**
 * EOF
 */
