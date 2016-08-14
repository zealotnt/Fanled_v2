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
#include "UartHandler/inc/mtUartHandler.h"
#include "UartHandler/inc/mtCcidHandler.h"
#include "UartHandler/inc/mtProtocolDriver.h"

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
extern myqueue_uart_t uart_buffer;


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
void mtUartResetQueue()
{
	mlsInitQueue(&uart_buffer);
}

void ccid_receive_state_machine(uint8_t char_read, myqueue_uart_t *buffer)
{
	static uint32_t startIdx = 0;
	const char SOF[] = "START";
	switch (buffer->state)
	{
	case CCID_UART_MSG_WAITING:
		if(char_read == 'S')
		{
			startIdx = 1;
			buffer->state = CCID_UART_MSG_STATE_STARTRECEIVING;
		}
		break;
	case CCID_UART_MSG_STATE_STARTRECEIVING:
		if(char_read == SOF[startIdx])
		{
			startIdx++;
			if(startIdx > 4)
			{
				buffer->state = CCID_UART_MSG_STATE_STARTDONE;
			}
		}
		else if (char_read == 'S')
		{
			startIdx = 1;
		}
		else
		{
			buffer->state = CCID_UART_MSG_WAITING;
		}
		break;
	case CCID_UART_MSG_STATE_STARTDONE:
		startIdx = 0;
		/* Get position of message in the buffer */
		buffer->start_of_msg = buffer->head;
		/* Now we waiting for length field (4 byte) */
		buffer->state = CCID_UART_MSG_STATE_WAITLENGHTBYTE;
		buffer->msg_length = 0;
		break;
	case CCID_UART_MSG_STATE_WAITLENGHTBYTE:
		/* Getting length field (4 byte) */
		buffer->state = CCID_UART_MSG_STATE_WAITLENGHTBYTE;
		buffer->msg_length += char_read << (startIdx*8);
		startIdx++;
		if(startIdx >= 4)
		{
			startIdx = 0;
			buffer->state = CCID_UART_MSG_STATE_WAITREMAINING;
		}
		break;
	case CCID_UART_MSG_STATE_WAITREMAINING:
		/* Waiting for remaining field */
		startIdx++;
		if(startIdx >= buffer->msg_length + 5)
		{
			buffer->state = CCID_UART_MSG_DONE;
			mtInterByteTimer_Disable();
		}
		break;
	default:
		break;
	}
}

void ProcessUartQueueCmd(void)
{
	if (uart_buffer.state == CCID_UART_MSG_DONE)
	{
		checkCcidUARTQueue(&uart_buffer.state, &uart_buffer.queue[uart_buffer.start_of_msg], uart_buffer.msg_length + 10);
		mtUartResetQueue();
	}
}

mtErrorCode_t checkUartString(const char* strToCheck,  myqueue_uart_t *buffer)
{
	uint8_t compare_buffer[MAX_UART_QUEUE_STORAGE];
	uint8_t *ptr = compare_buffer, buff_length = 0;
	uint8_t i, cnt = 0;
	while( mlsUartDequeue(buffer, ptr) == MT_SUCCESS )
	{
		ptr++;
		buff_length++;
	}
	for(i = 0; i < buff_length; i++)
	{
		if(compare_buffer[i] == '\r')
		{
			cnt++;
		}
	}
	if(cnt > 1)
	{
		// More than "new line" character received
		return MT_ERROR;
	}
	else
	{
		if( !strcmp(strToCheck, (const char *)compare_buffer) )
		{
			return MT_SUCCESS;
		}
		else
		{
			return MT_ERROR;
		}
	}
}

void USART1_IRQHandler(void)
{
#if STM32_PERIPH_LIB
	char char_rev;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		char_rev = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		mlsUartEnqueue(&uart_buffer, char_rev);
		ccid_receive_state_machine(char_rev, &uart_buffer);
		mtInterByteTimer_Reload(100);
	}
#endif
}

/************************* End of File ****************************************/
