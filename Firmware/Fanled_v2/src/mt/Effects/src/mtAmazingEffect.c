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
#include <math.h>
#include "../inc/mtIncludeEffects.h"
#include "App/inc/mtAppDef.h"

/******************************************************************************/
/* LOCAL CONSTANT AND COMPILE SWITCH SECTION                                  */
/******************************************************************************/


/******************************************************************************/
/* LOCAL TYPE DEFINITION SECTION                                              */
/******************************************************************************/


/******************************************************************************/
/* LOCAL MACRO DEFINITION SECTION                                             */
/******************************************************************************/


/******************************************************************************/
/* MODULE'S LOCAL VARIABLE DEFINITION SECTION                                 */
/******************************************************************************/
extern Display_Type Fanled_Display;


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

/*----------------------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------------------
*	Description: 	These functions show a HSV wheel effect
*					Call ColorWheelPrepare for drawing full circle
*----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------*/
double module_percent_double(double num1, double num2)
{
	double temp;
	uint32_t temp_u32;
	temp = num1 / num2;
	temp_u32 = (uint32_t)temp;
	return (num1 - num2 * temp_u32);
}

uint16_t rgb_24_to_16(rgb_t input)
{
	/*
	 * Formular convert from RGB16 to RGB24:
	 * rgb.Red = (input & 0xF800) >> 11;
	 * rgb.Green = ((input & 0x07E0) >> 5) & 0x3f;
	 * rgb.Blue = (input & 0x1f);
	 * */
	return ( ((input.Red & 0xF8) << 7) + ((input.Green & 0xFC) << 3) + (input.Blue >> 3) );	//R:5 G:6 B:5
}

rgb_t HSV_to_Color(int32_t H, int32_t S, int32_t V)
{
	/*
	 * HSV contains values scaled as in the color wheel:
	 * that is, all from 0 to 255.
	 * */

	/*
	 * for ( this code to work, HSV.Hue needs
	 * to be scaled from 0 to 360 (it//s the angle of the selected
	 * point within the circle). HSV.Saturation and HSV.value must be
	 * scaled to be between 0 and 1.
	 * */
	double h;
	double s;
	double v;

	double r = 0;
	double g = 0;
	double b = 0;
	rgb_t temp_rgb;
	/* Scale Hue to be between 0 and 360. Saturation */
	/* and value scale to be between 0 and 1. */
	/* h = ((double) H / 255 * 360) % 360; c# code */
	h = module_percent_double(((double) H / 255 * 360), 360);
	s = (double) S / 255;
	v = (double) V / 255;
	if ( s == 0 )
	{
		/* If s is 0, all colors are the same. */
		/* This is some flavor of gray. */
		r = v;
		g = v;
		b = v;
	}
	else
	{
		double p;
		double q;
		double t;

		double fractionalSector;
		int sectorNumber;
		double sectorPos;

		/* The color wheel consists of 6 sectors. */
		/* Figure out which sector you//re in. */
		sectorPos = h / 60;
		sectorNumber = (int)(floor(sectorPos));
		/* get the fractional part of the sector. */
		/* That is, how many degrees into the sector */
		/* are you? */
		fractionalSector = sectorPos - sectorNumber;

		/* Calculate values for the three axes */
		/* of the color. */
		p = v * (1 - s);
		q = v * (1 - (s * fractionalSector));
		t = v * (1 - (s * (1 - fractionalSector)));

		/* Assign the fractional colors to r, g, and b */
		/* based on the sector the angle is in. */
		switch (sectorNumber)
		{
			case 0:
				r = v;
				g = t;
				b = p;
				break;

			case 1:
				r = q;
				g = v;
				b = p;
				break;

			case 2:
				r = p;
				g = v;
				b = t;
				break;

			case 3:
				r = p;
				g = q;
				b = v;
				break;

			case 4:
				r = t;
				g = p;
				b = v;
				break;

			case 5:
				r = v;
				g = p;
				b = q;
				break;
		}
	}
	temp_rgb.Red = r * 255;
	temp_rgb.Green = g * 225;			//scale down green color		default: 255
	temp_rgb.Blue = b * 255;
	/* return an RGB structure, with values scaled */
	/* to be between 0 and 255. */
	return temp_rgb;
}

void ColorWheelPrepare_1(Display_Type *FanledDisplay)			// 1st version
{
	uint32_t i;
	rgb_t temp_rgb;
	for (i = 0; i < FANLED_RESOLUTION; i++)
	{
		temp_rgb = HSV_to_Color((int)((double)(i * 255) / FANLED_RESOLUTION), 255, 255);
		FanledDisplay->dis[i][0] = rgb_24_to_16(temp_rgb);
	}
}

uint16_t offset_color(uint16_t input, uint16_t offset)
{
	input += offset;
	if (input > 255) { input = 255; }
	return input;
}

/* version that draw full circle */
void ColorWheelPrepare(Display_Type *FanledDisplay)
{
	uint32_t i, j;
	rgb_t temp_rgb;
	rgb_t rgb_array[32];
	for (i = 0; i < FANLED_RESOLUTION; i++)
	{
		temp_rgb = HSV_to_Color((int)((double)(i * 255) / FANLED_RESOLUTION), 255, 255);
		for (j = 1; j < 32; j++)
		{
			rgb_array[j].Red = offset_color(temp_rgb.Red, 6 * j); //8 == 255/32
			rgb_array[j].Green = offset_color(temp_rgb.Green, 2 * j);
			rgb_array[j].Blue = offset_color(temp_rgb.Blue, 6 * j);
			FanledDisplay->dis[i][j] = rgb_24_to_16(rgb_array[j]);
		}
		FanledDisplay->dis[i][0] = rgb_24_to_16(temp_rgb);
	}
}


