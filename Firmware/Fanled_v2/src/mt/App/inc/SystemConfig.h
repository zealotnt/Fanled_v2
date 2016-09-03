/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : SystemConfig.h
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

#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "mtInclude.h"

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define FANLED_RCC_CLOCK_CMD			RCC_APB1PeriphClockCmd
#define FANLED_RCC_PERIPH				RCC_APB1Periph_TIM2
#define FANLED_TIMER_NVIC_IRQ			TIM2_IRQn
#define FANLED_TIMER_ID					TIM2

#define FANLED_HC05_KEY_PORT			GPIOA
#define FANLED_HC05_KEY_PIN				GPIO_Pin_11

#define FANLED_HALLSENSOR_PORT			GPIOB
#define FANLED_HALLSENSOR_PIN			GPIO_Pin_3
#define FANLED_HALLSENSOR_NVIC_IRQ		EXTI3_IRQn
/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
void blankAllLed(void);

void initBootloader();
void initAll();

void mtSdCardInit(void);
void mtSysTickInit();
void mtRCCInit();
void mtDelayMS(uint32_t time_delay);
void mtHallSensorInit();
void mtHallSensorDeinit();
void mtTimerFanledDisplayInit();
void mtTimerFanledDisplayDisable();

void mtHC05KeyPinInit();
void mtBluetoothUSARTInit(bool config);
void mtBluetoothUSARTChangeBaud(uint32_t baudrate);

void mtNvicDisableAll();

#endif /* _SYSTEM_CONFIG_H_ */
