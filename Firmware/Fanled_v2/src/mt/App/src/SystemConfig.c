/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : SystemConfig.c
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
#include "../inc/SystemConfig.h"
#include "../inc/mtAppDef.h"
#include "../tst/fanledTestApp.h"
#include "Porting/inc/mtTick.h"
#include "Porting/inc/mtUart.h"
#include "Porting/inc/mtSPI.h"
#include "Porting/inc/mtWdt.h"
#include "Effects/inc/mtFanledDisplay.h"
#include "Bluetooth/inc/bluetooth.h"
#include "UartHandler/inc/mtProtocolDriver.h"
#include "RTC/inc/mtRtcDriver.h"
#include "Bootloader/inc/driverBootloader.h"
#include "ff.h"

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

/* SD Card specific Variables */
FATFS gFatFs;
DIR mydir;
FILINFO myfno;
SdManager_t sdFileInfo;

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
void mtSdCardInit(void)
{
	FRESULT res = FR_DISK_ERR;
	uint8_t count = 0;

	while ((res != FR_OK) && (count < 10))
	{
		res = f_mount(0, &gFatFs);
		count++;
	}

	if (f_mount(0, &gFatFs) == FR_OK)
	{
		do
		{
			res = f_opendir(&mydir, "0:\\");
			mtDelayMS(1);
			count ++;
		}
		while (res != FR_OK && (count < 10));

		/* Err, return */
		if (res != FR_OK || count >= 10)
		{
			return;
		}

		do
		{
			res = f_readdir(&mydir, &myfno);
			if (myfno.fname[0])
			{
				sdFileInfo.NumOfItem++;
			}
		}
		while (myfno.fname[0]);
	}

	sdFileInfo.ChoiceNow = 0;
}

void initBootloader(void)
{
	mtRCCInit();
	mtSysTickInit();
	core_enable_isr();
	mtFanledSPIInit();
	bltInitModule(false);
	mtInterByteTimer_Init();
	mtBootloaderInitFlash();
	blankAllLed();
}

void initAll(void)
{
	initBootloader();
	mtHallSensorInit();
	stmInitRTC();
	mtTimerFanledDisplayInit();
	mtSdCardInit();
	mtWdtInit();
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

	/* Config GPIO */
	GPIOInitStructure.GPIO_Pin = FANLED_HALLSENSOR_PIN;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(FANLED_HALLSENSOR_PORT, &GPIOInitStructure);

	/* Config EXTI */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Config NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = FANLED_HALLSENSOR_NVIC_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ISR_HALLSENSOR_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ISR_HALLSENSOR_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/* For test color */
void mtHallSensorDeinit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Config EXTI */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Config NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void mtRCCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_PCLK1Config(RCC_HCLK_Div1);

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC |
	                        RCC_APB2Periph_AFIO, ENABLE);

	//SD Card enable
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)1);
#if (FANLED_APP)
	/* Set system control register SCR->VTOR  */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_BOOTLOADER_SIZE);
#elif (FANLED_BOOTLOADER)
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0);
#endif
}

void mtBluetoothUSARTInit(bool config)
{
	uart_cmd_init(config);
}

void mtBluetoothUSARTChangeBaud(uint32_t baudrate)
{
	uart_cmd_change_baud(baudrate);
}

/* Initialize HC05 key pin */
void mtHC05KeyPinInit(void)
{
	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FANLED_HC05_KEY_PORT, &GPIOInitStructure);
}

/* Timer2 used for controlling Fanled display timing */
void mtTimerFanledDisplayInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	FANLED_RCC_CLOCK_CMD(FANLED_RCC_PERIPH , ENABLE);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 36000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(FANLED_TIMER_ID, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	TIM_PrescalerConfig(FANLED_TIMER_ID, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(FANLED_TIMER_ID, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(FANLED_TIMER_ID, TIM_OCPreload_Disable);

	/* Output Compare Timing Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	TIM_OC2Init(FANLED_TIMER_ID, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(FANLED_TIMER_ID, TIM_OCPreload_Disable);

	/* TIM IT enable */
	TIM_ITConfig(FANLED_TIMER_ID, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_Update, ENABLE);

	/* FANLED_TIMER_ID enable counter */
	TIM_Cmd(FANLED_TIMER_ID, ENABLE);

	/* Enable the FANLED_TIMER_ID global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = FANLED_TIMER_NVIC_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ISR_TIMER_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = ISR_TIMER_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

void mtTimerFanledDisplayDisable()
{
	TIM_Cmd(FANLED_TIMER_ID, DISABLE);
}

void mtNvicDisableAll()
{
	NVIC_DisableIRQ(FANLED_TIMER_NVIC_IRQ);
	NVIC_DisableIRQ(FANLED_HALLSENSOR_NVIC_IRQ);
	NVIC_DisableIRQ(SPI_DISPLAY_Tx_DMA_IRQ);
	NVIC_DisableIRQ(FANLED_UART_IRQN);
	NVIC_DisableIRQ(FANLED_INTER_BYTE_TIMER_IRQN);
	mtWdtDisable();
}

/************************* End of File ****************************************/
