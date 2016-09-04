/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtAmazingEffect.h
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

#ifndef _AMAZING_EFFECT_H_
#define _AMAZING_EFFECT_H_

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include "App/inc/mtAppDef.h"

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
void ColorWheelPrepare(Display_Type *FanledDisplay);
void Sharingan_effect(void);
void EightLight(void);

#endif /* _AMAZING_EFFECT_H_ */
