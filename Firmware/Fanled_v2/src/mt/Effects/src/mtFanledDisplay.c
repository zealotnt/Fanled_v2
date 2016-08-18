/*
 * mtFanledDisplay.c
 *
 *  Created on: Aug 18, 2016
 *      Author: zealot
 */
#include "../inc/mtFanledDisplay.h"
#include "../inc/mtDisplayText.h"
#include "Utility/inc/mtDelay.h"
#include "Porting/inc/mtSPI.h"
#include <stdbool.h>
#include <stdint.h>

Display_Type Fanled_Display;
uint8_t ledPanel[36*4];

void mtFanledSendLineBuffer()
{
	mtSPIFanledSend((uint32_t)ledPanel, 36*4);
}

void blankAllLed(void)
{
	updatePanel16b(0, true);
	mtFanledSendLineBuffer();
	mtDelayMS(30);
	LED_LATCH();
	LED_UNBLANK();
}
