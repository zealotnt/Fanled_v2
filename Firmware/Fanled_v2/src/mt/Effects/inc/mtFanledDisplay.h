/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtFanledDisplay.h
**
** Module name       : Effects
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef MTFANLEDDISPLAY_H_
#define MTFANLEDDISPLAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "../App/inc/mtAppDef.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
#define FANLED_RESOLUTION			240
#define DISPLAY_SIZE				FANLED_RESOLUTION

#define COLOR_BLACK					0x00
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
typedef enum sharinganFlag
{
	STATE_ENABLE,
	STATE_DISABLE
} sharinganFlag_t;

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

	//variable of position of dual display
	uint8_t 	pos_now_inner, pos_now_outer;
	uint8_t 	mode;

	fanledAnimation_t 	animationCur;
	fanledAnimation_t 	animationOld;
	uint32_t			animationChangeCount;
	uint32_t 			animationChangeSpeed;

	//using for naruto effect
	uint32_t 		sharingan_count;
	sharinganFlag_t sharingan_flag;
	uint32_t 		misc_count;
	uint8_t 		misc_flag;
} Display_Type;

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
void mtFanledSendLineBuffer();
void blankAllLed(void);
void LED_BLANK();
void LED_UNBLANK();
void LED_LATCH();

#ifdef __cplusplus
}
#endif

#endif /* MTFANLEDDISPLAY_H_ */
