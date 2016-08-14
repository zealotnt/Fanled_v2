#include "App/inc/SystemConfig.h"
#include "Porting/inc/mtTick.h"

#if (OPENCM3)
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>
#elif (STD_PERIPH_LIB)
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#endif/**
 * @defgroup mtInclude	Common header
 * @brief Common header file
 *
 * Define type, macro,... cor common use in project
 * @{
 */

#ifndef __MTINCLUDE_H__
#define __MTINCLUDE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "mtTypes.h"
#include "mtErrors.h"
#include "mtMacros.h"

#ifdef __cplusplus
}
#endif

#endif /*__MTINCLUDE_H__  */
/*@}*/
