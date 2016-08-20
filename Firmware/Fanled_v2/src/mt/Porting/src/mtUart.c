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
#define USART_CMD							USART1
#define USART_CMD_RX_PRIORITY				0
#define USART_DBG							USART2

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
void uart_cmd_init(bool config)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USARTInitStructure;
	GPIO_InitTypeDef GPIOInitStructure;

	/* Configure GPIO Function */
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_10; //<! PA10-Rx
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_9; //<! PA9-TX
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOInitStructure);

	/* Configure UART parameters */
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
	USART_Init(USART_CMD, &USARTInitStructure);
	USART_ITConfig(USART_CMD, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART_CMD, ENABLE);

	/* Configure UART ISR parameters */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_CMD_RX_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_CMD_RX_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void uart_cmd_change_baud(uint32_t new_baudrate)
{
	USART_InitTypeDef USARTInitStructure;

	USART_Cmd(USART_CMD, DISABLE);
	USARTInitStructure.USART_BaudRate = new_baudrate;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity =  USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART_CMD, &USARTInitStructure);
	USART_Cmd(USART_CMD, ENABLE);
}

void uart_cmd_write_char(char c)
{
	while (USART_GetFlagStatus(USART_CMD, USART_FLAG_TXE) != SET);
	USART_SendData(USART_CMD, c);
	while (USART_GetFlagStatus(USART_CMD, USART_FLAG_TC) != SET);
}

void uart_cmd_write_str(char *str)
{
	while (*str)
	{
		uart_cmd_write_char(*str);
		str ++;
	}
}

void uart_dbg_init()
{
	USART_InitTypeDef USARTInitStructure;
	GPIO_InitTypeDef GPIOInitStructure;

	/* Configure GPIO Function */
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_3; //<! PA3-RX
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_2; //<! PA2-TX
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOInitStructure);

	/* Configure UART parameters */
	USARTInitStructure.USART_BaudRate = 460800;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity =  USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART_DBG, &USARTInitStructure);
	USART_Cmd(USART_DBG, ENABLE);
}

void uart_dbg_write_buff(uint8_t *p_data, uint16_t p_len)
{
	for (int i = 0; i < p_len; ++i)
	{
		/* Transmit Data */
		while (USART_GetFlagStatus(USART_DBG, USART_FLAG_TXE) == RESET);
		USART_SendData(USART_DBG, *(p_data++));
	}
}

void uart_dbg_read_buff(uint8_t *p_data, uint16_t p_len)
{
	for (int i = 0; i < p_len; ++i)
	{
		while (USART_GetFlagStatus(USART_DBG, USART_FLAG_RXNE) == RESET);
		*(p_data++) = USART_ReceiveData(USART_DBG);
	}
}

/*
 * _write
 */
int _write(int file, char *ptr, int len)
{
	uart_dbg_write_buff((uint8_t *)ptr, len);
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
		uart_dbg_read_buff((uint8_t *)ptr, 1);
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





