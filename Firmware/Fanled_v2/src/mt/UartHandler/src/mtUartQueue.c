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
#include "../UartHandler/inc/mtUartQueue.h"
#include "../UartHandler/inc/mtUartHandler.h"
#include "../UartHandler/inc/printf_lite.h"


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
myqueue_uart_t uart_buffer;

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
void mlsInitQueue(myqueue_uart_t *q)
{
	q->count = 0;
	q->tail = 0;
	q->head = SC_MAX_EVENT_STORAGE - 1;

	q->state =	CCID_UART_MSG_WAITING;
	q->msg_length = 0;
	q->start_of_msg = 0;

	memset(q->queue, 0x00, MAX_UART_QUEUE_STORAGE);
}

mtErrorCode_t mlsUartEnqueue(myqueue_uart_t *q, uint8_t value)
{
	mtErrorCode_t errCode = MT_SUCCESS;
	if (q->count >= MAX_UART_QUEUE_STORAGE)
	{
		return MT_ERROR;
	}
    else
    {
		q->head = (q->head + 1) % MAX_UART_QUEUE_STORAGE;
		q->queue[ q->head ] = value;
		q->count = q->count + 1;
    }
	return errCode;
}

mtErrorCode_t mlsUartDequeue(myqueue_uart_t *q, uint8_t *dataOut)
{
	mtErrorCode_t errCode = MT_SUCCESS;
	if (q->count <= 0) 
	{
		return MT_ERROR;
	}
	else
	{
		*dataOut = q->queue[ q->tail ];
		q->tail = (q->tail+1) % MAX_UART_QUEUE_STORAGE;
		q->count = q->count - 1;
	}
	return (errCode);
}

int mlsIsUartQueueEmpty(myqueue_uart_t *q)
{
	if (q->count <= 0) return (1);
	else return (0);
}

/************************* End of File ****************************************/
