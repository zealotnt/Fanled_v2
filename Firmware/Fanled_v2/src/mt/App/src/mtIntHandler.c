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

#include "App/inc/mtAppDef.h"
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

extern Display_Type Fanled_Display;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
static uint16_t gtime_capture;
static uint8_t gTimer_Overload_Count = 0;

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
void mtSystickHandler(void)
{
	static volatile uint32_t Timing_Count = 0;
	static volatile uint32_t Scroll_Count = 0;

	mtDelayClockTick();
	TimingDelay_Decrement();

	if(Scroll_Count > Fanled_Display.scroll_times && Fanled_Display.enable_flag == SCROLL_ENABLE_DISPLAY)
	{
		Fanled_Display.move_flag = 1;
		Scroll_Count = 0;
	}
	else
		Scroll_Count++;

	Timing_Count++;
	if(Timing_Count > Fanled_Display.animation_change_speed)
	{
		if(Fanled_Display.animation == MAX_ANIMATION)
		{
			Fanled_Display.animation = 1;
		}
		else
		{
			Fanled_Display.animation++;
		}
		Fanled_Display.animation_change_speed += ANIMATION_CHANGE_SPEED;
	}

	if(Timing_Count > Fanled_Display.sharingan_count)
	{
		Fanled_Display.sharingan_flag = ENABLE;
		Fanled_Display.sharingan_count += SHARINGAN_CHANGE_SPEED;
	}

	if(Timing_Count > Fanled_Display.misc_count)
	{
		Fanled_Display.misc_flag = ENABLE;
		Fanled_Display.misc_count += MISC_CHANGE_SPEED;
	}
}

void mtFanledSpiTxCmplt(void)
{
#if STD_PERIPH_LIB
//	if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
//	{
		g_SPI_DMA_Flag = 1;
		DMA_ClearFlag(DMA1_FLAG_TC3);
		DMA_Cmd(DMA1_Channel3, DISABLE);   
//	}
#endif
}

void mtHallSensorHandler(void)
{
#if STD_PERIPH_LIB
	TIM_Cmd(TIM2, DISABLE);
	gtime_capture = TIM_GetCounter(TIM2);
	CCR1_Val = (gTimer_Overload_Count*65536 + gtime_capture) / FANLED_RESOLUTION;
	CCR2_Val = CCR1_Val/2;
	
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
#if STD_PERIPH_LIB
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		TIM2->SR = (uint16_t)~TIM_IT_CC1;
		if(gCurrent_point < FANLED_RESOLUTION)
		{
			//	capture = TIM_GetCapture1(TIM2);
			gtime_capture = TIM2->CCR1;
			
			//	TIM_SetCompare1(TIM2, capture + CCR1_Val);
			TIM2->CCR1 = gtime_capture + CCR1_Val;
			
			//	TIM_SetCompare2(TIM2, capture + CCR2_Val);
			TIM2->CCR2 = gtime_capture + CCR2_Val;
			
			//	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
			TIM2->DIER |= TIM_IT_CC2;
			gDisplayEnable = 1;
			gCurrent_point++;
		}
		else
		{
			//	TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2, DISABLE);
			TIM2->DIER &= (uint16_t)~(TIM_IT_CC1 | TIM_IT_CC2);
		}
	}
	//It's time to turn off leds
	else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		TIM2->SR = (uint16_t)~TIM_IT_CC2;
		
		//	TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
		TIM2->DIER &= (uint16_t)~TIM_IT_CC2;
		LED_BLANK();
	}
	// overflow timer
	else if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		// TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM2->SR = (uint16_t)~TIM_IT_Update;
		gTimer_Overload_Count++;
	}
#endif
}

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void EXTI0_IRQHandler(void)
{

}

void EXTI1_IRQHandler(void)
{

}


void EXTI15_10_IRQHandler(void)
{

}
/************************* End of File ****************************************/
