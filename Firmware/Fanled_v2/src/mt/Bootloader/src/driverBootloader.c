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
#include <stm32f10x_bkp.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_crc.h>

#include "mtInclude.h"
#include "../inc/driverBootloader.h"

#pragma GCC optimize("O0")
/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#	define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
#	define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

#define BKP_BOOTLOADER_ID				BKP_DR2
#define BKP_PATTERN_JUMP_TO_APP			0x1234
#define BKP_PATTERN_REQ_UPGRADE			0x0000

/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/
typedef void(*pFunction)(void);

/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/
#define BL_INFO(...)			DEBUG_INFO(__VA_ARGS__)
#define BL_ERR(...)				DEBUG_ERROR(__VA_ARGS__)

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
u32 convert_endianess(u32 little_endian)
{
	uint32_t b0,b1,b2,b3;

	b0 = (little_endian & 0x000000ff) << 24u;
	b1 = (little_endian & 0x0000ff00) << 8u;
	b2 = (little_endian & 0x00ff0000) >> 8u;
	b3 = (little_endian & 0xff000000) >> 24u;

	return b0 | b1 | b2 | b3;
}

u32 revbit(u32 data)
{
	asm("rbit r0,r0");
	return data;
};

/**
 *  [online tool](http://www.sunshine2k.de/coding/javascript/crc/crc_js.html)
 *  [reference source code](https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=%2Fpublic%2FSTe2ecommunities%2Fmcu%2FLists%2Fcortex_mx_stm32%2FCRC%20computation&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=9124)
 */
u32 CalcCRC32_BZIP2(u8 *buffer, u32 size, Bool swap)
{
	u32 i, j;
	u32 ui32;
	u32 converted;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

	CRC->CR = 1;

	/* delay for hardware ready */
	asm("NOP"); asm("NOP"); asm("NOP");

	i = size >> 2;

	while (i--)
	{
		if (swap)
		{
			converted = convert_endianess(*((u32 *)buffer));
			ui32 = converted;
		}
		else
		{
			ui32 = *((u32 *)buffer);
		}

		buffer += 4;

		/* reverse the bit order of input data */
		ui32 = revbit(ui32);

		CRC->DR = ui32;
	}

	ui32 = CRC->DR;

	/* reverse the bit order of output data */
	ui32 = revbit(ui32);

	i = size & 3;

	while (i--)
	{
		if (swap)
		{
			converted = convert_endianess((u32) * buffer++);
			ui32 ^= converted;
		}
		else
		{
			ui32 ^= (u32) * buffer++;
		}

		for (j = 0; j < 8; j++)
		{
			if (ui32 & 1)
			{
				ui32 = (ui32 >> 1) ^ 0xEDB88320;
			}
			else
			{
				ui32 >>= 1;
			}
		}
	}

	/* xor with 0xffffffff */
	ui32 ^= 0xffffffff;

	/* now the output is compatible with windows/winzip/winrar */
	return ui32;
};

/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
void mtBootloaderInitFlash(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR |
	                       RCC_APB1Periph_BKP, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();

	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();

	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
}

mtErrorCode_t mtBootloaderFlashWrite(uint32_t address, uint32_t data)
{
	if (FLASH_COMPLETE != FLASH_ProgramWord(address, data))
	{
		return MT_ERROR;
	}
	return MT_SUCCESS;
}

mtErrorCode_t mtBootloaderFlashWriteBuff(uint32_t address, uint32_t buff[], uint32_t len)
{
	UInt32 idx = 0;
	while(len)
	{
		if (MT_SUCCESS != mtBootloaderFlashWrite(address + idx * 4, buff[idx]))
		{
			return MT_ERROR;
		}
		idx ++;
		len --;
	}
	return MT_SUCCESS;
}

Void mtBootloaderCoreReset()
{
	core_disable_isr();
	NVIC_SystemReset();
}

Void mtBootloaderRequestUpgrade()
{
	/* Next time reset -> jump Bootloader will wait for firmware upgrade */
	BKP_WriteBackupRegister(BKP_DR2, BKP_PATTERN_REQ_UPGRADE);
}

Bool mtBootloaderCheckFwUpgardeRequest()
{
	Bool status = False;

	/* Check if there is any upgrage request */
	/* Set time registers to 00:00:00; configuration done via gui */
	if (BKP_PATTERN_REQ_UPGRADE == BKP_ReadBackupRegister(BKP_BOOTLOADER_ID))
	{
		BL_INFO("Get firmware upgrade request, wait for firmware download\r\n");
		status = True;
		goto exit;
	}
	BL_INFO("No firmware upgrade request detected\r\n");
exit:
	/* Next time reset -> jump straight to application */
	BKP_WriteBackupRegister(BKP_DR2, BKP_PATTERN_JUMP_TO_APP);
	return status;
}

/**
  * @brief  Jump to a given address and execute it
  * @param  None
  * @retval define jump address
  */
void mtBootloaderJumpToApp(uint32_t appOffset, uint32_t vtorOffset)
{
	/* Jump Parameters */
	pFunction Jump_To_Application;
	uint32_t JumpAddress;

	/* Disable all IRQ */
	core_disable_isr();

	/* Set system control register SCR->VTOR  */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, vtorOffset);

	JumpAddress = *(uint32_t*) (appOffset + 4);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(uint32_t*) appOffset);
	Jump_To_Application();
}

UInt32 mtBootloaderFlashCalculateCRC32(UInt8 *start_add, UInt16 len)
{
	return CalcCRC32_BZIP2(start_add, len, True);
}

void mtBootloaderEraseAppFw(void)
{
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint32_t EraseCounter = 0x00;
	uint32_t NbrOfPage = 0x00;

	/* Define the number of page to be erased */
	NbrOfPage = (FLASH_APP_END_ADDRESS - FLASH_APP_START_ADDRESS) / FLASH_PAGE_SIZE;

	/* Erase the FLASH pages */
	DEBUG_INFO("Erasing page from address: 0x%x\r\n", FLASH_APP_START_ADDRESS);
	for (EraseCounter = 0; (EraseCounter < NbrOfPage); EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(FLASH_APP_START_ADDRESS + (FLASH_PAGE_SIZE * EraseCounter));
		if (FLASHStatus != FLASH_COMPLETE)
		{
			DEBUG_INFO("Err when erasing, status=%d\r\n", FLASHStatus);
			return;
		}
	}
	DEBUG_INFO("Done erasing\r\n");
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

	if (startPage > (FLASH_TOTAL_SIZE / FLASH_PAGE_SIZE - 1) || (startPage < (FLASH_BOOTLOADER_SIZE / FLASH_PAGE_SIZE)))
	{
		DEBUG_ERROR("Invalid startPage number");
		return -1;
	}

	FLASHStatus = FLASH_ErasePage(FLASH_APP_START_ADDRESS + (FLASH_PAGE_SIZE * startPage));
	Address = FLASH_APP_START_ADDRESS + (FLASH_PAGE_SIZE * startPage);

	for (i = 0; i <  FLASH_PAGE_SIZE / 4; i++)
	{
		FLASHStatus = FLASH_ProgramWord(Address, pattern[i % 4]);
		Address += 4;
	}

	return FLASHStatus;
}

#pragma GCC reset_options
/************************* End of File ****************************************/
