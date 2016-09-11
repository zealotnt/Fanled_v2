#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# send_cmd.py


# ---- IMPORTS

import os
import re
import time
import sys
import serial
import struct
from optparse import OptionParser, OptionGroup

sys.path.insert(0, 'bluefinserial')
from fanled_api_basic import *
from fanled_api_fw_upgrade import *
from datalink_deliver import *
from scan import scan
from utils import *


# ---- CONSTANTS
DEFAULT_FIRMWARE_APP_FILE = "../../Firmware/Fanled_v2/Build-App/Fanled_v2.bin"
DEFAULT_FIRMWARE_BL_FILE = "../../Firmware/Fanled_v2/Build-Bl/Fanled_v2.bin"

if __name__ == "__main__":

	return_code = 0

	parser = OptionParser()

	parser.add_option(  "-s", "--serial",
						dest="serial",
						type="string",
						help="define the serial port to use")
	parser.add_option(  "-f", "--file",
						dest="firmware_file",
						type="string",
						help="define the file path to firmware")
	parser.add_option(  "-t", "--type",
						dest="firmware_type",
						type="string",
						help="define the firmware type to upgrade")
	(options, args) = parser.parse_args()

	if options.serial is not None:
		serial = options.serial
	else:
		serial = BLUEFINSERIAL_DEFAULT_SERIAL_PORT
		print_ok("No serial port specified, use " +
			BLUEFINSERIAL_DEFAULT_SERIAL_PORT +
			" with baudrate = " +
			str(BLUEFINSERIAL_BAUDRATE) +
			" as default")

	if options.firmware_type is None:
		print_err("Please specify firmware_type by -t or --type flag")
		sys.exit(-1)

	if options.firmware_type == "app" or options.firmware_type == "2":
		file = DEFAULT_FIRMWARE_APP_FILE
		options.firmware_type = "app"
	elif options.firmware_type == "bl" or options.firmware_type == "1":
		file = DEFAULT_FIRMWARE_BL_FILE
		options.firmware_type = "bl"
	else:
		print_err("Invalid firmware_type")
		sys.exit(-2)

	if options.firmware_file is not None:
		file = options.firmware_file

	file_path = file
	port_name = serial
	file_type = options.firmware_type

	comm = BluefinserialSend(port_name, BLUEFINSERIAL_BAUDRATE)

	fanled_fw_upgrade = FanledAPIFwUpgrade(comm)

	fanled_fw_upgrade.UpgradeFirmware(file_type, file_path)
