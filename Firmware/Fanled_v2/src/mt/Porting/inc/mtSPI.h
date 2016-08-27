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
