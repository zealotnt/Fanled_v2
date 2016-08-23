/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtSerialCmdParser.h
**
** Module name       : UartHandler
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef _MTSERIALCMDPARSER_H_
#define _MTSERIALCMDPARSER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include "mtInclude.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/

/*! \def SERIAL_FI_DEVICE_TO_RF_PROCESSOR
	Second FI byte use when transfer Command from Host to RF processor
*/
#define SERIAL_FI_DEVICE_TO_RF_PROCESSOR				0x35

/*! \def SERIAL_FI_RF_PROCESSOR_TO_DEVICE
	Second FI byte use when transfer Response from RF processor to Host
*/
#define SERIAL_FI_RF_PROCESSOR_TO_DEVICE				0x3A

/*! \def SERIAL_FI_FIRSTBYTE
	First FI byte use when transfer Response from RF processor to Host
*/
#define SERIAL_FI_FIRSTBYTE								0x00

/*! \def SERIAL_NUM_OF_LENGTH_BYTE
	Number of length bytes in BluefinSerial protocol
*/
#define SERIAL_NUM_OF_LENGTH_BYTE						3

/*! \def MAX_SERIAL_DATA_EXCEPT_CMD
	Maximum value of bytes in Data field in BluefinSerial protocol
*/
#define MAX_SERIAL_DATA_EXCEPT_CMD						512

/*! \def MAX_SERIAL_DEBUG_MSG_LEN
	Maximum value of bytes in Debug message from App processor send to Host at a time
*/
#define MAX_SERIAL_DEBUG_MSG_LEN						512

/*! \def SERIAL_PACKET_UNKNOWN
	Unknown packet format
*/
#define SERIAL_PACKET_UNKNOWN							(UInt8)(0)

/*! \def SERIAL_PACKET_ACK
	ACK packet format
*/
#define SERIAL_PACKET_ACK								(UInt8)(1)

/*! \def SERIAL_PACKET_NACK
	NACK packet format
*/
#define SERIAL_PACKET_NACK								(UInt8)(2)

/*! \def SERIAL_PACKET_DATA
	Data packet format
*/
#define SERIAL_PACKET_DATA								(UInt8)(3)

/*! \def MAX_SERIAL_MSG_QUEUE_SIZE
	Maximum BluefinSerial message queue size
*/
#define MAX_SERIAL_MSG_QUEUE_SIZE					sizeof(serialQueuePayload_t)

/*! \def MAX_SERIAL_DATAPAYLOAD_IN_PACKET_SIZE
	Size of BluefinSerial frame
*/
#define MAX_SERIAL_DATAPAYLOAD_IN_PACKET_SIZE		sizeof(serialRcvFrame_t) /* 2FI + 3LEN + 512DATA + 1CRC = 518 */

#define UART_QUEUE_MAX_COUNT						200

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
/*!
 *	\brief decision output from Receive State Machine
 */
typedef enum
{
	ROUTINE_RET_NO_CHANGE,		/*!< mtSerialRcvStateHandling return with no data outcome */
	ROUTINE_RET_PUSH_DATA		/*!< mtSerialRcvStateHandling return with data push to mlsDataLinkHandlingThread */
} mtSerialRcvRoutineDecision_t;

/*!
 *	\brief List all of Bluefin Serial state machine's state
 */
typedef enum
{
	RCV_IDLE,		/*!< Idle state, waiting for FI */
	RCV_IF,			/*!< receiving FI bytes state */
	RCV_LEN,		/*!< receiving Len bytes state */
	RCV_DATA,		/*!< receiving Data bytes state */
	RCV_DONE,		/*!< done receiving packet */
	RCV_ERROR,		/*!< error occurred, wait for timeout */
} receiveRoutineState_t;

/*!
 *	\brief List all queue type
 */
typedef enum
{
	DATA_TYPE,					/*!< Data queue type (normal Command packet) */
	DATA_FORWARDING_TYPE,		/*!< Forwarding queue type (forward Host Command to RF processor) */
	DATA_FROM_RF,				/*!< Packet from RF queue type (not use yet) */
	SEND_CMD_TYPE,				/*!< Command sending queue type (Only for Host mode) */
	EXCEPTION_TYPE				/*!< Exception queue type (not use yet) */
} serialQueueType_t;

/*!
 *	\brief Contain all error in Bluefin Serial Datalink layer
 */
