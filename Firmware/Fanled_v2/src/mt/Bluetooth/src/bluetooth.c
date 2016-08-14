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
#include <stdarg.h>
#include "Bluetooth/inc/bluetooth.h"
#include "App/inc/SystemConfig.h"
#include "UartHandler/inc/mtUartQueue.h"
#include "UartHandler/inc/mtUartHandler.h"
#include "Porting/inc/mtUart.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/
#define BLUETOOTH_KEYPIN_RESET()		//GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define BLUETOOTH_KEYPIN_SET()			//GPIO_SetBits(GPIOA, GPIO_Pin_11);


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
	if( checkUartString("OK\r\n", &uart_buffer) ) 
	{
		while(1);
	}	
}

void bltSendCharUart(char data)
{
	uart_write_char(data);
}

void bltSendMultiChar(char *data, uint32_t length)
{
	uint32_t i = 0;
	uint32_t temp = length;
	while(length != 0)
	{
		bltSendCharUart(data[i]);
		i++;
		length--;
	}
}

void bltPrintf(const char * __restrict format, ...)
{
	char Tx_Buf[256];
	va_list arg;

	memset((void *) Tx_Buf, 0, sizeof(Tx_Buf));
	// Start the varargs processing.
	va_start(arg, format);

	// Call vsnprintf to perform the conversion.  Use a large number for the
	// buffer size.
//	vsprintf((char *) Tx_Buf, format, arg);

	// End the varargs processing.
	va_end(arg);

	bltSendMultiChar((char *)Tx_Buf, strlen((const char *) Tx_Buf));
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
	mtUartResetQueue();
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