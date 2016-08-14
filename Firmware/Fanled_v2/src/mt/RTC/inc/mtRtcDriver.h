/*
 * mtRtcDriver.h
 *
 *  Created on: Aug 10, 2016
 *      Author: zealot
 */

#ifndef MTRTCDRIVER_H_
#define MTRTCDRIVER_H_

#include <misc.h>
#include <stm32f10x_rtc.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_bkp.h>
#include <stdint.h>
#include <stdbool.h>

bool stmConfigureRTC(uint32_t countval);
uint32_t stmWaitForLastTask(void);
bool stmInitRTC(void);

#endif /* MTRTCDRIVER_H_ */
