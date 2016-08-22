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

#ifndef _MTSERIALPORTING_H_
#define _MTSERIALPORTING_H_

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


/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
typedef void (*pTimerTimeoutHandler)(Void *param);

/*!
 *	\brief Baudrate support by SIRIUS
 */
typedef enum
{
	BR_9600			= 0x00,
	BR_19200		= 0x01,
	BR_115200		= 0x02,
	BR_230400		= 0x03,
	BR_460800		= 0x04,
	BR_14400		= 0x06,
	BR_28800		= 0x07,
	BR_38400		= 0x08,
	BR_57600		= 0x09,
	BR_76800		= 0x0A,
	BR_153600		= 0x0B,
	BR_307200		= 0x0C,
	BR_56000		= 0x0D,
	BR_128000		= 0x0E,
	BR_256000		= 0x0F,
} mtBaudrate_t;

/*!
 *	\brief Baudrate * Inter-byte timeout timer value lookup table
 */
typedef struct
{
	mtBaudrate_t 	baud;
	UInt32 			inteval;
} mtBaudTimerIntervalTable_t;

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/
#if (DEBUG)
#define DEBUG_SERIAL_PRINT(...)				printf(__VA_ARGS__)
#define DEBUG_SERIAL_PRINT_NOTIFY_BAD(...)	do {printf(KRED KBOLD __VA_ARGS__ ); printf(KRESET "\r\n");} while(0);
#define DEBUG_SERIAL_TIMING_PRINT(...)
#define DEBUG_SERIAL_ERROR_PRINT(...)		printf(__VA_ARGS__ "\r\n");
#define DEBUG_SERIAL_PRINT_NOTIFY_HANG(...)	do {printf(KRED KBOLD __VA_ARGS__); printf(KRESET "\r\n"); while(1);} while(0);
#else
#define DEBUG_SERIAL_PRINT(...)
#define DEBUG_SERIAL_PRINT_NOTIFY_BAD(...)
#define DEBUG_SERIAL_TIMING_PRINT(...)
#define DEBUG_SERIAL_ERROR_PRINT(...)
#define DEBUG_SERIAL_PRINT_NOTIFY_HANG(...)
#endif

#define mtMutexLock(a)
#define mtMutexUnlock(a)

#define mtGetCurrentTime(a)


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
mtErrorCode_t mtSerialPort_InterByteTimerInit(Void);

/**
 * \brief Disable timer
 *
 * \param timerId timerID to be disabled
 */
Void mtSerialPort_InterByteTimerDisable(Void);

/**
 * \brief Reload next interrupt value of timer
 *
 * \param timerId timerID to be reloaded
 * \param msTimeOut timeout in millisecond
 */
Void mtSerialPort_InterByteTimerReload(UInt32 dwMsTimeOut);

/**
 * \brief Register
 *
 * \param timer pointer to Command packet
 * \param fncHandler call back function to be registered
 */
Void mtSerialPort_InterByteTimerRegister(pTimerTimeoutHandler *timer,
        pTimerTimeoutHandler fncHandler);

/**
 * \brief Get inter-byte interval value base on current baudrate
 *
 * \return Value of interval in millisecond
 */
UInt32 mtSerialPort_InterByteTimerGetInverval();

/**
 * \brief Write specific len of byte to interface port
 * (Serial command in Host mode, or send Response in slave mode).
 *
 * \param data pointer to buffer data
 * \param lenToWrite len to write
 */
Void mtSerialPort_Write(UInt8 *pData, UInt16 wLenToWrite);

/**
 * \brief Flush RX buffer
 */
Void mtSerialPort_FlushRxData(Void);

/**
 * \brief Get current baudrate of communicating port
 */
mtBaudrate_t mtSerialPort_GetBaurate(Void);

/**
 * \brief After receive ACK from Host, check condition and change baudrate (if this operation is valid)
 * \param baudInfo contain information about condition about change baudrate procedure
 *
 * \return MT_SUCCESS
 * \return MT_ERROR
 */
mtErrorCode_t mtSerialPort_CheckAndChangeBaudrate(Void);

Int32 mtSerialPort_GetRxFifo(Void);

#ifdef __cplusplus
}
#endif

#endif /* _MTSERIALPORTING_H_ */
