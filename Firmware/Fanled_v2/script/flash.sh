#!/bin/bash

OPENOCD_DIR=/home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev
OPENOCD=$OPENOCD_DIR/bin/openocd
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
HEX_OUT=$DIR/../Debug/Fanled_v2.hex

$OPENOCD 	-s {$OPENOCD_DIR}\
		   	-f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/interface/jlink_swd.cfg\
			-f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/target/stm32f1x.cfg\
	        -c init -c targets -c "reset halt" \
	        -c "flash write_image erase $HEX_OUT" \
	        -c "verify_image $HEX_OUT" \
	        -c "reset run" -c shutdown
