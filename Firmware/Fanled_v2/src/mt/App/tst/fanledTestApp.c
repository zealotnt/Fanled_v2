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
#include "App/inc/main.h"
#include "App/tst/fanledTestApp.h"
#include "App/inc/SystemConfig.h"
#include "Effects/inc/mtCalendar.h"
#include "Effects/inc/mtIncludeEffects.h"
#include "Effects/inc/mtFanledDisplay.h"
#include "Effects/inc/image_data.h"
#include "Bluetooth/inc/bluetooth.h"
#include "Porting/inc/mtWdt.h"
#include "FanledAPI/inc/mtFanledAPIBasic.h"

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
extern uint16_t gCurrent_point;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
void test_color_cb(void *param);

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
void test_curve_cb(void *param)
{
#define CURVE_START_POS			100
#define CURVE_END_POS			120
#define CURVE_ENABLE_POS		10
#define CURVE_COLOR				COLOR_ORANGE_MAX
	UInt16 buff_color[32] = {COLOR_BLACK};

	if (gDisplayEnable == 1)
	{
		if (gCurrent_point < FANLED_RESOLUTION)
		{
			if (gCurrent_point >= CURVE_START_POS &&
			        gCurrent_point <= CURVE_END_POS)
			{
				memset(buff_color, CURVE_COLOR, sizeof(buff_color));
				LED_LATCH();
				LED_UNBLANK();
				updatePanel16b(buff_color, true);
				mtFanledSendLineBuffer();
			}
			else
			{
				memset(buff_color, COLOR_BLACK, sizeof(buff_color));
				LED_LATCH();
				LED_UNBLANK();
				updatePanel16b(buff_color, true);
				mtFanledSendLineBuffer();
			}
		}
		gDisplayEnable = 0;
	}
}

void test_color_cb(void *param)
{
	uint16_t i;
	uint16_t test_color = 0;

	test_color = 0x00;
	for (i = 0; i < 0x1f; i++)
	{
		updatePanel16b(&test_color, true);
		mtFanledSendLineBuffer();
		mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
		LED_LATCH();
		LED_UNBLANK();
		test_color ++;
	}

	test_color = 0x00;
	for (i = 0; i < 0x3f; i++)
	{
		updatePanel16b(&test_color, true);
		mtFanledSendLineBuffer();
		mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
		LED_LATCH();
		LED_UNBLANK();
		test_color += 0x20;
	}

	test_color = 0x00;
	for (i = 0; i < 0x1f; i++)
	{
		updatePanel16b(&test_color, true);
		mtFanledSendLineBuffer();
		mtDelayMS(INTERVAL_BETWEEN_CHANGE_COLOR);
		LED_LATCH();
		LED_UNBLANK();
		test_color += 0x800;
	}
}

void test_display_frame_buffer(void *param)
{
	if (gDisplayEnable == 1)
	{
		if (gCurrent_point < FANLED_RESOLUTION)
		{
			LED_LATCH();
			LED_UNBLANK();
			updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
			mtFanledSendLineBuffer();
		}
		gDisplayEnable = 0;
	}
}

void test_eightlight(void *param)
{
	if (gDisplayEnable == 1)
	{
		if (gCurrent_point < FANLED_RESOLUTION)
		{
			LED_LATCH();
			LED_UNBLANK();
			updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
			mtFanledSendLineBuffer();
			EightLight();
			//End of functions
		}
		gDisplayEnable = 0;
	}
}

void test_sharingan(void *param)
{
	if (gDisplayEnable == 1)
	{
		if (gCurrent_point < FANLED_RESOLUTION)
		{
			LED_LATCH();
			LED_UNBLANK();
			updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
			mtFanledSendLineBuffer();
			Sharingan_effect();
			//End of functions
		}
		gDisplayEnable = 0;
	}
}

void app_developing(void *param)
{
	if (gDisplayEnable == 1)
	{
		if (gCurrent_point < FANLED_RESOLUTION)
		{
			LED_LATCH();
			LED_UNBLANK();
			updatePanel16b(Fanled_Display.dis[gCurrent_point], false);
			mtFanledSendLineBuffer();

			//Some functions here
			switch (Fanled_Display.animationCur)
			{
				case (ANIMATION_1):
					if (Fanled_Display.animationOld != Fanled_Display.animationCur)
					{
						clear_screen_buffer(&Fanled_Display);
						Fanled_Display.animationOld = Fanled_Display.animationCur;
						Fanled_Display.enable_flag = STATE_FREST;
					}
					fanled_puts_scroll("Happy New Year!!", COLOR_RED_MAX, &Fanled_Display, 50);
					break;
				case (ANIMATION_2):
					if (Fanled_Display.animationOld != Fanled_Display.animationCur)
					{
						clear_screen_buffer(&Fanled_Display);
						DrawCircle(&Fanled_Display);
						Calendar_Populate(&Fanled_Display, &sys_date);
						Fanled_Display.animationOld = Fanled_Display.animationCur;
					}
					if (sys_date.isUpdated == true)
					{
						sys_date.isUpdated = false;
						Calendar_Populate(&Fanled_Display, &sys_date);
					}
					break;
				case (ANIMATION_3):
//					if(Fanled_Display.animationOld != Fanled_Display.animation)
//					{
//						clear_screen_buffer(&Fanled_Display);
//						Fanled_Display.animation_old = Fanled_Display.animation;
//						load_image(rgb);
//					}
					break;

				case (ANIMATION_4):

					break;
				case (ANIMATION_5):

					break;

				default :
					break;
			}
			//End of functions
		}
		gDisplayEnable = 0;
	}//end of if(gDisplayEnable == 1)
}

/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
int mainTestHC05(void)
{
	initAll();
	blankAllLed();
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
	while (1)
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

void FanledTestCurve()
{
	mainCallBackRegister(test_curve_cb);
}

void FanledTestColor(void)
{
	mtHallSensorDeinit();
	mtTimerFanledDisplayDisable();
	mainCallBackRegister(test_color_cb);
}

void FanledTestPicture(void)
{
#if defined(LOAD_IMAGE_TO_FLASH)
	load_image(img_rgb);
#endif
	blankAllLed();
	mainCallBackRegister(test_display_frame_buffer);
}

void FanledAppDeveloping(void)
{
	Fanled_Display.animationCur = ANIMATION_1;
	Fanled_Display.animationChangeSpeed = ANIMATION_CHANGE_SPEED;
	Fanled_Display.enable_flag = STATE_FREST;

	blankAllLed();
	mainCallBackRegister(app_developing);
}

void FanledTestHSVCircle(void)
{
	blankAllLed();
	ColorWheelPrepare(&Fanled_Display);
	mainCallBackRegister(test_display_frame_buffer);
}

void FanledTestSharinganEffect(void)
{
	Fanled_Display.misc_count = MISC_CHANGE_SPEED;

	blankAllLed();
	mainCallBackRegister(test_sharingan);
}

void FanledTestEightLight(void)
{
	Fanled_Display.sharingan_count = SHARINGAN_CHANGE_SPEED;
	Fanled_Display.misc_count = MISC_CHANGE_SPEED;

	blankAllLed();
	mainCallBackRegister(test_eightlight);
}

/************************* End of File ****************************************/
