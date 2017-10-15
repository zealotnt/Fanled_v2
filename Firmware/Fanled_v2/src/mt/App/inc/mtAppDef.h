/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtAppDef.h
**
** Module name       : App
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef _MT_APP_DEF_H_
#define _MT_APP_DEF_H_

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "mtInclude.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/

/* Definition constant for charatcter display*****************************/
#define SCROLL_ENABLE_COUNTING		0
#define SCROLL_ENABLE_DISPLAY		1
#define SCROLL_DISABLE_FINISH		101

#define PUT_ENABLE_LOADING		0
#define PUT_ENABLE_DISPLAY		1

#define JUST_RESET				100
#define STATE_FREST				255

#define ANIMATION_IMAGE					ANIMATION_1
#define ANIMATION_CALENDAR				ANIMATION_2
#define ANIMATION_VALENTINE				ANIMATION_3
#define ANIMATION_HAPPYNEWYEAR			ANIMATION_4

#define ANIMATION_1				1
#define ANIMATION_2				2
#define ANIMATION_3				3
#define ANIMATION_4				4
#define ANIMATION_5				5

#define MAX_ANIMATION			2
#define ANIMATION_CHANGE_SPEED	15000

/* These definition works for Sharingan Effect*/
#define RING_SIZE				100
#define SHARINGAN_CHANGE_SPEED	50
#define MISC_CHANGE_SPEED		30

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
typedef enum __flags
{
	FLAG_TX_COMPLETED   = 1 << 1,
	FLAG_RX_COMPLETED   = 1 << 2,
	FLAG_READ_COMPLETED = 1 << 3,
	FLAG_WRITE_COMPLETED = 1 << 4,
} flags_t;

typedef struct
{
	uint8_t NumOfItem;
	int8_t ChoiceNow;
	int8_t OldChoice;
} SdManager_t;

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/
/* Definition fanled actions  ********************************************/
#define SPI_WAIT() 					//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
#define SPI_SEND(b) 				//SPI_I2S_SendData(SPI1,b)

/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/


#endif /* _MT_APP_DEF_H_ */
