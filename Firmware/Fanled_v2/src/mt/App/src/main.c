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

#include "App/inc/SystemConfig.h"
#include "Porting/inc/mtSPI.h"

#include "Effects/inc/image_data.h"

#include "Effects/inc/mtIncludeEffects.h"
#include "App/tst/fanledTestApp.h"
#include "Bootloader/inc/driverBootloader.h"
#include "RTC/inc/mtRtc.h"
#include "Porting/inc/mtUart.h"

#include "UartHandler/inc/mtUartHandler.h"
#include "UartHandler/inc/mtCcidHandler.h"

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


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
// SD Card specific Variables
FATFS gFatFs;
DIR mydir;
FILINFO myfno;
MediaPlayer MyPlayer;

// Display Buffer
uint8_t ledPanel[36*4];
Display_Type Fanled_Display;

/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/


/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/
void sd_first_step(void);
void init_sreenbuffer(uint16_t *value, uint16_t val1, uint16_t val2);

/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/
void init_sreenbuffer(uint16_t *value, uint16_t val1, uint16_t val2)
{
	uint8_t i, j;
	for(i = 0; i < 32; i++)
	{
		for(j = val1; j < val2; j++)
		{
			Fanled_Display.dis[j][i] = value[i];
		}
	}
}

void sd_first_step(void)
{
	FRESULT res;
	uint8_t count = 0;
	mtDelayMS(5);
	while((res!= FR_OK) && (count < 10)) {res = f_mount(&gFatFs, "0:", 1); count++;}
	if (f_mount(&gFatFs, "0:", 1) == FR_OK)
	{
	do
	{
		res = f_opendir(&mydir, "0:\\");
		mtDelayMS(5);
	}
	while(res != FR_OK);
	do
	{
		res = f_readdir(&mydir, &myfno);
		if (myfno.fname[0]) MyPlayer.NumOfItem++;
	}
	while(myfno.fname[0]);
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
	DEBUG_INFO("Bootloader !!!\r\n");
	mtBlInitFlash();
	while(1)
	{
		if (uart_buffer.state == CCID_UART_MSG_DONE)
		{
			checkCcidUARTQueue(&uart_buffer.state, &uart_buffer.queue[uart_buffer.start_of_msg], uart_buffer.msg_length + 10);
			mtUartResetQueue();
		}
	}
	mtBlJumpToApp(FLASH_APP_START_ADDRESS, FLASH_BOOTLOADER_SIZE); //FLASH_BOOTLOADER_SIZE

#elif defined(FANLED_APP)
//	mainTestColor();
//	mainTestHC05();
//	mainTestRTC();
//	mainPicture();
//	mainAppDeveloping();
//	mainTestHSVCircle();
//	mainTestNarutoEffect();

#endif

	BOOTLOADER_DEBUG_PRINT("Application !!!\r\n");	
	while(1);
	return 0;
}

/************************* End of File ****************************************/
