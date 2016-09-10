/*
 * mtCompileSwitch.h
 *
 *  Created on: Aug 20, 2016
 *      Author: zealot
 */

#ifndef MTCOMPILESWITCH_H_
#define MTCOMPILESWITCH_H_

#ifndef FANLED_APP
#	define FANLED_APP					0
#endif

#ifndef FANLED_BOOTLOADER
#	define FANLED_BOOTLOADER			0
#endif

#ifndef DEBUG
#	if(FANLED_BOOTLOADER)
#		define DEBUG						0
#	elif(FANLED_APP)
#		define DEBUG						0
#	endif
#endif

#define WDT_ENABLE							0

#endif /* MTCOMPILESWITCH_H_ */
