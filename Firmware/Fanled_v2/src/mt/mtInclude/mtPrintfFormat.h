/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtPrintfFormat.h
**
** Module name       : mtInclude
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef _MT_PRINTF_FORMAT_H_
#define _MT_PRINTF_FORMAT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdio.h>
#include "mtCompileSwitch.h"

/*****************************************************************************/
/* DEFINITION OF COMPILE SWITCH                                              */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF CONSTANTS                                                   */
/*****************************************************************************/
/* Console color print */
#define KNRM  	"\033[0m"
#define KRED  	"\033[31m"
#define KGRN  	"\033[32m"
#define KYEL  	"\033[33m"
#define KBLU  	"\033[34m"
#define KMAG  	"\033[35m"
#define KCYN  	"\033[36m"
#define KWHT  	"\033[37m"

#define KLGRN	"\033[92m"
#define KLRED	"\033[91m"
#define KLYEL	"\033[93m"
#define KLBLU	"\033[94m"
#define KLMAG	"\033[95m"
#define KLCYN	"\033[96m"

/* Console style print */
#define KBOLD	"\033[1m"
#define KUDLN	"\033[4m"

/* Reset all settings of console print */
#define KRESET "\033[0m"
#define KRBOLD "\033[21m"

/*****************************************************************************/
/* DEFINITION OF TYPES                                                       */
/*****************************************************************************/


/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/
/* Header for module's debugging */
#if (DEBUG)
#define DEBUG_INFO(...)					printf(__VA_ARGS__)
#define DEBUG_ERROR(...)				printf(KRED KBOLD __VA_ARGS__); printf(KRESET "\r\n");
#else
#define DEBUG_INFO(...)
#define DEBUG_ERROR(...)
#endif

/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/


/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif

