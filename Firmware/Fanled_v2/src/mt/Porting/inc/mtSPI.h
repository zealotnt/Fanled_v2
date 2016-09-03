/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtSPI.h
**
** Module name       : Porting
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef _MYSPI_H_
#define _MYSPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define FANLED_DISPLAY_SPI_PORT				GPIOA
#define FANLED_DISPLAY_SPI_NUM				SPI1
#define FANLED_DISPLAY_SPI_RCC_DMA			RCC_AHBPeriph_DMA1
#define FANLED_DISPLAY_SPI_RCC_DMA_CMD		RCC_AHBPeriphClockCmd
#define FANLED_DISPLAY_SPI_RCC_PORT			RCC_APB2Periph_GPIOA
#define FANLED_DISPLAY_SPI_RCC_PORT_CMD		RCC_APB2PeriphClockCmd
#define FANLED_DISPLAY_SPI_RCC_SPI			RCC_APB2Periph_SPI1
#define FANLED_DISPLAY_SPI_RCC_SPI_CMD		RCC_APB2PeriphClockCmd

#define SPI_MASTER_DR_Base						0x4001300C
#define SPI_MASTER_Tx_DMA_Channel				DMA1_Channel3
#define SPI_DISPLAY_Tx_DMA_IRQ					DMA1_Channel3_IRQn
/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
void mtSPIInit(void);
uint8_t mtSPISend(SPI_TypeDef* SPIx, uint8_t data);
void mtSPISendMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint8_t *dataIn, uint16_t count);
void mtSPIWriteMulti(SPI_TypeDef* SPIx, uint8_t *dataOut, uint16_t count);
void mtSPIReadMulti(SPI_TypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint16_t count);

void mtFanledSPIInit(void);
void mtFanledDMAInit(void);
void mtFanledSendDmaBuffer(uint32_t Addr, uint32_t Size);

#ifdef __cplusplus
}
#endif

#endif /* _MYSPI_H_ */
