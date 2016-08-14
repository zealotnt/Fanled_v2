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

#ifndef _MT_APP_DEF_H_
#define _MT_APP_DEF_H_

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define FANLED_RESOLUTION			240
#define DISPLAY_SIZE				FANLED_RESOLUTION

#define ENABLE		1
#define DISABLE		0

/* Definition constant for charatcter display*****************************/
#define INNER_CIRCLE			1
#define OUTER_CIRCLE			2

#define SCROLL_ENABLE_COUNTING		0
#define SCROLL_ENABLE_DISPLAY		1
#define SCROLL_DISABLE_FINISH		101

#define PUT_ENABLE_LOADING		0
#define PUT_ENABLE_DISPLAY		1

#define JUST_RESET				100
#define STATE_FREST				255

#define RTC_UPDATE				5
#define RTC_WAIT				6
#define RTC_ENBALE				7


#define ANIMATION_IMAGE					ANIMATION_1
#define ANIMATION_CALENDAR				ANIMATION_2
#define ANIMATION_VALENTINE				ANIMATION_3
#define ANIMATION_HAPPYNEWYEAR			ANIMATION_4


#define ANIMATION_1				1
#define ANIMATION_2				2
#define ANIMATION_3				3
#define ANIMATION_4				4
#define ANIMATION_5				5

#define MAX_ANIMATION			3
#define ANIMATION_CHANGE_SPEED	15000

/* These definition works for Sharingan Effect*/
#define RING_SIZE		100
#define SHARINGAN_CHANGE_SPEED	50
#define MISC_CHANGE_SPEED		30

/* custom sdcard library settings ****************************************/
#define FATFS_USE_SDIO                 		0
#define FATFS_SPI                			SPI2
#define FATFS_SPI_PINSPACK        			SPI_PinsPack_2
#define FATFS_CS_RCC       				 	RCC_APB2Periph_GPIOB
#define FATFS_CS_PORT        				GPIOB
#define FATFS_CS_PIN        				GPIO_Pin_12

#define MONDAY			1
#define TUESDAY			2
#define WEDNESDAY		3
#define THURSDAY		4
#define FRIDAY			5
#define SATURDAY		6
#define SUNDAY			7


#define COLOR_RED_MAX				0xf800
#define COLOR_BLUE_MAX				0x1f
#define COLOR_GREEN_MAX				0x7e0
#define COLOR_ORANGE_MAX			0xfbe0
#define COLOR_YELLOW_MAX			0xffe0
#define COLOR_VIOLET_MAX			0x781f

#define CALENDAR_SECOND_STICK_COLOR			COLOR_RED_MAX
#define CALENDAR_MINUTE_STICK_COLOR			COLOR_GREEN_MAX
#define CALENDAR_HOUR_STICK_COLOR			COLOR_BLUE_MAX

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/
typedef enum __flags {
    FLAG_TX_COMPLETED   = 1<<1,
    FLAG_RX_COMPLETED   = 1<<2,
    FLAG_READ_COMPLETED = 1<<3,
    FLAG_WRITE_COMPLETED = 1<<4,
} flags_t;

typedef struct
{
	uint8_t NumOfItem;
	int8_t ChoiceNow;
	int8_t OldChoice;
} MediaPlayer;

typedef struct
{
	uint16_t Red;
	uint16_t Green;
	uint16_t Blue;
} rgb_t;

typedef struct
{
	uint16_t	dis[FANLED_RESOLUTION][32];
	uint16_t 	pos;
	
	//variable of scroll
	uint16_t 	scroll_times, pos_char;									
	
	//variable of puts, putc
	uint8_t 	numOfchar, enable_flag, move_flag, section_of_char; 	
	uint8_t 	rtc_flag;
	
	//variable of position of dual display
	uint8_t 	pos_now_inner, pos_now_outer;							
	uint8_t 	mode;
		
	uint8_t animation;
	uint8_t animation_old;
	uint32_t animation_change_speed;
		
	//using for naruto effect
	uint32_t sharingan_count;											
	uint8_t sharingan_flag;
	uint32_t misc_count;												
	uint8_t misc_flag;
} Display_Type;


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/
/* Definition fanled actions  ********************************************/
#define LED_BLANK()   				//GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)1);
#define LED_UNBLANK()   			//GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)0);
#define LED_LATCH()					//GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)1);GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)0);

#define SPI_WAIT() 					//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
#define SPI_SEND(b) 				//SPI_I2S_SendData(SPI1,b)

/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/


#endif /* _MT_APP_DEF_H_ */
