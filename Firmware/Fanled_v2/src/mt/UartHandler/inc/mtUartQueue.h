/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.h
**
** Module name       : template
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/
#ifndef INCLUDE_MYQUEUE_H_
#define INCLUDE_MYQUEUE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include <string.h>
#include "mtInclude.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define SC_MAX_EVENT_STORAGE		100
#define MAX_UART_QUEUE_STORAGE		100

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
typedef struct
{
	uint16_t 	head;
	uint16_t 	tail;
	int16_t 	count;
	//Specific for CCID handling
	uint16_t 	msg_count;
	uint8_t 	state;
	uint32_t 	msg_length;
	uint16_t 	start_of_msg;
	uint8_t queue[MAX_UART_QUEUE_STORAGE + 5];
}myqueue_uart_t;


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/
extern myqueue_uart_t uart_buffer;

/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
void mlsInitQueue(myqueue_uart_t *q);
mtErrorCode_t mlsUartEnqueue(myqueue_uart_t *q, uint8_t value);
mtErrorCode_t mlsUartDequeue(myqueue_uart_t *q, uint8_t *dataOut);
int mlsIsUartQueueEmpty(myqueue_uart_t *q);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MYQUEUE_H_ */
