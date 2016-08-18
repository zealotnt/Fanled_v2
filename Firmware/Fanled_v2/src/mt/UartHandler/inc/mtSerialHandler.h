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

#ifndef _MTREADERHANDLER_H_
#define _MTREADERHANDLER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
/********** Include section ***************************************************/
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
typedef mtErrorCode_t (*fnSerialCmdHandler) (UInt8 *msgIn, UInt16 msgInLen, UInt8 *msgOut, UInt16 *msgOutLen);

#pragma pack(1)
/*!
 *	\brief basic element of ReaderHandler Table Function
 */
typedef struct
{
	UInt8 CmdCode;
	UInt8 ControlCode;
	fnSerialCmdHandler fnHandler;
} mtSerialAppAPIHandler;
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
/**
 * \brief Parse Command code, control code and Process receive Command
 *
 * \param msgIn pointer to Command packet
 * \param msgInLen len of Command packet
 * \param msgOut pointer to Response packet
 * \param msgOutLen len of Response packet
 *
 * \return MT_SUCCESS
 */
mtErrorCode_t mtSerialProcessCmdPacket(UInt8 *msgIn,
										UInt16 msgInLen,
										UInt8 *msgOut,
										UInt16 *msgOutLen);

#ifdef __cplusplus
}
#endif

#endif /* _MTREADERHANDLER_H_ */
