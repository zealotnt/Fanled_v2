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
#include <stm32f10x.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "../inc/bluetooth.h"
#include "App/inc/SystemConfig.h"
#include "Porting/inc/mtUart.h"

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
#define bltPrintf(...)					printf(__VA_ARGS__)

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
#if ABC
	if( checkUartString("OK\r\n", &uart_buffer) ) 
	{
		while(1);
	}
#endif
}

void bltSendCharUart(char data)
{
	uart_write_char(data);
}

void bltSendMultiChar(char *data, uint32_t length)
{
	uint32_t i = 0;
	while(length != 0)
	{
		bltSendCharUart(data[i]);
		i++;
		length--;
	}
}

void bltChangeBaud(uint32_t baurate)
{
	mtBluetoothUSARTChangeBaud(baurate);
}

void bltInitModule(bool config)
{
	// Init periph for HC05 module
	mtBluetoothUSARTInit(config);
	mtHC05KeyPinInit();
	if(config == true)
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
