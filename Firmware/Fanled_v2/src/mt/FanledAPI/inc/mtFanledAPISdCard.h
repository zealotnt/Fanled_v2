/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPISdCard.h
**
** Module name       : FanledAPI
**
**
** Summary:
**
**= History ====================================================================
** @date 	Sep 5, 2016
** @author	zealot
** - Development
==============================================================================*/

#ifndef MTFANLEDAPISDCARD_H_
#define MTFANLEDAPISDCARD_H_

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


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
mtErrorCode_t mtFanledApiSdListFile(UInt8 *msgIn,
                                    UInt16 msgInLen,
                                    UInt8 *msgOut,
                                    UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiSdInspect(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiSdReadFile(UInt8 *msgIn,
                                    UInt16 msgInLen,
                                    UInt8 *msgOut,
                                    UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiSdDeleleFile(UInt8 *msgIn,
                                      UInt16 msgInLen,
                                      UInt8 *msgOut,
                                      UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiSdWriteFile(UInt8 *msgIn,
                                     UInt16 msgInLen,
                                     UInt8 *msgOut,
                                     UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiSdCheckFileMd5(UInt8 *msgIn,
                                        UInt16 msgInLen,
                                        UInt8 *msgOut,
                                        UInt16 *msgOutLen);

#ifdef __cplusplus
}
#endif

#endif /* MTFANLEDAPISDCARD_H_ */
