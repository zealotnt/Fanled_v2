/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledAPISdCard.c
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

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "mtInclude.h"

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
mtErrorCode_t mtFanledApiSdListFile(UInt8 *msgIn,
                                    UInt16 msgInLen,
                                    UInt8 *msgOut,
                                    UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdInspect(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdReadFile(UInt8 *msgIn,
                                   UInt16 msgInLen,
                                   UInt8 *msgOut,
                                   UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdDeleleFile(UInt8 *msgIn,
                                      UInt16 msgInLen,
                                      UInt8 *msgOut,
                                      UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}

mtErrorCode_t mtFanledApiSdWriteFile(UInt8 *msgIn,
                                     UInt16 msgInLen,
                                     UInt8 *msgOut,
                                     UInt16 *msgOutLen)
{
	return MT_SUCCESS;
}
/************************* End of File ****************************************/
