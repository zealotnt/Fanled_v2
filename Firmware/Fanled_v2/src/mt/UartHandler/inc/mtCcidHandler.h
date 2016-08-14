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

#ifndef INCLUDE_MT_CCIDHANDLER_H_
#define INCLUDE_MT_CCIDHANDLER_H_


#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include "mtInclude.h"
#include <stdint.h>

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

#define CCID_EP_BULK_OUT    0x01
#define CCID_EP_BULK_IN     0x81
#define CCID_EP_INTR_IN     0x82

#define MAX_CCIDMSG_SIZE	272

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
/** fnCcidHandler
 * @brief: declare CcidHandler function pointer type
 * @param[in]: mesgIn is message input
 * @param[out]: mesgOut is message output
 * @param[int]: timeOutMs is milliseconds to handle input message to output message
 */
typedef mtErrorCode_t (*fnCcidHandler) (UInt8 *msgIn, UInt16 msgInLen, UInt8 *msgOut, UInt16 *msgOutLen);

typedef enum ccid_command
{
	CCID_READ_BLOCK = 0x01,
	CCID_WRITE_BLOCK = 0x02,
	CCID_TEST_ECHO = 0xfe,
} ccid_command_t;

/* This is the error to be reported in the bError and bStatus */
typedef enum ccid_error
{
	CCID_ERROR_NONE = 0x00,
} ccid_error_t;

#pragma pack(1)

/**
 * mtCcidHandler_t
 * @brief: declare mtCcidHandler_t struct
 */
typedef struct
{
	UInt8			bMessageType;			/** Message type */
	fnCcidHandler	fnHandler;				/** Function pointer to handler */
} mtCcidHandler_t;
#pragma pack()

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
int32_t checkCcidUARTQueue(uint8_t *flag, uint8_t *cmd, uint16_t cmdLen);
int32_t mtCcidWriteData(uint8_t *pData, uint32_t length);
mtErrorCode_t mlsFanledApi_Echo(UInt8 *msgIn, UInt16 msgInLen, UInt8 *msgOut, UInt16 *msgOutLen);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MT_CCIDHANDLER_H_ */
