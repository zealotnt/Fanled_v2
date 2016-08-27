/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : SystemConfig.h
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

#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

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
void blankAllLed(void);

void initBootloader();
void initAll();

void mtSdCardInit(void);
void mtSysTickInit();
void mtRCCInit();
void mtDelayMS(uint32_t time_delay);
void mtHallSensorInit();
void mtHallSensorDeinit();
void mtTimerFanledDisplayInit();
void mtTimerFanledDisplayDisable();

void mtHC05KeyPinInit();
void mtBluetoothUSARTInit(bool config);
void mtBluetoothUSARTChangeBaud(uint32_t baudrate);


#endif /* _SYSTEM_CONFIG_H_ */
