/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtWdt.c
**
** Module name       : Porting
**
**
** Summary:
**
**= History ====================================================================
** @date 	Aug 24, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include <stm32f10x_wwdg.h>
#include <stm32f10x_iwdg.h>
#include <stm32f10x_rcc.h>
#include <misc.h>

#include "mtInclude.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/
#define WDT_RELOAD_VALUE			0x7F
#define WDT_COUNT_MAX_VALUE			50

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
volatile UInt8 gWdtCount = 0;

/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
void mtWdtInit()
{
	/* Enable clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	/* Setup watchdog */
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_Enable(WDT_RELOAD_VALUE);
	WWDG_ClearFlag();
	WWDG_EnableIT();

	/* WWDG interrupt */
	/* NVIC initialization */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	/* In 2, sub priority 3 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void mtWdtFeed()
{
	gWdtCount = 0;
	WWDG_Enable(WDT_RELOAD_VALUE);
}

Bool mtWdtCheckTriggered()
{
	Bool ret;
	ret = (Bool)RCC_GetFlagStatus(RCC_FLAG_WWDGRST);
	RCC_ClearFlag();
	return ret;
}

/**
 * The watchdog interrupt service routine
 */
void WWDG_IRQHandler(void)
{
#if (FANLED_APP)
	if (gWdtCount < WDT_COUNT_MAX_VALUE)
	{
		WWDG_Enable(WDT_RELOAD_VALUE);
	    /*Remove pre wakeup interrupt flag*/
	    WWDG_ClearFlag();
	    gWdtCount ++;
	}
#endif
}

/************************* End of File ****************************************/
