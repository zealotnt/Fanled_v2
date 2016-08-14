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
#include "Porting/inc/mtSPI.h"


/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
#define SPI_MASTER_DR_Base						0x4001300C
#define SPI_MASTER_Tx_DMA_Channel				DMA1_Channel3
#define SPI_Tx_DMA_IRQ							DMA1_Channel3_IRQn


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
DMA_InitTypeDef	g_DMA_InitStructure;


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
void mtSPIInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	//Common settings for all pins
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//Enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//Pinspack				        	MISO         		MOSI				SCK
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI2_PRESCALER;
	SPI_InitStruct.SPI_DataSize = 	SPI2_DATASIZE;
	SPI_InitStruct.SPI_Direction = 	SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = 	SPI2_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = 			SPI2_MASTERSLAVE;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_Cmd(SPI2, ENABLE);
}

uint8_t mtSPISend(SPI_TypeDef* SPIx, uint8_t data)
{
	//Fill output buffer with data
	SPIx->DR = data;
	//Wait for transmission to complete
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE));
	//Wait for received data to complete
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
	//Wait for SPI to be ready
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY));
	//Return data from buffer
	return SPIx->DR;
}

void mtSPISendMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint8_t *dataIn, uint16_t count)
{
	uint8_t i;
	for (i = 0; i < count; i++) 
	{
		dataIn[i] = mtSPISend(SPIx, dataOut[i]);
	}
}

void mtSPIWriteMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint16_t count)
{
	uint16_t i;
	for (i = 0; i < count; i++) 
	{
		mtSPISend(SPIx, dataOut[i]);
	}
}

void mtSPIReadMulti(SPI_TypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint16_t count)
{
	uint16_t i;
	for (i = 0; i < count; i++) 
	{
		dataIn[i] = mtSPISend(SPIx, dummy);
	}
}

void mtFanledSPIInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Common settings for all pins
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//Config Output for 		BLANK			LATCH
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//Enable clock for SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//Pinspack					 MOSI			SCK
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4|GPIO_Pin_6,(BitAction)0);
	
	SPI_I2S_DeInit(SPI1);
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);	
	mtFanledDMAInit();
}

void mtFanledDMAInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* SPI_SLAVE_Rx_DMA_Channel configuration ---------------------------------------------*/
	DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, DISABLE);
	DMA_DeInit(SPI_MASTER_Tx_DMA_Channel);
	g_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	
	//This field will be configured in transmit function
	g_DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;    
	//This field will be configured in transmit function
	g_DMA_InitStructure.DMA_BufferSize = (uint32_t)0;    
  	
	g_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  		
	g_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	g_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	g_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	g_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	g_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	g_DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	g_DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(SPI_MASTER_Tx_DMA_Channel, &g_DMA_InitStructure);
	DMA_ITConfig(SPI_MASTER_Tx_DMA_Channel, DMA_IT_TC, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = SPI_Tx_DMA_IRQ;
	// 0 is the highest
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, DISABLE);
	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

void mtSPIFanledSend(uint32_t Addr, uint32_t Size)
{
	/* Configure the buffer address and size */
	g_DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Addr;
	g_DMA_InitStructure.DMA_BufferSize = (uint32_t)Size;		//one package is 8 bit
	/* Configure the DMA Stream with the new parameters */
	DMA_Init(SPI_MASTER_Tx_DMA_Channel, &g_DMA_InitStructure);
	/* Enable the I2S DMA Stream*/
	DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, ENABLE); 
	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

/************************* End of File ****************************************/
