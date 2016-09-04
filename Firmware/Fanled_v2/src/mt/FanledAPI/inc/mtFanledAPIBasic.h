/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPIBasic.h
**
** Module name       : FanledAPI
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef MTFANLEDAPIBASIC_H_
#define MTFANLEDAPIBASIC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/


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
void mtDbgVarRegister(UInt32 *VarAdd);

mtErrorCode_t mtFanledApiGetFirmwareVersion(UInt8 *msgIn,
        UInt16 msgInLen,
        UInt8 *msgOut,
        UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiProtocolTest(UInt8 *msgIn,
                                      UInt16 msgInLen,
                                      UInt8 *msgOut,
                                      UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiGetSetRTC(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiHardFault(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen);

mtErrorCode_t mtFanledApiGetDbgVar(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen);



#ifdef __cplusplus
}
#endif

#endif /* MTFANLEDAPIBASIC_H_ */
