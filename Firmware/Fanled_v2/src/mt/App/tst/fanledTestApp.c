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
#include "RTC/inc/mtRtcDriver.h"
#include "App/tst/fanledTestApp.h"
#include "App/inc/SystemConfig.h"
#include "Porting/inc/mtSPI.h"
#include "Effects/inc/mtCalendar.h"
#include "Effects/inc/mtIncludeEffects.h"
#include "Bluetooth/inc/bluetooth.h"
#include "RTC/inc/mtRtc.h"
#include "UartHandler/inc/mtProtocolDriver.h"

#include <stdint.h>
#include <stdio.h>
#include <math.h>

//#include "ff.h"

#if (OPENCM3)
#include "libopencmsis/core_cm3.h"
#endif

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define SystemCoreClock 0
#define INTERVAL_BETWEEN_CHANGE_COLOR	30

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
extern uint8_t gDisplayEnable;
extern Display_Type Fanled_Display;
extern uint8_t ledPanel[36*4];
extern volatile uint8_t g_SPI_DMA_Flag;
extern uint16_t gCurrent_point;

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
void blankAllLed(void)
{
	updatePanel16b(0, true);
	mtSPIFanledSend((uint32_t)ledPanel, 36*4);
	mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
	LED_LATCH();
	LED_UNBLANK();
}

void initBootloader(void)
{
	// 1ms interval
	mtSysTickInit();
	mtRCCInit();
	mtFanledSPIInit();
	blankAllLed();
	bltInitModule(false);
	mtInterByteTimer_Init();
	//mtTimerFanledDisplayInit();
}

void initAll(void)
{
	mtSysTickInit();
	Fanled_Display.enable_flag = STATE_FREST;
	mtRCCInit();
	mtFanledSPIInit();
	mtTimerFanledDisplayInit();
	mtHallSensorInit();
	bltInitModule(false);
	stmInitRTC();
}

int mainTestHC05(void)
{
	initAll();
	blankAllLed();
	// True for first time config HC-05 module
	// False for instant usage
	bltInitModule(false);

	return 0;
}

int mainTestRTC(void)
{
	sytemdate_t oldDate;
	initAll();
	blankAllLed();
	/* Just use bluetooth UART to communicate to host */
	bltInitModule(false);
	while(1)
	{
		mtDelayMS(1000);
		if (memcmp(&oldDate, &sys_date, sizeof(sytemdate_t)))
		{
			printCurTime();
			oldDate = sys_date;
		}
	}

	return 0;
}


int mainTestColor(void)
{
	uint16_t i;
	uint16_t test_color = 0;
	initAll();
	mtHallSensorDeinit();
	mtTimerFanledDisplayDisable();
	while(1)
	{
		test_color = 0x00;
		for(i = 0; i < 0x1f; i++)
		{
			updatePanel16b(&test_color, true);
			mtSPIFanledSend((uint32_t)ledPanel, 36*4);
			mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
			LED_LATCH();
			LED_UNBLANK();
			test_color ++;			
		}
		
		test_color = 0x00;
		for(i = 0; i < 0x3f; i++)
		{
			updatePanel16b(&test_color, true);
			mtSPIFanledSend((uint32_t)ledPanel, 36*4);
			mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
			LED_LATCH();
			LED_UNBLANK();
			test_color += 0x20;			
		}

		test_color = 0x00;
		for(i = 0; i < 0x1f; i++)
		{
			updatePanel16b(&test_color, true);
			mtSPIFanledSend((uint32_t)ledPanel, 36*4);
			mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
			LED_LATCH();
			LED_UNBLANK();
			test_color += 0x800;			
		}
	}

	return 0;
}

int mainPicture(void)			//Test picture
{
	initAll();
	
#if defined(LOAD_IMAGE_TO_FLASH)
	load_image(rgb);
#endif
	
	Fanled_Display.enable_flag = STATE_FREST;
	Fanled_Display.rtc_flag = RTC_ENBALE;
	
	updatePanel16b(0, true);
	mtSPIFanledSend((uint32_t)ledPanel, 36*4);
	while(g_SPI_DMA_Flag == 0);
	LED_LATCH();
	LED_UNBLANK();
	while(1)
	{
		if(gDisplayEnable == 1)
		{
			if(gCurrent_point < FANLED_RESOLUTION)
			{
				LED_LATCH();
				LED_UNBLANK();
				updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
				mtSPIFanledSend((uint32_t)ledPanel, 36*4);
			}
			gDisplayEnable = 0;
		}
	}

	return 0;
}

