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
#include "mtInclude.h"
#include "mtVersion.h"
#include "../inc/mtFanledAPICode.h"

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
mtErrorCode_t mtFanledApiGetFirmwareVersion(UInt8 *msgIn,
											UInt16 msgInLen,
											UInt8 *msgOut,
											UInt16 *msgOutLen)
{
	mtErrorCode_t retVal = MT_SUCCESS;
	UInt32 dwVersion;

	dwVersion = FIRMWARE_VERSION_MAJOR*10000 + FIRMWARE_VERSION_MINOR*100 + FIRMWARE_REVISION;

	msgOut[6] = 3;		/* Firmware ID = 3 --> RF Processor Firmware */
	msgOut[3] = (dwVersion & 0xFF);
	msgOut[4] = (dwVersion >> 8) & 0xFF;
	msgOut[5] = (dwVersion >> 16) & 0xFF;
	*msgOutLen = 7;

	return retVal;
}

mtErrorCode_t mtFanledApiProtocolTest(UInt8 *msgIn,
								   UInt16 msgInLen,
								   UInt8 *msgOut,
								   UInt16 *msgOutLen)
{
	UInt32 i;

	*msgOutLen = (UInt16)(*(UInt16 *)(&msgIn[6]));

	/* Copy count value */
	memcpy(&msgOut[2], &msgIn[2], 4);

	for (i = 6; i < *msgOutLen; i++)
	{
		msgOut[i] = i;
	}

	return MT_SUCCESS;
}
/************************* End of File ****************************************/
