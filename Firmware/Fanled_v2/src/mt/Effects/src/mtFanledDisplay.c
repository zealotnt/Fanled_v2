/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledDisplay.c
**
** Module name       : Effects
**
**
** Summary:
**
**= History ====================================================================
** @date 	Aug 18, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include <stm32f10x_gpio.h>
#include <stdbool.h>
#include <stdint.h>

#include "../inc/mtFanledDisplay.h"
#include "../inc/mtDisplayText.h"
#include "Utility/inc/mtDelay.h"
#include "Porting/inc/mtSPI.h"

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
Display_Type Fanled_Display;
#endif
uint8_t ledPanel[36 * 4];

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
void LED_BLANK()
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)1);
}

void LED_UNBLANK()
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)0);
}

void LED_LATCH()
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)1);GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)0);
}


void mtFanledSendLineBuffer()
{
	mtFanledSendDmaBuffer((uint32_t)ledPanel, 36 * 4);
}

void blankAllLed(void)
{
	updatePanel16b(0, true);
	mtFanledSendLineBuffer();
	mtDelayMS(30);
	LED_LATCH();
	LED_UNBLANK();
}

/************************* End of File ****************************************/