int mainAppDeveloping(void)			// App developing
{
	initAll();
	Fanled_Display.animation = ANIMATION_1;
	Fanled_Display.animation_change_speed = ANIMATION_CHANGE_SPEED;
	
	Fanled_Display.enable_flag = STATE_FREST;
	Fanled_Display.rtc_flag = RTC_ENBALE;
	
	updatePanel16b(0, true);
	mtSPIFanledSend((uint32_t)ledPanel, 36*4);
	while(g_SPI_DMA_Flag == 0);
	LED_LATCH();
	LED_UNBLANK();
	while(1)
	{
		if(gDisplayEnable == 1)
		{
			if(gCurrent_point < FANLED_RESOLUTION)
			{
				LED_LATCH();
				LED_UNBLANK();
				updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
				mtSPIFanledSend((uint32_t)ledPanel, 36*4);
				
				//Some functions here
				switch(Fanled_Display.animation)
				{
					case (ANIMATION_1):
						if(Fanled_Display.animation_old != Fanled_Display.animation)
						{
							clear_screen_buffer(&Fanled_Display);
							Fanled_Display.animation_old = Fanled_Display.animation;
							Fanled_Display.enable_flag = STATE_FREST;
						}
						fanled_puts_scroll("Happy New Year!!", COLOR_RED_MAX, &Fanled_Display, 50);
						break;
					case (ANIMATION_2):
						if(Fanled_Display.animation_old != Fanled_Display.animation)
						{
							clear_screen_buffer(&Fanled_Display);
							Fanled_Display.animation_old = Fanled_Display.animation;
							//commend for quick flash program
							//load_image(rgb);
						}
						break;
					case (ANIMATION_3):
						if(Fanled_Display.animation_old != Fanled_Display.animation)
						{
							clear_screen_buffer(&Fanled_Display);
							DrawCircle(&Fanled_Display);
							Calendar_Populate(&Fanled_Display, &sys_date);
							Fanled_Display.animation_old = Fanled_Display.animation;							
						}
						if(Fanled_Display.rtc_flag == RTC_UPDATE)
						{
							Calendar_Populate(&Fanled_Display, &sys_date);
							Fanled_Display.rtc_flag = RTC_WAIT;
						}								
						break;
					case (ANIMATION_4):
						
						break;
					case (ANIMATION_5):
						
						break;
				}
				//End of functions
			}
			gDisplayEnable = 0;
		}//end of if(gDisplayEnable == 1)
	}//end of while(1)

	return 0;
}


int mainTestHSVCircle(void)
{
	initAll();
	
	Fanled_Display.animation = ANIMATION_1;
	Fanled_Display.animation_change_speed = ANIMATION_CHANGE_SPEED;
	
	Fanled_Display.enable_flag = STATE_FREST;
	Fanled_Display.rtc_flag = RTC_ENBALE;
	
	updatePanel16b(0, true);
	mtSPIFanledSend((uint32_t)ledPanel, 36*4);
	while(g_SPI_DMA_Flag == 0);
	LED_LATCH();
	LED_UNBLANK();
	ColorWheelPrepare(&Fanled_Display);
	while(1)
	{
		if(gDisplayEnable == 1)
		{
			if(gCurrent_point < FANLED_RESOLUTION)
			{
				LED_LATCH();
				LED_UNBLANK();
				updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
				mtSPIFanledSend((uint32_t)ledPanel, 36*4);
				//End of functions
			}
			gDisplayEnable = 0;
		}//end of if(gDisplayEnable == 1)
	}//end of while(1)

	return 0;
}


int mainTestNarutoEffect(void)
{
	initAll();
	
	Fanled_Display.animation = ANIMATION_1;
	Fanled_Display.animation_change_speed = ANIMATION_CHANGE_SPEED;
	Fanled_Display.sharingan_count = SHARINGAN_CHANGE_SPEED;
	Fanled_Display.misc_count = MISC_CHANGE_SPEED;
	
	Fanled_Display.enable_flag = STATE_FREST;
	Fanled_Display.rtc_flag = RTC_ENBALE;
	
	updatePanel16b(0, true);
	mtSPIFanledSend((uint32_t)ledPanel, 36*4);
	while(g_SPI_DMA_Flag == 0);
	LED_LATCH();
	LED_UNBLANK();
	while(1)
	{
		if(gDisplayEnable == 1)
		{
			if(gCurrent_point < FANLED_RESOLUTION)
			{
				LED_LATCH();
				LED_UNBLANK();
				updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
				mtSPIFanledSend((uint32_t)ledPanel, 36*4);
				EightLight();
				//End of functions
			}
			gDisplayEnable = 0;
		}//end of if(gDisplayEnable == 1)
	}//end of while(1)

	return 0;
}

/************************* End of File ****************************************/
