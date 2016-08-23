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
static volatile uint32_t TimingDelay = 0;
static volatile uint32_t Time = 0;
static volatile uint32_t Time2;
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
void Delay(volatile uint32_t nTime)
{
	TimingDelay = nTime;
	while (TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	Time++;
	if (Time2 != 0x00)
	{
		Time2--;
	}
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

uint32_t DELAY_Time(void)
{
	return Time;
}

uint32_t DELAY_Time2(void)
{
	return Time2;
}

void DELAY_SetTime(volatile uint32_t time)
{
	Time = time;
}

void DELAY_SetTime2(volatile uint32_t time)
{
	Time2 = time;
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
#if (OPENCM3)
	tick_wait_ms(time_delay);
#else
	TimingDelay = time_delay;
	while (TimingDelay != 0);
#endif
}

/************************* End of File ****************************************/
