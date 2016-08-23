/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : bluetooth.c
**
** Module name       : Bluetooth
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
#include <stm32f10x.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "../inc/bluetooth.h"
#include "App/inc/SystemConfig.h"
#include "Porting/inc/mtUart.h"
#include "UartHandler/inc/mtProtocolDriver.h"

#include <stdarg.h>
#include <stdio.h>

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/
#define BLUETOOTH_KEYPIN_RESET()		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define BLUETOOTH_KEYPIN_SET()			GPIO_SetBits(GPIOA, GPIO_Pin_11);
#define bltPrintf(...)					bltPrintStr(__VA_ARGS__)

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
void checkResponseOK(void)
{
//	if ( checkUartString("OK\r\n", &uart_buffer) )
//	{
//		while (1);
//	}
}

void bltPrintStr(char *str)
{
	while (*str)
	{
		uart_cmd_write_char(*str);
		str++;
	}
}

void bltSendMultiChar(char *data, uint32_t length)
{
	mtUartWriteBuf((UInt8 *)data, length);
}

void bltChangeBaud(uint32_t baurate)
{
	mtBluetoothUSARTChangeBaud(baurate);
}

/* True for first time config HC-05 module */
/* False for instant usage */
void bltInitModule(bool config)
{
	/* Initialize peripheral for HC05 module */
	mtBluetoothUSARTInit(config);
	mtHC05KeyPinInit();
	if (config == true)
	{
		bltInitStandardBluetoothMode();
	}
	else
	{
		BLUETOOTH_KEYPIN_RESET();
	}
}

void bltInitStandardBluetoothMode(void)
{
	BLUETOOTH_KEYPIN_SET();
	mtDelayMS(200);
	bltPrintf("AT\r\n");
	mtDelayMS(300);
	checkResponseOK();

	bltPrintf("AT+CMODE=1\r\n");
	mtDelayMS(300);
	checkResponseOK();

	bltPrintf("AT+NAME=\"ZEALOT-HC\"\r\n");
	mtDelayMS(300);
	checkResponseOK();

	bltPrintf("AT+PSWD=\"1234\"\r\n");
	mtDelayMS(300);
	checkResponseOK();

	bltPrintf("AT+ROLE=0\r\n");
	mtDelayMS(300);
	checkResponseOK();

	bltPrintf("AT+UART=115200,1,0\r\n");
	mtDelayMS(300);
	checkResponseOK();

	BLUETOOTH_KEYPIN_RESET();
}

/************************* End of File ****************************************/
