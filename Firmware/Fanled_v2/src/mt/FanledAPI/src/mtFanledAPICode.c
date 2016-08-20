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
#define CMD_CODE(buff)		(buff[0]) 	/*!< Get Command Code from buffer data*/
#define CTR_CODE(buff)		(buff[1])	/*!< Get Control Code from buffer data*/

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
Void mtFanledApiSetResponseCode(UInt8 *pInBuf, UInt8 *pOutBuf, UInt16 *pOutBufLen)
{
	*pOutBufLen = MT_DATA_RES_LEN_MIN;			/*Initialize length of message response*/
	pOutBuf[0] = CMD_CODE(pInBuf);				/*Response code*/
	pOutBuf[1] = CTR_CODE(pInBuf) + 1;			/*Response control code*/
	pOutBuf[2] = API_COMMAND_EXECUTE_SUCCESS;	/*Result code*/
}

/************************* End of File ****************************************/
