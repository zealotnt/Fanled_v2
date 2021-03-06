/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtDisplayHandler.c
**
** Module name       : Effects
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "stm32f10x_it.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"

#include "mtInclude.h"
#include "App/inc/mtAppDef.h"
#include "Effects/inc/mtFanledDisplay.h"
#include "Utility/inc/mtDelay.h"
#include "Porting/inc/mtSPI.h"
#include "App/inc/SystemConfig.h"
#include "Effects/inc/mtIncludeEffects.h"

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
uint16_t gCurrent_point = 0;
volatile uint32_t CCR1_Val = 30000;
volatile uint32_t CCR2_Val = 30000;
volatile uint8_t g_SPI_DMA_Flag = 0;
volatile uint8_t gDisplayEnable;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
#if (FANLED_APP)
static uint16_t gtime_capture;
static uint8_t gTimer_Overload_Count = 0;
#endif

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
void mtFanledSpiTxCmplt(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
	{
		g_SPI_DMA_Flag = 1;
		DMA_ClearFlag(DMA1_FLAG_TC3);
		DMA_Cmd(DMA1_Channel3, DISABLE);
	}
}

void mtHallSensorHandler(void)
{
#if (FANLED_APP)
	TIM_Cmd(TIM2, DISABLE);
	gtime_capture = TIM_GetCounter(TIM2);
	CCR1_Val = (gTimer_Overload_Count * 65536 + gtime_capture) / FANLED_RESOLUTION;
	CCR2_Val = CCR1_Val / 2;

	gCurrent_point = 0;
	gTimer_Overload_Count = 0;
	gDisplayEnable = 1;

	// reset the timer
	TIM_SetCounter(TIM2, 0);
	TIM_SetCompare1(TIM2, CCR1_Val);
	TIM_SetCompare2(TIM2, CCR2_Val);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	EXTI_ClearITPendingBit(EXTI_Line3);
#endif
}

void mtFanledTimerHandler(void)
{
#if (FANLED_APP)
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		// TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		TIM2->SR = (uint16_t)~TIM_IT_CC1;
		if (gCurrent_point < FANLED_RESOLUTION)
		{
			// capture = TIM_GetCapture1(TIM2);
			gtime_capture = TIM2->CCR1;

			// TIM_SetCompare1(TIM2, capture + CCR1_Val);
			TIM2->CCR1 = gtime_capture + CCR1_Val;

			// TIM_SetCompare2(TIM2, capture + CCR2_Val);
			TIM2->CCR2 = gtime_capture + CCR2_Val;

			// TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
			TIM2->DIER |= TIM_IT_CC2;
			gDisplayEnable = 1;
			gCurrent_point++;
		}
		else
		{
			// TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2, DISABLE);
			TIM2->DIER &= (uint16_t)~(TIM_IT_CC1 | TIM_IT_CC2);
		}
	}
	// It's time to turn off leds
	else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		// TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		TIM2->SR = (uint16_t)~TIM_IT_CC2;

		// TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
		TIM2->DIER &= (uint16_t)~TIM_IT_CC2;
		LED_BLANK();
	}
	// overflow timer
	else if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		// TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM2->SR = (uint16_t)~TIM_IT_Update;
		gTimer_Overload_Count++;
	}
#endif
}