/*----------------------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------------------
*	Description: 	These functions show a sharingan effect :v
*	Requirement:	This function must work with timing function (systick)
*					and set value to Fanled_Display.sharingan_count (already done in Sharingan_effect)
*					So just call Sharingan_effect in main handler
*----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------*/
void Ring_set_data(uint16_t color, uint32_t pos_start_ring, uint32_t pos_radius_ring)
{
	uint32_t i;
	/* Not accept data out of range */
	if (pos_start_ring < FANLED_RESOLUTION )
	{
		/* Set color for ring */
		for (i = pos_start_ring; i < (RING_SIZE + pos_start_ring); i++)
		{
			Fanled_Display.dis[i % FANLED_RESOLUTION][pos_radius_ring] = color;
		}
		/* Remove color for region not the ring */
		if ((pos_start_ring + RING_SIZE) > FANLED_RESOLUTION)
		{
			for (i = ((RING_SIZE + pos_start_ring) % FANLED_RESOLUTION); i < (pos_start_ring - 1); i++)
			{
				Fanled_Display.dis[i][pos_radius_ring] = 0;
			}
		}
		else
		{
			for (i = 0; i < pos_start_ring; i++)
			{
				Fanled_Display.dis[i][pos_radius_ring] = 0;
			}
			for (i = pos_start_ring + RING_SIZE; i < FANLED_RESOLUTION; i++)
			{
				Fanled_Display.dis[i][pos_radius_ring] = 0;
			}
		}
	}
}

void Ring_set_all(uint16_t color, uint32_t offset)
{
	uint32_t i;
	for (i = 0; i < 32; i++)
	{
		Ring_set_data(color, (offset + FANLED_RESOLUTION / 9 * i) % FANLED_RESOLUTION, i);
	}
}

uint8_t Ring_set_all_mod(uint16_t color, uint32_t offset)
{
	static uint8_t pos = 0;
	Ring_set_data(color, (offset + FANLED_RESOLUTION / 9 * pos) % FANLED_RESOLUTION, pos);
	pos++;
	if (pos == 32)
	{
		pos = 0;
		return 1;
	}
	return 0;
}

/****************************************************************************
*	Function name: 	Sharingan_effect
*	Description: 	This function show a sharingan effect :v
*	Requirement:	This function must work with timing function (systick)
*					and set value to Fanled_Display.sharingan_count
*/
void Sharingan_effect(void)			//condition to start
{
	static uint32_t ring_pos = 0;
	if (Fanled_Display.sharingan_flag == STATE_ENABLE)
	{
		if (Ring_set_all_mod(COLOR_RED_MAX, ring_pos % FANLED_RESOLUTION))
		{
			ring_pos -= 30;
			Fanled_Display.sharingan_flag = STATE_DISABLE;
		}
	}
}



/*----------------------------------------------------------------------------------------------------------------------------
*-----------------------------------------------------------------------------------------------------------------------------
*	Description: 	These functions show a Eight Light effect :v
*	Requirement:	This function must work with timing function (systick)
*					...	(add more description here)
*					... (add more description here)
*----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------*/

void EightLightMachine(uint16_t color)
{
	static uint16_t s_state = 0;
	uint32_t i, j;
	/* Init the first view (only one outer circle) */
	if (s_state == 0)
	{
		for (i = 0; i < FANLED_RESOLUTION; i++)
		{
			/* Set outer circle color value*/
			Fanled_Display.dis[i][0] = color;
			/* And remove all the remaining color of fanled*/
			for (j = 1; j < 32; j++)
			{
				Fanled_Display.dis[i][j] = 0;
			}
		}
		s_state++;
	}
	/* Draw circle and remove point at the same time */
	else if ( (s_state > 0) && (s_state < 29) )
	{
		/* Set value for new update circle (along radius) */
		for (i = 0; i < 8; i++)
		{
			for (j = s_state + 1; j < 30; j++)
			{
				Fanled_Display.dis[i * 30 + j][s_state] = color;
			}
		}

		/* Remove unneeded value in same circle */
		/* Remove each circle (from outer to the present inner ON circle) */
		for (i = 0; i < s_state; i++)
		{
			/* Search for item that need to be deleted */
			for (j = 0; j < 8; j++)
			{
				/* Fanled_Display.dis[j*30+s_state][i] = 0;	--> only this, the left image is like a shuriken */
				/* Remove target item */
				Fanled_Display.dis[j * 30 + s_state - 1][i] = 0;
				Fanled_Display.dis[j * 30 + s_state][i] = 0;
			}
		}
		s_state++;
	}
	/* Draw the left point */
	else if ( (s_state >= 29) && (s_state < 32) )
	{
		for (i = 0; i < 8; i++)
		{
			Fanled_Display.dis[i * 30 + 29][s_state] = color;
		}
		s_state++;
	}
	else if (s_state >= 32 && s_state < 64)
	{
		for (i = 0; i < 8; i++)
		{
			Fanled_Display.dis[i * 30 + 29][s_state % 32] = 0;
		}
		s_state++;
	}
	else if ( s_state < 70)
	{
		s_state++;
	}
	else
	{
		s_state = 0;
	}
}

void EightLight(void)
{
	if ( Fanled_Display.misc_flag == STATE_ENABLE)
	{
		EightLightMachine(COLOR_RED_MAX);
		Fanled_Display.misc_flag = STATE_DISABLE;
	}
}

/************************* End of File ****************************************/