typedef enum
{
	ERROR_NONE,									/*!< No error occurred yet (first state) */
	ERROR_INVALID_IF1 				= 0xF0,		/*!< Error when receiving FI1 */
	ERROR_INVALID_IF2 				= 0xF1,		/*!< Error when receiving FI2 */
	ERROR_INVALID_LEN_LCS 			= 0xF2,		/*!< LCS check error */
	ERROR_LEN_MISMATCH 				= 0xF3,		/*!< Data received not same as Len in packet's Len field */
	ERROR_LEN_TOO_BIG 				= 0xF4,		/*!< Packet's len field too big (out of specs) */
	ERROR_INVALID_CRC 				= 0xF5,		/*!< CRC check error */
	ERROR_INTER_BYTE_TIMEOUT		= 0xF6,		/*!< Interbyte receive timeout */
	ERROR_SERIAL_BUFFER_OVERFLOW	= 0xF7,		/*!< Receive too much (more than 512 + 6 bytes) */
	ERROR_NO_RESPONSE				= 0xF8,		/*!< No response from App/RF processor (Host mode) */
	ERROR_NOT_EXPECTED_RSP			= 0xF9,		/*!< Not expect response (Host mode) */
	ERROR_CMD_NOT_SUPPORTED 		= 0xFA,		/*!< Command not supported by App/RF processor (Host mode) */
	RCV_SUCCESS 					= 0x00		/*!< */
} serialErrorType_t;

#pragma pack(1)
/*!
 *	\brief Header FI in Communication packet frame structure
 */
typedef struct
{
	UInt8 FIl;			/*!< FI Low byte  */
	UInt8 FIm;			/*!< FI High byte */
} serialHeaderFI_t;

/*!
 *	\brief Header LEN in Communication packet frame structure
 */
typedef struct
{
	UInt8 Lenl;			/*!< Len Low byte   */
	UInt8 Lenm;			/*!< Len High byte  */
	UInt8 LCS;			/*!< Len LCS byte   */
} serialHeaderLen_t;

/*!
 *	\brief Data field in Command packet frame structure
 */
typedef struct
{
	UInt8 CmdCode;									/*!< Command Code byte 		*/
	UInt8 ControlCode;								/*!< Control Code byte 		*/
	UInt8 Dt[MAX_SERIAL_DATA_EXCEPT_CMD - 2];		/*!< Data packet frame byte */
} serialCmdData_t;

/*!
 *	\brief Data field in Response packet frame structure
 */
typedef struct
{
	UInt8 RspCode;									/*!< Response Command Code byte 		*/
	UInt8 RspControlCode;							/*!< Response Control Code byte 		*/
	UInt8 Dt[MAX_SERIAL_DATA_EXCEPT_CMD - 2];		/*!< Data packet frame byte 	 		*/
} serialRspHeaderData_t;

/*!
 *	\brief Full frame of Command/Response packet frame structure
 */
typedef struct
{
	serialHeaderFI_t 		IF;			/*!< FI field */
	serialHeaderLen_t 		Len;		/*!< Len field */
	serialCmdData_t 		Data;		/*!< Data field */
	UInt8 					CRC8;		/*!< CRC field */
} serialRcvFrame_t;

/*!
 *	\brief Message queue frame that contain Command/Response packet,
 *	this queue is used to transfer packet between ReceiveStateMachine and CommandHandlingThread
 */
typedef struct
{
	serialQueueType_t 		type;				/*!< queue type */
	serialErrorType_t 		errorFlag;			/*!< error flag */
	serialRcvFrame_t 		serialDataFrame;	/*!< receive/send packet */
	UInt32					rcvLen;				/*!< counter for receive length (use in state machine - reset when failed) */
	UInt32					lenMonitoring;		/*!< counter for receive length (always monitoring - even failed, reset when begin new packet) */
	receiveRoutineState_t	rcvState;			/*!< receive state (feed to mtSerialRcvStateHandling) */
	Bool					Done;
	Bool					InitHC;
	UInt8					BufHead;
	UInt8					BufTail;
	UInt8					BufCount;
} volatile serialQueuePayload_t;
#pragma pack()

typedef void (*pCmdHandlerCallback)(Void *param);

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/
extern volatile serialQueuePayload_t gQueuePayload;

/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
/*!
 * \brief Thread that process receive command (or response if run in Host mode).
 * \param param (not use)
 *
 */
Void mtSerialCmdDataLinkHandlingThread(serialQueuePayload_t sQueuePayload);

Void mtSerialCmdDataLinkCallbackRegister(pCmdHandlerCallback call_back);

/*!
 * \brief State machine for receiving Bluefin Serial Command
 * \param data data feed to state machine
 * \param buffer buffer that contain current's channel state machine
 * \param TotalDataLen pointer to current data len for state machine to monitor the process
 *
 * \return ROUTINE_RET_NO_CHANGE (state machine still need to process).
 * \return ROUTINE_RET_PUSH_DATA (state machine output a result).
 */
mtSerialRcvRoutineDecision_t mtSerialCmdRcvStateHandling(UInt8 bData,
        volatile serialQueuePayload_t *qBuff,
        UInt32 *pdwTotalDataLen);

Void mtSerialCmd_InterByteTimeOutHandling(volatile Void *pParam);

Void mtSerialQueueInit(serialQueuePayload_t *q);
mtErrorCode_t mtSerialUartEnqueue(serialQueuePayload_t *q, uint8_t value);
mtErrorCode_t mlsSerialUartDequeue(serialQueuePayload_t *q, uint8_t *dataOut);

#ifdef __cplusplus
}
#endif

#endif /* _MTSERIALCMDPARSER_H_ */
