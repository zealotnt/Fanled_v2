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
#include "App/inc/mtAppDef.h"
#include "../inc/mtCalendar.h"

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
void DrawCircle(Display_Type *FanledDisplay)
{
	uint8_t i;
	for (i = 0; i < 60; i++)
	{
		FanledDisplay->dis[i * 4][0] = COLOR_RED_MAX;
	}
	for (i = 0; i < 12; i++)
	{
		FanledDisplay->dis[i * 20][1] = COLOR_GREEN_MAX;
	}
	for (i = 0; i < 12; i++)
	{
		FanledDisplay->dis[i * 20][2] = COLOR_BLUE_MAX;
	}

	FanledDisplay->dis[0][0] = COLOR_VIOLET_MAX;
	FanledDisplay->dis[0][1] = COLOR_VIOLET_MAX;
	FanledDisplay->dis[1][1] = COLOR_VIOLET_MAX;
	FanledDisplay->dis[FANLED_RESOLUTION - 1][1] = COLOR_VIOLET_MAX;
	FanledDisplay->dis[0][2] = COLOR_VIOLET_MAX;
}

void Calendar_Populate(Display_Type *FanledDisplay, sytemdate_t *sysDate)
{
	uint16_t i;
	uint8_t j;

	/* Circle data from 		0-->2 */
	/* Circle time data from 	3-->5 */
	/* Date data from 			6-->14 */
	/* Stick time data from 	15-->31 */

	/* Erase fanled data */
	for (i = 0; i < FANLED_RESOLUTION; i++)
	{
		for (j = 15; j < 31; j++)
		{
			FanledDisplay->dis[i][j] = 0x00;
		}
		for (j = 3; j < 5; j++)
		{
			FanledDisplay->dis[i][j] = 0x00;
		}
	}

	/* Draw Second stick and circle */
	for (i = 15; i < 31; i++)
	{
		FanledDisplay->dis[sysDate->ss * 4][i] = CALENDAR_SECOND_STICK_COLOR;
	}
	for (i = 0; i <= sysDate->ss * 4; i++)
	{
		FanledDisplay->dis[i][3] = CALENDAR_SECOND_STICK_COLOR;
	}

	/* Draw Minute stick and circle */
	for (i = 18; i < 31; i++)
	{
		FanledDisplay->dis[sysDate->mm * 4][i] = CALENDAR_MINUTE_STICK_COLOR;
	}
	for (i = 0; i <= sysDate->mm * 4; i++)
	{
		FanledDisplay->dis[i][4] = CALENDAR_MINUTE_STICK_COLOR;
	}

	/* Draw hour stick and circle */
	for (i = 20; i < 31; i++)
	{
		FanledDisplay->dis[(sysDate->hh % 12) * 20][i] = CALENDAR_HOUR_STICK_COLOR;
	}
	for (i = 0; i <= (sysDate->hh % 12) * 20; i++)
	{
		FanledDisplay->dis[i][5] = CALENDAR_HOUR_STICK_COLOR;
	}
}


/************************* End of File ****************************************/
