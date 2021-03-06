/*==============================================================================
**
**                      Proprietary - Copyright (C) 2016
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : GCC
**------------------------------------------------------------------------------
** File name         : mtRtc.h
**
** Module name       : RTC
**
**
** Summary:
**
**= History ====================================================================
** - Creation
** - Development
==============================================================================*/

#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUSIONS                                                                */
/*****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
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
typedef struct
{
	uint8_t   hh;
	uint8_t   mm;
	uint8_t   ss;
	uint16_t  year;
	uint8_t   month;
	uint8_t   day;
	uint8_t   weekday;
	uint8_t   yearday;
	uint8_t   isdst;
	bool isUpdated;
} sytemdate_t;

/*****************************************************************************/
/* DEFINITION OF MACROS                                                      */
/*****************************************************************************/
#define TIME_BUFFER_LEN	sizeof(sytemdate_t)

/*****************************************************************************/
/* DECLARATION OF VARIABLES (Only external global variables)                 */
/*****************************************************************************/
extern sytemdate_t sys_date;
extern bool        sys_nextday;

/*****************************************************************************/
/* DECLARATION OF GLOBALES FUNCTIONS (APIs, Callbacks & MainFunctions)       */
/*****************************************************************************/
UInt32 mtRtcGetUnixTime();
void mtRtcSetUnixTime(UInt32 value);

void printCurTime(void);
sytemdate_t *mtRtcGetSystemTime(void);
bool mtRtcSetSystemTime(sytemdate_t *time);

#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_H_ */
