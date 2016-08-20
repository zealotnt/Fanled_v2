#!/bin/bash

case $# in
1)
	TARGET=$1
	;;
*)
	echo "Wrong argument"
	exit 1
	;;
esac

OPENOCD_DIR=/home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev
OPENOCD=$OPENOCD_DIR/bin/openocd
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ $TARGET -eq 1 ]; then
	echo "Flashing Fanled Bootloader"
	HEX_OUT=$DIR/../Build-Bl/Fanled_v2.hex
	$OPENOCD 	-s {$OPENOCD_DIR}\
			   	-f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/interface/jlink_swd.cfg\
				-f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/target/stm32f1x.cfg\
		        -c init -c targets -c "reset halt" \
		        -c "flash write_image erase $HEX_OUT" \
		        -c "verify_image $HEX_OUT" \
		        -c "reset run" -c shutdown
	echo "Done flashing Fanled Bootloader"
else
	echo "Flashing Fanled App"
	HEX_OUT=$DIR/../Build-App/Fanled_v2.hex
	$OPENOCD 	-s {$OPENOCD_DIR}\
			   	-f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/interface/jlink_swd.cfg\
				-f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/target/stm32f1x.cfg\
		        -c init -c targets -c "reset halt" \
		        -c "flash write_image erase unlock $HEX_OUT" \
		        -c "verify_image $HEX_OUT" \
		        -c "reset run" -c shutdown
	echo "Done flashing Fanled App"
fi

#/home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/bin/openocd -s /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev -f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/interface/jlink_swd.cfg -f /home/zealot/bin/openocd-gnu-arm-eclipse/0.10.0-201601101000-dev/scripts/target/stm32f1x.cfg -c init -c targets -c "reset halt" -c "flash write_image erase unlock /home/zealot/workspace_NotCategorized/Fanled_v2/Firmware/Fanled_v2/Build-App/Fanled_v2.hex" -c "reset run" -c shutdown