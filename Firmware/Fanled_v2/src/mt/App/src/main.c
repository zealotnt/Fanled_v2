/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : main.c
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
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "mtInclude.h"
#include "../inc/main.h"
#include "../inc/mtVersion.h"
#include "App/inc/SystemConfig.h"
#include "App/tst/fanledTestApp.h"
#include "Porting/inc/mtSPI.h"
#include "Porting/inc/mtUart.h"
#include "Porting/inc/mtWdt.h"
#include "Effects/inc/image_data.h"
#include "Effects/inc/mtIncludeEffects.h"
#include "Bootloader/inc/driverBootloader.h"
#include "RTC/inc/mtRtc.h"
#include "UartHandler/inc/mtSerialCmdParser.h"

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
static pMainHandler mainHandler = Null;

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to find out why we're here */
	while (1);
}

int main(void)
{
#if (FANLED_BOOTLOADER)
	initBootloader();
	uart_dbg_init();
	DEBUG_INFO("Bootloader %s \r\n", FIRMWARE_VERSION_FULL);

	if (True != mtBootloaderCheckFwUpgardeRequest())
	{
		DEBUG_INFO("Jump to app \r\n");
		mtBootloaderJumpToApp(FLASH_APP_START_ADDRESS, FLASH_BOOTLOADER_SIZE);
	}

	while (1)
	{
		if (True == gQueuePayload.Done)
		{
			mtSerialCmdDataLinkHandlingThread(gQueuePayload);
			gQueuePayload.Done = False;
		}
	}

#elif (FANLED_APP)
	initAll();
	uart_dbg_init();
	DEBUG_INFO("App %s \r\n", FIRMWARE_VERSION_FULL);

//	FanledTestColor();
//	FanledTestPicture();
//	FanledAppDeveloping();
//	FanledTestHSVCircle();
//	FanledTestNarutoEffect();

	while (1)
	{
		mtWdtFeed();

		if (mainHandler != Null)
		{
			mainHandler(Null);
		}

		if (True == gQueuePayload.Done)
		{
			mtSerialCmdDataLinkHandlingThread(gQueuePayload);
			gQueuePayload.Done = False;
		}
	}

//	mainTestHC05();
//	mainTestRTC();

#endif
	return 0;
}

Void mainCallBackRegister(pMainHandler call_back)
{
	mainHandler = call_back;
}
/************************* End of File ****************************************/
