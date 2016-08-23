/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtDisplayText.c
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

/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "../inc/mtIncludeEffects.h"
#include "App/inc/SystemConfig.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/
#define INNER_CIRCLE			1
#define OUTER_CIRCLE			2


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
extern uint8_t ledPanel[36 * 4];


//*****************************************************************************
// ASCII table
//*****************************************************************************
const char ASCII[][6] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20
	, {0x00, 0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
	, {0x00, 0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
	, {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
	, {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
	, {0x00, 0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
	, {0x00, 0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
	, {0x00, 0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
	, {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
	, {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
	, {0x00, 0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
	, {0x00, 0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
	, {0x00, 0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
	, {0x00, 0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
	, {0x00, 0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
	, {0x00, 0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
	, {0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
	, {0x00, 0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
	, {0x00, 0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
	, {0x00, 0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
	, {0x00, 0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
	, {0x00, 0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
	, {0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
	, {0x00, 0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
	, {0x00, 0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
	, {0x00, 0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
	, {0x00, 0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
	, {0x00, 0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
	, {0x00, 0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
	, {0x00, 0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
	, {0x00, 0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
	, {0x00, 0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
	, {0x00, 0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
	, {0x00, 0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
	, {0x00, 0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
	, {0x00, 0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
	, {0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
	, {0x00, 0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
	, {0x00, 0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
	, {0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
	, {0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
	, {0x00, 0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
	, {0x00, 0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
	, {0x00, 0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
	, {0x00, 0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
	, {0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
	, {0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
	, {0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
	, {0x00, 0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
	, {0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
	, {0x00, 0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
	, {0x00, 0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
	, {0x00, 0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
	, {0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
	, {0x00, 0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
	, {0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
	, {0x00, 0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
	, {0x00, 0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
	, {0x00, 0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
	, {0x00, 0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
	, {0x00, 0x02, 0x04, 0x08, 0x10, 0x20} // 5c �
	, {0x00, 0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
	, {0x00, 0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
	, {0x00, 0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
	, {0x00, 0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
	, {0x00, 0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
	, {0x00, 0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
	, {0x00, 0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
	, {0x00, 0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
	, {0x00, 0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
	, {0x00, 0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
	, {0x00, 0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
	, {0x00, 0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
	, {0x00, 0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
	, {0x00, 0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
	, {0x00, 0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
	, {0x00, 0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
	, {0x00, 0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
	, {0x00, 0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
	, {0x00, 0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
	, {0x00, 0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
	, {0x00, 0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
	, {0x00, 0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
	, {0x00, 0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
	, {0x00, 0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
	, {0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
	, {0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
	, {0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
	, {0x00, 0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
	, {0x00, 0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
	, {0x00, 0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
	, {0x00, 0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
	, {0x00, 0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
	, {0x00, 0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
	, {0x00, 0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
	, {0x00, 0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
	, {0x00, 0x04, 0x04, 0x15, 0x0e, 0x04}	// 80 ->
};


/******************************************************************************/
/* LOCAL (STATIC) VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
unsigned int pos_now = 0;

/******************************************************************************/
/* LOCAL (STATIC) FUNCTION DECLARATION SECTION                                */
/******************************************************************************/


/******************************************************************************/
/* LOCAL FUNCTION DEFINITION SECTION                                          */
/******************************************************************************/


/******************************************************************************/
/* GLOBAL FUNCTION DEFINITION SECTION                                         */
/******************************************************************************/
/* modified for 16bit rgb color */
void updatePanel16b(uint16_t *input_color, bool blank)
{
	uint8_t i;
	if ( (input_color == 0) && (blank == true) )
	{
		for (i = 0; i < 32; i++)
		{
			assignLed16b(0, i);
		}
	}
	else if ( (*input_color != 0) && (blank == true) )
	{
		for (i = 0; i < 32; i++)
		{
			assignLed16b(*input_color, i);
		}
	}
	else
	{
		for (i = 0; i < 32; i++)
		{
			assignLed16b(input_color[i], i);
		}
	}
}

/* modified for 16bit rgb color */
void assignLed16b(uint16_t input, uint8_t num)
{
	uint16_t pos;
	rgb_t rgb;
	pos = 12 * 3 * num;
	rgb.Red = (input & 0xF800) >> 11;
	rgb.Green = ((input & 0x07E0) >> 5) & 0x3f;
	rgb.Blue = (input & 0x1f);
	if (pos % 8 == 0)
	{
		pos = pos / 8;
		ledPanel[pos] =   rgb.Red >> 4;
		ledPanel[pos + 1] = ((rgb.Red & 0x0F) << 4);
		ledPanel[pos + 2] = rgb.Green;
		ledPanel[pos + 3] = rgb.Blue >> 4;
		ledPanel[pos + 4] = (rgb.Blue & 0x0F) << 4;
	}
	else
	{
		pos = pos / 8;
		ledPanel[pos + 1] = (rgb.Red & 0x001F);
		ledPanel[pos + 2] = (rgb.Green) >> 4;
		ledPanel[pos + 3] = ((rgb.Green & 0x000F) << 4);
		ledPanel[pos + 4] = rgb.Blue;
	}
}

//******************************************************************************
//basic in - out function
//******************************************************************************
/* non overlap */
void fanled_write_onecolor(uint32_t value,
                           uint16_t color,
                           uint16_t pos_to_write,
                           Display_Type *display)
{
	uint8_t i;
	for (i = 0; i < 32; i++)
	{
		if ( (value & (0x80000000 >> i)) != 0)
		{
			display->dis[pos_to_write][i] = color;
		}
	}
}

void fanled_write_onecolor_overlap(uint32_t value,
                                   uint16_t color,
                                   uint16_t pos_to_write,
                                   Display_Type *display)
{
	uint8_t i;
	for (i = 0; i < 32; i++)
	{
		if ( (value & (0x80000000 >> i)) != 0)
		{
			display->dis[pos_to_write][i] = color;
		}
		else
		{
			display->dis[pos_to_write][i] = 0;
		}
	}
}

void fanled_write_date(uint32_t value,
                       uint16_t color,
                       uint16_t pos_to_write,
                       Display_Type *display)
{
	uint8_t i;
	for (i = 6; i < 14; i++)
	{
		if ( (value & (0x80000000 >> i)) != 0)
		{
			display->dis[pos_to_write][i] = color;
		}
		else
		{
			display->dis[pos_to_write][i] = 0;
		}
	}
}

uint32_t fanled_read_onecolor(uint16_t pos_to_read,
                              Display_Type *display)
{
	uint8_t i;
	uint32_t temp_val = 0;
	for (i = 0; i < 32; i++)
	{
		if ( display->dis[pos_to_read][i] != 0)
		{
			temp_val |= (0x80000000 >> i);
		}
	}
	return temp_val;
}

//******************************************************************************
//dual printing function
//******************************************************************************
void fanled_write_dual(char byte,
                       uint16_t color,
                       uint16_t position_to_write,
                       uint16_t position_of_char,
                       Display_Type *display,
                       uint8_t row)
{
	if (row == OUTER_CIRCLE)
	{
		fanled_write_onecolor((byte << 24), color, DISPLAY_SIZE - position_to_write * 6 - position_of_char - 2, display);
	}
	if (row == INNER_CIRCLE)
	{
		fanled_write_onecolor((byte << 16), color, DISPLAY_SIZE - position_to_write * 14 - position_of_char - 2, display);
	}
}

void fanled_putc_dual(char c,
                      uint16_t color,
                      Display_Type *display,
                      unsigned char row)
{
	if (row == OUTER_CIRCLE)
	{
		if ((c >= 0x20) && (c <= 0x7f))
		{
			unsigned char i;
			c -= 0x20;
			for (i = 0; i < 6; i++)
				fanled_write_dual(ASCII[(unsigned char)c][i],
				                  color,
				                  display->pos_now_outer,
				                  i,
				                  display,
				                  OUTER_CIRCLE);
			if ((*display).pos_now_outer < DISPLAY_SIZE) { (*display).pos_now_outer++; }
		}
	}
	else if (row == INNER_CIRCLE)
	{
		if ((c >= 0x20) && (c <= 0x7f))
		{
			unsigned char i;
			c -= 0x20;
			for (i = 0; i < 12; i++)
				fanled_write_dual(ASCII[(unsigned char)c][i / 2],
				                  color,
				                  display->pos_now_inner,
				                  i,
				                  display,
				                  INNER_CIRCLE);
			fanled_write_dual(0x00, color, display->pos_now_inner, 12, display, INNER_CIRCLE);
			fanled_write_dual(0x00, color, display->pos_now_inner, 13, display, INNER_CIRCLE);
			if ((*display).pos_now_inner < DISPLAY_SIZE) { (*display).pos_now_inner++; }
		}
	}
}

//******************************************************************************
//basic interface function
//******************************************************************************
void fanled_write(char byte,
                  uint16_t color,
                  uint16_t position_to_write,
                  uint16_t position_of_char,
                  Display_Type *display)
{
	uint16_t 	temp = 0x0000;
	temp = fanled_read_onecolor(DISPLAY_SIZE - position_to_write * 6 - position_of_char - 2, display) & 0x00ffffff;
	fanled_write_onecolor((byte << 24) | temp, color, DISPLAY_SIZE - position_to_write * 6 - position_of_char - 2, display);
}

void fanled_putc(char c,
                 uint16_t color,
                 Display_Type *display)
{
	if ((c >= 0x20) && (c <= 0x7f))
	{
		unsigned char i;
		c -= 0x20;
		for (i = 0; i < 6; i++)
		{
			fanled_write(ASCII[(unsigned char)c][i],
			             color,
			             pos_now,
			             i,
			             display);
		}

		pos_now++;
	}
}

void fanled_puts(char *s,
                 uint16_t color,
                 Display_Type *display)
{
	while (*s)
	{
		fanled_putc(*s++, color, display);
	}
}

void fanled_puts_dual(char *s,
                      uint16_t color,
                      Display_Type *display,
                      unsigned char row)
{
	while (*s)
	{
		fanled_putc_dual(*s++, color, display, row);
	}
}

void fanled_goto_pos (uint16_t val,
                      uint8_t row,
                      Display_Type *display)
{
	/* inner circle 14section per character */
	if (row == INNER_CIRCLE)
	{
		if (val < DISPLAY_SIZE / 7) { (*display).pos_now_inner = val; }
	}
	/* outer circle 6section per character */
	/* character available for each display buffer */
	if (row == OUTER_CIRCLE)
	{
		if (val < DISPLAY_SIZE / 3) { (*display).pos_now_outer = val; }
	}
}

void fanled_puts_scroll(char *s,
                        uint16_t color,
                        Display_Type *display,
                        uint32_t speed)
{
	char i;
	int32_t j;
	char *s_temp;
	s_temp = s;

	if (display->enable_flag > JUST_RESET)
	{
		display->pos = 0;
		display->enable_flag = SCROLL_ENABLE_COUNTING;
	}

	if (display->enable_flag == SCROLL_ENABLE_COUNTING)
	{
		display->numOfchar = 0;
		while (*s)
		{
			display->numOfchar++;
			s++;
		}
		display->enable_flag = SCROLL_ENABLE_DISPLAY;
		display->pos_char = 0;
		display->section_of_char = 0;
		s = s_temp;
		display->scroll_times = speed;
	}

	if (display->enable_flag != SCROLL_ENABLE_DISPLAY)
	{
		return;
	}

	if (display->move_flag != 1)
	{
		return;
	}

	if (display->pos_char < display->numOfchar)
	{
		if (display->section_of_char <= 6)
		{
			for (j = DISPLAY_SIZE - 2; j >= 0; j--)
			{
				fanled_write_onecolor_overlap(fanled_read_onecolor(j, display), color, j + 1, display);
			}
			//temp = fanled_read_onecolor(0, display) & 0x00ffffff;
			i = ASCII[*(s + display->pos_char) - 0x20][display->section_of_char];
			//fanled_write_onecolor((i) << 24 | temp, color, 0, display);
			fanled_write_onecolor_overlap( (uint32_t)(i << 24), color, 0, display);
			display->section_of_char++;
		}
		else
		{
			display->section_of_char = 0;
			display->pos_char++;
		}
	}
	else if (display->pos_char < DISPLAY_SIZE / 6 + display->numOfchar)
	{
		if (display->section_of_char <= 6)
		{
			for (j = DISPLAY_SIZE - 2; j >= 0; j--)
			{
				fanled_write_onecolor_overlap(fanled_read_onecolor(j, display), color, j + 1, display);
			}
			i = ASCII[0][0];
			fanled_write_onecolor_overlap( (uint32_t)(i << 24), color, 0, display);
			display->section_of_char++;
		}
		else
		{
			display->section_of_char = 0;
			display->pos_char++;
		}
	}
	else
	{
		display->enable_flag = SCROLL_DISABLE_FINISH;
	}
	display->move_flag = 0;
}

void fanled_date_scroll(char *s,
                        uint16_t color,
                        Display_Type *display,
                        uint32_t speed)
{
	char i;
	uint32_t temp = 0x00;
	int32_t j;
	char *s_temp;
	s_temp = s;

	if (display->enable_flag > JUST_RESET)
	{
		display->pos = 0;
		display->enable_flag = SCROLL_ENABLE_COUNTING;
	}

	if (display->enable_flag == SCROLL_ENABLE_COUNTING)
	{
		display->numOfchar = 0;
		while (*s)
		{
			display->numOfchar++;
			s++;
		}
		display->enable_flag = SCROLL_ENABLE_DISPLAY;
		display->pos_char = 0;
		display->section_of_char = 0;
		s = s_temp;
		display->scroll_times = speed;
	}

	if (display->enable_flag != SCROLL_ENABLE_DISPLAY)
	{
		return;
	}

	if (display->move_flag != 1)
	{
		return;
	}

	if (display->pos_char < display->numOfchar)
	{
		if (display->section_of_char <= 6)
		{
			for (j = DISPLAY_SIZE - 2; j >= 0; j--)
			{
				fanled_write_date(fanled_read_onecolor(j, display), color, j + 1, display);
			}
			i = ASCII[*(s + display->pos_char) - 0x20][display->section_of_char];
			fanled_write_date( (uint32_t)(i << 18), color, 0, display);
			display->section_of_char++;
		}
		else
		{
			display->section_of_char = 0;
			display->pos_char++;
		}
	}
	else if (display->pos_char < DISPLAY_SIZE / 6 + display->numOfchar)
	{
		temp = fanled_read_onecolor(DISPLAY_SIZE - 1, display);
		for (j = DISPLAY_SIZE - 2; j >= 0; j--)
		{
			fanled_write_date(fanled_read_onecolor(j, display), color, j + 1, display);
		}
		fanled_write_date( temp, color, 0, display);
		display->section_of_char++;
	}
	else
	{
		display->enable_flag = SCROLL_DISABLE_FINISH;
	}
	display->move_flag = 0;
}

void clear_screen_buffer(Display_Type *fanleddata)
{
	uint16_t i, j;

	for (i = 0; i < FANLED_RESOLUTION; i++)
	{
		for (j = 0; j < 32; j++)
		{
			fanleddata->dis[i][j] = 0;
		}
	}
}

/************************* End of File ****************************************/

