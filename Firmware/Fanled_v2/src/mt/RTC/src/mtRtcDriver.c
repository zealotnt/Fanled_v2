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
** @date 	Feb 23, 2016
** @author	zealot
** - Development
==============================================================================*/

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include <time.h>
#include <misc.h>
#include <stm32f10x_rtc.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_pwr.h>

#include "../inc/mtRtcDriver.h"
#include "../inc/mtRtc.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define PL_RTC_Update               	RTC_IRQHandler
#define RTC_PREEMPTY_PRIORITY			4
#define RTC_SUB_PRIORITY				0

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/
//typedef struct tm time_t;

/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/
extern sytemdate_t sys_date;

/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
uint32_t stmWaitForSynchro(void);

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
/**
	* @brief  Waits until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL)
	*   are synchronized with RTC APB clock.
	* @note   This function must be called before any read operation after an APB reset
	*   or an APB clock stop.
	* @param  None
	* @retval None
	*/
uint32_t stmWaitForSynchro(void)
{
	uint32_t del = 1000000;
	/* Clear RSF flag */
	RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
	/* Loop until RSF flag is set */
	while (((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET) && del)
	{
		del--;
	}
	return del;
}

/*******************************************************************************
* Function Name  : PL_ConfigureRTC
* Description    : Configure the RTC if the backup registers are not initialized
* Input          : Request delay (1 unit = 100us)
* Output         : None
* Return         : None
*******************************************************************************/
bool stmConfigureRTC(uint32_t countval)
{
	uint32_t nRTCdel;

	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	BKP_DeInit();

	nRTCdel = 1000000;
#ifdef  DEVICE_USE_EXTOSC_32KHZ
	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && nRTCdel)
	{
		nRTCdel--;
	}
#else
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */
	while ((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && nRTCdel)
	{
		nRTCdel--;
	}
#endif

	if (nRTCdel)
	{
#ifdef  DEVICE_USE_EXTOSC_32KHZ
		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#else
		/* Select LSI as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
#endif
		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);
		/* Wait for RTC registers synchronization */
		if (stmWaitForSynchro() == 0)
		{
			return false;
		}
		/* Wait until last write operation on RTC registers has finished */
		if (stmWaitForLastTask() == 0)
		{
			return false;
		}
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		if (stmWaitForLastTask() == 0)
		{
			return false;
		}
		/* Set RTC prescaler: set RTC period to 1sec */
#ifdef  DEVICE_USE_EXTOSC_32KHZ
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#else
		RTC_SetPrescaler(40000);
		//RTC_SetPrescaler(62500);
#endif
		/* Wait until last write operation on RTC registers has finished */
		if (stmWaitForLastTask() == 0)
		{
			return false;
		}
		/* Set time registers to 00:00:00; configuration done via gui */
		if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
		{
			RTC_SetCounter(countval);
		}

		/* Wait until last write operation on RTC registers has finished */
		if (stmWaitForLastTask() == 0)
		{
			return false;
		}
		/* Write RTC flag in backup register */
		BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
		return true;
	}
	return false;
}


/**
	* @brief  Waits until last write operation on RTC registers has finished.
	* @note   This function must be called before any write to RTC registers.
	* @param  None
	* @retval None
	*/
uint32_t stmWaitForLastTask(void)
{
	uint32_t del = 1000000;

	/* Loop until RTOFF flag is set */
	while (((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET) && del)
	{
		del--;
	}
	return del;
}

/*******************************************************************************
* Function Name  : PL_InitRTC
* Description    : Configure the internal RTC
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
bool stmInitRTC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	struct tm tLocalTime;

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_PREEMPTY_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();

	/* Check if backup data register value is not correct or not yet programmed (when
		 the first time the program is executed) */
	if (BKP_ReadBackupRegister(BKP_DR1) == 0x5A5A)
	{
		/* Check if the Power On Reset flag is set */
		if ((RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) || (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET))
		{
			// Reset or power off occurred
		}
		/* Device use LSE --> Check the clock, if it is ok --> Go on, if not, set the default time */
		/* Wait for RTC registers synchronization */
		if (stmWaitForSynchro() != 0)
		{
			/* Enable the RTC Second */
			RTC_ITConfig(RTC_IT_SEC, ENABLE);
			/* Wait until last write operation on RTC registers has finished */
			if (stmWaitForLastTask() != 0)
			{
				/* Clear reset flags */
				RCC_ClearFlag();
				return true;
			}
		}
		/* Device use LSI --> Just config the RTC and return */
		else
		{
			uint32_t nRTCdel;
			/* Enable PWR and BKP clocks */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
			nRTCdel = 1000000;
			/* Enable the LSI OSC */
			RCC_LSICmd(ENABLE);
			/* Wait till LSI is ready */
			while ((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && nRTCdel)
			{
				nRTCdel--;
			}
			/* Select LSI as RTC Clock Source */
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
			/* Enable RTC Clock */
			RCC_RTCCLKCmd(ENABLE);
			/* Wait for RTC registers synchronization */
			if (stmWaitForSynchro() == 0)
			{
				return false;
			}
			/* Wait until last write operation on RTC registers has finished */
			if (stmWaitForLastTask() == 0)
			{
				return false;
			}
			/* Enable the RTC Second */
			RTC_ITConfig(RTC_IT_SEC, ENABLE);
			/* Wait until last write operation on RTC registers has finished */
			if (stmWaitForLastTask() == 0)
			{
				return false;
			}
			/* Set RTC prescaler: set RTC period to 1sec */
			RTC_SetPrescaler(40000);
			return true;
		}
	}

	/* Date-Time Initialization: 16 December 2011 15:20:00 */
	tLocalTime.tm_hour  = 20;   /* hours since midnight (0,23)      */
	tLocalTime.tm_min   = 50;   /* minutes after the hour (0,59)    */
	tLocalTime.tm_sec   = 0;    /* seconds after the minute (0,61)  */
	tLocalTime.tm_mday  = 10;   /* day of the month (1,31)          */
	tLocalTime.tm_mon   = 0;   	/* months since January (0,11)      */
	tLocalTime.tm_year  = 116;  /* years since 1900                 */
	tLocalTime.tm_wday  = 0;    /* days since Sunday (0,6)          */
	tLocalTime.tm_yday  = 9;  	/* days since January 1 (0,365)     */
	tLocalTime.tm_isdst = 0;    /* Daylight Saving Time flag        */
	/* RTC Configuration, reset everything and set the local time */
	if (!stmConfigureRTC(mktime(&tLocalTime)))
	{
		return false;
	}

	/* Clear reset flags */
	RCC_ClearFlag();
	return true;
}

/*******************************************************************************
* Function Name  : PL_RTC_LimitCheck
* Description    : Verify RTC registers limits
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void stmUpdate(void)
{
	time_t acttime;
	struct tm *pLocalTime;

	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		acttime = RTC_GetCounter();
		pLocalTime = localtime(&acttime);
		sys_date.hh     = (uint8_t)pLocalTime->tm_hour;			/* hours since midnight (0,23)      */
		sys_date.mm     = (uint8_t)pLocalTime->tm_min;			/* minutes after the hour (0,59)    */
		sys_date.ss     = (uint8_t)pLocalTime->tm_sec;			/* seconds after the minute (0,61)  */
		sys_date.year   = (uint16_t)pLocalTime->tm_year + 1900;	/* years since 1900                 */
		sys_date.month  = (uint8_t)pLocalTime->tm_mon;			/* months since January (0,11)      */
		sys_date.day    = (uint8_t)pLocalTime->tm_mday;			/* days since January 1 (0,365)     */
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
}

/************************* End of File ****************************************/
