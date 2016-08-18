/*
 * mtFanledDisplay.h
 *
 *  Created on: Aug 18, 2016
 *      Author: zealot
 */

#ifndef MTFANLEDDISPLAY_H_
#define MTFANLEDDISPLAY_H_

#include <stdbool.h>
#include <stdint.h>

#define FANLED_RESOLUTION			240
#define DISPLAY_SIZE				FANLED_RESOLUTION


#define COLOR_RED_MAX				0xf800
#define COLOR_BLUE_MAX				0x1f
#define COLOR_GREEN_MAX				0x7e0
#define COLOR_ORANGE_MAX			0xfbe0
#define COLOR_YELLOW_MAX			0xffe0
#define COLOR_VIOLET_MAX			0x781f

#define CALENDAR_SECOND_STICK_COLOR			COLOR_RED_MAX
#define CALENDAR_MINUTE_STICK_COLOR			COLOR_GREEN_MAX
#define CALENDAR_HOUR_STICK_COLOR			COLOR_BLUE_MAX

#define LED_BLANK()   				//GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)1);
#define LED_UNBLANK()   			//GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)0);
#define LED_LATCH()					//GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)1);GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)0);

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
	uint8_t 	rtc_flag;

	//variable of position of dual display
	uint8_t 	pos_now_inner, pos_now_outer;
	uint8_t 	mode;

	uint8_t 	animation;
	uint8_t 	animation_old;
	uint32_t 	animation_change_speed;

	//using for naruto effect
	uint32_t 		sharingan_count;
	sharinganFlag_t sharingan_flag;
	uint32_t 		misc_count;
	uint8_t 		misc_flag;
} Display_Type;

void mtFanledSendLineBuffer();
void blankAllLed(void);
#endif /* MTFANLEDDISPLAY_H_ */
