/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtDelay.c
**
** Module name       : Utility
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
#include <stdint.h>
#include "../inc/mtDelay.h"

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
static volatile uint32_t gtickDelay;

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
void DelayRaw(volatile uint32_t nTime)
{
	while(nTime) nTime--;
}

void mtDelayClockTick()
{
	if (gtickDelay != 0)
	{
		gtickDelay--;
	}
}

/* Delay Millisecond using Systick */
void mtDelayMS(volatile uint32_t time_delay)
{
	gtickDelay = time_delay;
	while (gtickDelay != 0);
}

/************************* End of File ****************************************/
