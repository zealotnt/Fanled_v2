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
#include "../inc/SystemConfig.h"
#include "../inc/mtAppDef.h"
#include "../tst/fanledTestApp.h"
#include "Porting/inc/mtTick.h"
#include "Porting/inc/mtUart.h"
#include "Porting/inc/mtSPI.h"
#include "Effects/inc/mtFanledDisplay.h"
#include "Bluetooth/inc/bluetooth.h"
#include "UartHandler/inc/mtProtocolDriver.h"
#include "RTC/inc/mtRtcDriver.h"

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define ISR_HALLSENSOR_PRIORITY			1		//(good value: 0)
#define ISR_TIMER_PRIORITY				2
#define ISR_RTC_PRIORITY				3

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
extern volatile uint32_t CCR1_Val;
extern volatile uint32_t CCR2_Val;

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
void initBootloader(void)
{
	mtSysTickInit();
	mtRCCInit();
	mtFanledSPIInit();
	bltInitModule(false);
	mtInterByteTimer_Init();
//	blankAllLed();
//	mtTimerFanledDisplayInit();
}

void initAll(void)
{
	mtSysTickInit();
	mtRCCInit();
	mtFanledSPIInit();
	mtTimerFanledDisplayInit();
	mtHallSensorInit();
	bltInitModule(false);
	stmInitRTC();
}

void mtSysTickInit(void)
{
#if (OPENCM3)
	tick_init();
#else
	SysTick_Config(SystemCoreClock / 1000);
#endif
}

void mtHallSensorInit(void)
{
	GPIO_InitTypeDef GPIOInitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIOInitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIOInitStructure);
	/* Config EXTI */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ISR_HALLSENSOR_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ISR_HALLSENSOR_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// For test color
void mtHallSensorDeinit(void)
{
	GPIO_InitTypeDef GPIOInitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIOInitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIOInitStructure);
	/* Config EXTI */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void mtRCCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA |
	                        RCC_APB2Periph_GPIOB |
	                        RCC_APB2Periph_GPIOC |
	                        RCC_APB2Periph_AFIO |
	                        RCC_APB2Periph_USART1 |
	                        RCC_APB2Periph_SPI1 , ENABLE);

	//SD Card enable
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)1);
}

void mtBluetoothUSARTInit(bool config)
{
	uart_init(config);
}

void mtBluetoothUSARTChangeBaud(uint32_t baudrate)
{
	uart_change_baud(baudrate);
}

/* Initialize HC05 key pin */
void mtHC05KeyPinInit(void)
{
	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIOInitStructure);
}

/* Timer2 used for controlling Fanled display timing */
void mtTimerFanledDisplayInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	RCC_PCLK1Config(RCC_HCLK_Div1);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 36000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* Output Compare Timing Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ISR_TIMER_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ISR_TIMER_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

void mtTimerFanledDisplayDisable()
{
	TIM_Cmd(TIM2, DISABLE);
}

/************************* End of File ****************************************/
