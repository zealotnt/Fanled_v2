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
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"

#include <stdio.h>
#include <stdbool.h>

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define USARTx							USART1
#define UARTx_RX_PRIORITY				0
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
void uart_init(bool config)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USARTInitStructure;
	GPIO_InitTypeDef GPIOInitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_10; //<! PA10-Rx
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_9; //<! PA9-TX
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	if (config == true)
	{
		USARTInitStructure.USART_BaudRate = 38400;
	}
	else
	{
		USARTInitStructure.USART_BaudRate = 460800;
	}
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity =  USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USARTx, &USARTInitStructure);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_RX_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_RX_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USARTx, ENABLE);
}

void uart_change_baud(uint32_t new_baudrate)
{
	USART_InitTypeDef USARTInitStructure;

	USART_Cmd(USARTx, DISABLE);
	USARTInitStructure.USART_BaudRate = new_baudrate;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity =  USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USARTx, &USARTInitStructure);
	USART_Cmd(USARTx, ENABLE);
}

void uart_write_char(char c)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != SET);
	USART_SendData(USARTx, c);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);
}

void uart_write_str(char *str)
{
	while (*str)
	{
		uart_write_char(*str);
		str ++;
	}
}

void uart_write_buff(uint8_t *p_data, uint16_t p_len)
{
	for (int i = 0; i < p_len; ++i)
	{
		/* Transmit Data */
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *(p_data++));
	}
}

void uart_read_buff(uint8_t *p_data, uint16_t p_len)
{
	for (int i = 0; i < p_len; ++i)
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
		*(p_data++) = USART_ReceiveData(USARTx);
	}
}

/*
 * _write
 */
int _write(int file, char *ptr, int len)
{
	uart_write_buff((uint8_t *)ptr, len);
	return len;
}


/*
 * _read
 */
int _read(int file, char *ptr, int len)
{
	int n;
	int num = 0;
	for (n = 0; n < len; n++)
	{
		uart_read_buff((uint8_t *)ptr, 1);
		printf("%c", *ptr);
		fflush(stdout);
		num++;
		if (*ptr == 0x0d)
		{
			break;
		}
		ptr++;

	}
	return num;
}

/************************* End of File ****************************************/





