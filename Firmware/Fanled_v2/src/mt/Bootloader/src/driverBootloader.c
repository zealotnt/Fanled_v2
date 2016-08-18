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
#include <stdio.h>
#include <misc.h>
#include <stm32f10x_flash.h>

#include "mtInclude.h"
#include "../inc/driverBootloader.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#	define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
#	define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

/* Define Application Address Area */
#define BANK1_WRITE_START_ADDR  ((uint32_t)FLASH_APP_START_ADDRESS)
#define BANK1_WRITE_END_ADDR    ((uint32_t)FLASH_APP_END_ADDRESS) ///< BANK1_WRITE_START_ADDR + ProgramSize

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/
typedef void(*pFunction)(void);

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
void mtBlInitFlash(void)
{
	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();
	
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
}

void mtBlFlashWrite(uint32_t address, uint32_t data)
{
	FLASH_ProgramWord(address, data);
}

/**
  * @brief  Jump to a given address and execute it 
  * @param  None
  * @retval define jump address
  */
void mtBlJumpToApp(uint32_t appOffset, uint32_t vtorOffset)
{
	/* Jump Parameters */
	pFunction Jump_To_Application;	
	uint32_t JumpAddress;
	
	/* TODO: Should disable all IRQ */
	
	/* Set system control register SCR->VTOR  */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, vtorOffset);
	
	JumpAddress = *(uint32_t*) (appOffset + 4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(uint32_t*) appOffset);
	Jump_To_Application();
}

void mtBlEraseAppFw(void)
{
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint32_t EraseCounter = 0x00;
	uint32_t NbrOfPage = 0x00;
	
	/* Define the number of page to be erased */
	NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	
	/* Erase the FLASH pages */
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
		DEBUG_INFO("Erasing page from address: 0x%lx\r\n", BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
		FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}
}

/**
 * @Function	: retAppPage
 * @Brief		: input page as relative to App's Flash region (0-StartofAppPage --> MaxAppPage),
 *                return as absolute page address (0 --> EndOfBootloader's Page --> StartofAppPage --> MaxAppPage)
 * @Parameter	:
 * @retval Value:
 */
uint32_t retAppPage(uint32_t relativePage)
{
	uint32_t offset = (FLASH_BOOTLOADER_SIZE / FLASH_PAGE_SIZE);
	return (relativePage + offset);
}

FLASH_Status testWriteDummyDataToFlash(uint32_t startPage)
{
	volatile FLASH_Status FLASHStatus;
	const uint32_t pattern[4] = {0x12345678, 0x98765432, 0xa5a51234, 0x5a5a4321};
	uint32_t i;
	uint32_t Address = 0;

	if (startPage > (FLASH_TOTAL_SIZE/FLASH_PAGE_SIZE - 1) || (startPage < (FLASH_BOOTLOADER_SIZE/FLASH_PAGE_SIZE)))
	{
		DEBUG_ERROR("Invalid startPage number");
		return -1;
	}

	FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * startPage));
	Address = BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * startPage);

	for (i = 0; i <  FLASH_PAGE_SIZE/4; i++)
	{
		FLASHStatus = FLASH_ProgramWord(Address, pattern[i%4]);
		Address += 4;
	}

	return FLASHStatus;
}

/************************* End of File ****************************************/
