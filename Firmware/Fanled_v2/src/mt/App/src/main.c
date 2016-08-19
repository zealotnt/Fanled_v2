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
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "mtInclude.h"
#include "mtVersion.h"
#include "App/inc/SystemConfig.h"
#include "App/tst/fanledTestApp.h"
#include "Porting/inc/mtSPI.h"
#include "Porting/inc/mtUart.h"
#include "Effects/inc/image_data.h"
#include "Effects/inc/mtIncludeEffects.h"
#include "Bootloader/inc/driverBootloader.h"
#include "RTC/inc/mtRtc.h"
#include "UartHandler/inc/mtSerialCmdParser.h"

#include "ff.h"
/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/
extern serialQueuePayload_t gQueuePayload;

/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
// SD Card specific Variables
FATFS gFatFs;
DIR mydir;
FILINFO myfno;
MediaPlayer MyPlayer;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
void sd_first_step(void);

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
void sd_first_step(void)
{
	FRESULT res = FR_DISK_ERR;
	uint8_t count = 0;
	mtDelayMS(5);
	while ((res != FR_OK) && (count < 10))
	{
		res = f_mount(&gFatFs, "0:", 1);
		count++;
	}

	if (f_mount(&gFatFs, "0:", 1) == FR_OK)
	{
		do
		{
			res = f_opendir(&mydir, "0:\\");
			mtDelayMS(5);
		}
		while (res != FR_OK);

		do
		{
			res = f_readdir(&mydir, &myfno);
			if (myfno.fname[0]) { MyPlayer.NumOfItem++; }
		}
		while (myfno.fname[0]);
	}
	MyPlayer.ChoiceNow = 0;
}

/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
#define FANLED_BOOTLOADER
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to find out why we're here */
	while (1);
}

int main(void)
{
#if defined(FANLED_BOOTLOADER)
	initBootloader();
	DEBUG_INFO("Bootloader " FIRMWARE_VER_FULL "\r\n");
	mtBootloaderInitFlash();
	while (1)
	{
		if (True == gQueuePayload.Done)
		{
			mtSerialCmdDataLinkHandlingThread(gQueuePayload);
			gQueuePayload.Done = False;
		}
	}
	mtBootloaderJumpToApp(FLASH_APP_START_ADDRESS, FLASH_BOOTLOADER_SIZE); //FLASH_BOOTLOADER_SIZE

#elif defined(FANLED_APP)
//	mainTestColor();
//	mainTestHC05();
//	mainTestRTC();
//	mainPicture();
//	mainAppDeveloping();
//	mainTestHSVCircle();
//	mainTestNarutoEffect();

#endif

	DEBUG_INFO("Application !!!\r\n");
	while (1);
	return 0;
}

/************************* End of File ****************************************/
