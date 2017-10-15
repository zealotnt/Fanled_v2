/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtIntHandler.c
**
** Module name       : App
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

#include "../inc/mtIntHandler.h"
#include "../inc/mtAppDef.h"
#include "../inc/SystemConfig.h"
#include "Utility/inc/mtDelay.h"
#include "Porting/inc/mtSPI.h"
#include "Effects/inc/mtIncludeEffects.h"
#include "Effects/inc/mtFanledDisplay.h"

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
#if (FANLED_APP)
extern Display_Type Fanled_Display;
#endif
extern void disk_timerproc (void);

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
void mtSystickHandler(void)
{
#if (FANLED_APP)
	static volatile uint32_t Timing_Count = 0;
	static volatile uint32_t Scroll_Count = 0;

	if (Scroll_Count > Fanled_Display.scroll_times && Fanled_Display.enable_flag == SCROLL_ENABLE_DISPLAY)
	{
		Fanled_Display.move_flag = 1;
		Scroll_Count = 0;
	}
	else
	{
		Scroll_Count++;
	}

	Timing_Count++;
	if (Timing_Count > Fanled_Display.animation_change_speed)
	{
		if (Fanled_Display.animation == MAX_ANIMATION)
		{
			Fanled_Display.animation = 1;
		}
		else
		{
			Fanled_Display.animation++;
		}
		Fanled_Display.animation_change_speed += ANIMATION_CHANGE_SPEED;
	}

	if (Timing_Count > Fanled_Display.sharingan_count)
	{
		Fanled_Display.sharingan_flag = STATE_ENABLE;
		Fanled_Display.sharingan_count += SHARINGAN_CHANGE_SPEED;
	}

	if (Timing_Count > Fanled_Display.misc_count)
	{
		Fanled_Display.misc_flag = STATE_ENABLE;
		Fanled_Display.misc_count += MISC_CHANGE_SPEED;
	}
#endif
	mtDelayClockTick();
	disk_timerproc();
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
