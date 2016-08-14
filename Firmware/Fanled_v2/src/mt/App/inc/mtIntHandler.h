/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.h
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

#ifndef MTINTHANDLER_H_
#define MTINTHANDLER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#if (OPENCM3)
#define mtHallSensorHandler							exti3_isr
#define mtSystickHandler							SysTick_Handler
#define mtFanledTimerHandler						TIM2_IRQHandler
#else
//#define mtFanledSpiTxCmplt							DMA1_Channel3_IRQHandler
//#define mtHallSensorHandler							EXTI3_IRQHandler
//#define mtSystickHandler							SysTick_Handler
//#define mtFanledTimerHandler						TIM2_IRQHandler
//#define mtBluetoothInterbyteTimerHandler			TIM3_IRQHandler
//#define mtBluetoothRcvHandler						USART1_IRQHandler
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* MTINTHANDLER_H_ */
