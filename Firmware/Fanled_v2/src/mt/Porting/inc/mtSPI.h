/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : template.h
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
//----- SPI2 options start -------
//Options can be overwriten in defines.h file
#ifndef SPI2_PRESCALER
#define SPI2_PRESCALER	SPI_BaudRatePrescaler_4
#endif
//Specify datasize
#ifndef SPI2_DATASIZE
#define SPI2_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef SPI2_FIRSTBIT
#define SPI2_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef SPI2_MASTERSLAVE
#define SPI2_MASTERSLAVE SPI_Mode_Master
#endif
//----- SPI2 options end -------

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
void mtSPIFanledSend(uint32_t Addr, uint32_t Size);

#ifdef __cplusplus
}
#endif

#endif /* _MYSPI_H_ */
