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

#ifndef INCLUDE_MYUARTHANDLER_H_
#define INCLUDE_MYUARTHANDLER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include "mtUartQueue.h"
#include "mtInclude.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define CCID_UART_MSG_STATE_STARTRECEIVING		1		/* S T A R T = 5b*/
#define CCID_UART_MSG_STATE_STARTDONE			5
#define CCID_UART_MSG_STATE_WAITLENGHTBYTE		6		/* Mess Type = 1b*/
#define CCID_UART_MSG_STATE_WAITREMAINING		11		/* until all of message received */
#define	CCID_UART_MSG_DONE				100
#define	CCID_UART_MSG_WAITING			0

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
void mtUartResetQueue();
void ccid_receive_state_machine(uint8_t char_read, myqueue_uart_t *buffer);
void USART1_IRQHandler(void);
mtErrorCode_t checkUartString(const char* strToCheck,  myqueue_uart_t *buffer);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MYUARTHANDLER_H_ */
