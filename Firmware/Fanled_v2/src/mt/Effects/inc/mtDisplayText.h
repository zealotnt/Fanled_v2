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

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "../inc/mtFanledDisplay.h"

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
void updatePanel16b(uint16_t *input_color, bool blank);
void assignLed16b(uint16_t input, uint8_t num);

void fanled_puts(char *s, uint16_t color, Display_Type *display);
void fanled_puts_dual(char *s, uint16_t color, Display_Type *display, unsigned char row);
void fanled_puts_scroll(char *s, uint16_t color, Display_Type *display, uint32_t speed);
void fanled_date_scroll(char *s, uint16_t color, Display_Type *display, uint32_t speed);
void clear_screen_buffer(Display_Type *fanleddata);

#endif /* _DISPLAY_H_ */
