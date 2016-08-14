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
#include "App/inc/SystemConfig.h"
#include "Porting/inc/mtTick.h"

#if (OPENCM3)
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>
#elif (STD_PERIPH_LIB)
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#endif

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define ISR_HALLSENSOR_PRIORITY			1		//(good value: 0)
#define ISR_UART_RX_PRIORITY			0
#define ISR_TIMER_PRIORITY				2
#define ISR_RTC_PRIORITY				3
#define BOARD_UART				USART1
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
void mtSysTickInit(void)
{
#if (OPENCM3)
	tick_init();
#endif
}

void mtHallSensorInit(void)
{
#if (STD_PERIPH_LIB)
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
#elif (OPENCM3)
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO3);
	exti_set_trigger(EXTI3, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI3);

#endif
}

// For test color
void mtHallSensorDeinit(void)
{
#if (STD_PERIPH_LIB)
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
#elif (OPENCM3)

#endif
}

void mtRCCInit(void)
{
#if (STD_PERIPH_LIB)
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | \
												RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1 | RCC_APB2Periph_SPI1 ,ENABLE);

	//SD Card enable
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);		
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)1);
#elif (OPENCM3)
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOD);
	//rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_I2C2);

#endif
}

void mtBluetoothUSARTInit(bool config)
{
#if STD_PERIPH_LIB
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USARTInitStructure;
	GPIO_InitTypeDef GPIOInitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_10; //<! PA10-Rx
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_9; //<! PA9-TX
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	if(config == true)
	{
		USARTInitStructure.USART_BaudRate = 38400;
	}
	else
	{
		USARTInitStructure.USART_BaudRate = 115200;
	}
	USARTInitStructure.USART_WordLength= USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity =  USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(BOARD_UART,&USARTInitStructure);
	USART_ITConfig(BOARD_UART, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ISR_UART_RX_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ISR_UART_RX_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(BOARD_UART,ENABLE);
#elif (OPENCM3)
	gpio_set_mode(	GPIOA,
					GPIO_MODE_OUTPUT_50_MHZ,
					GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
					GPIO_USART1_TX);

	/* Setup UART parameters. */
	// usart_set_baudrate(USART1, 38400);
	/* TODO usart_set_baudrate() doesn't support 24MHz clock (yet). */
	/* This is the equivalent: */
	USART_BRR(USART1) = (uint16_t)((24000000 << 4) / (38400 * 16));

	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
#endif
}

void mtBluetoothUSARTChangeBaud(uint32_t baudrate)
{
#if STD_PERIPH_LIB
	USART_InitTypeDef USARTInitStructure;
	
	USART_Cmd(BOARD_UART,DISABLE); 
	USARTInitStructure.USART_BaudRate = baudrate;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity =  USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(BOARD_UART, &USARTInitStructure);	
	USART_Cmd(BOARD_UART, ENABLE);
#elif (OPENCM3)
	USART_BRR(USART1) = (uint16_t)((24000000 << 4) / (baudrate * 16));

#endif
}

/* Delay Millisecond using Systick */
void mtDelayMS(volatile uint32_t time_delay)
{
#if (OPENCM3)
	tick_wait_ms(time_delay);
#endif
}

/* Initialize HC05 key pin */
void mtHC05KeyPinInit(void)
{
#if (STD_PERIPH_LIB)
	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIOInitStructure);
#elif (OPENCM3)
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11);

#endif
}

/* Timer2 used for controlling Fanled display timing */
void mtTimerFanledDisplayInit(void)
{
#if (STD_PERIPH_LIB)
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
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
#elif (OPENCM3)

#endif
}

/************************* End of File ****************************************/
