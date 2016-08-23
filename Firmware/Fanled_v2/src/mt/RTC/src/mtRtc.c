/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtRtc.c
**
** Module name       : RTC
**
**
** Summary:
**
**= History ====================================================================
** @date 	Feb 23, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include <stm32f10x_rtc.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "mtInclude.h"
#include "../inc/mtRtc.h"
#include "../inc/mtRtcDriver.h"
#include "std_type.h"


/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/
sytemdate_t sys_date;



/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
static char monthDecStrTable[12][4] =
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

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
/* 	Get string of month base on decimal value
	month value since January (0 --> 11) */
char *getMonthStr(uint8_t month)
{
	if (month >= 11)
	{
		return NULL;
	}
	return monthDecStrTable[month];
}

void printCurTime(void)
{
	DEBUG_INFO("Current time: %2dh, %2dm, %2ds, %d %s %d\r\n",
	       sys_date.hh, sys_date.mm, sys_date.ss,
	       sys_date.day, getMonthStr(sys_date.month), sys_date.year);
}

/*******************************************************************************
* Function Name  : mtRtcGetSystemTime
* Description    : Get the current RTC time value
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
sytemdate_t *mtRtcGetSystemTime(void)
{
	time_t acttime;
	struct tm* pLocalTime;

	acttime = RTC_GetCounter();
	pLocalTime = localtime(&acttime);

	sys_date.hh     = (uint8_t)pLocalTime->tm_hour;            /* hours since midnight (0,23)      */
	sys_date.mm     = (uint8_t)pLocalTime->tm_min;             /* minutes after the hour (0,59)    */
	sys_date.ss     = (uint8_t)pLocalTime->tm_sec;             /* seconds after the minute (0,61)  */
	sys_date.year   = (uint16_t)pLocalTime->tm_year + 1900;    /* years since 1900                 */
	sys_date.month  = (uint8_t)pLocalTime->tm_mon + 1;         /* months since January (0,11)      */
	sys_date.day    = (uint8_t)pLocalTime->tm_mday;            /* day of the month (1,31)          */

	return &sys_date;
}

/*******************************************************************************
* Function Name  : PL_RTC_SetTime
* Description    : Adjust the RTC time value
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
bool mtRtcSetSystemTime(sytemdate_t *time)
{
	time_t acttime;
	struct tm *pLocalTime;

	acttime = RTC_GetCounter();
	pLocalTime = localtime(&acttime);

	pLocalTime->tm_hour = (int)(time->hh);     		/* hours since midnight (0,23)      */
	pLocalTime->tm_min = (int)(time->mm);      		/* minutes after the hour (0,59)    */
	pLocalTime->tm_sec = (int)(time->ss);      		/* seconds after the minute (0,61)  */
	pLocalTime->tm_year = (int)(time->year - 1900); 	/* years since 1900 */
	pLocalTime->tm_mon = (int)(time->month - 1);  	/* months since January (0,11)      */
	pLocalTime->tm_mday = (int)(time->day);     	/* day of the month (1,31)          */
	pLocalTime->tm_wday  = (int)(time->weekday);    /* days since Sunday (0,6)          */
	pLocalTime->tm_yday  = (int)(time->yearday);    /* days since January 1 (0,365)     */
	pLocalTime->tm_isdst = (int)(0);    			/* Daylight Saving Time flag        */

	/* Update RTC counter */
	RTC_SetCounter(mktime(pLocalTime));

	/* Wait until last write operation on RTC registers has finished */
	if (stmWaitForLastTask() == 0)
	{
		return false;
	}

	/* Change the current time */
	memcpy(&sys_date, time, sizeof(sytemdate_t));

	return true;
}

/************************* End of File ****************************************/
