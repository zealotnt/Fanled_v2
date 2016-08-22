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
DEFAULT_SERIAL_PORT = "/dev/ttyUSB0"

if __name__ == "__main__":

	return_code = 0

	parser = OptionParser()

	parser.add_option(  "-s", "--serial", 
						dest="serial", 
						type="string", 
						help="define the serial port to use")
	parser.add_option(  "-f", "--file", 
						dest="firmware", 
						type="string", 
						help="define the file path to firmware")

	(options, args) = parser.parse_args()

	if options.serial is not None:
		serial = options.serial
	else:
		serial = DEFAULT_SERIAL_PORT
		print_err("No serial port specified, use " + DEFAULT_SERIAL_PORT + " as default")

	if options.firmware is None:
		print_err("Please specified firmware file with -f flag")
		sys.exit(-1)

	file_path = options.firmware
	port_name = serial

	comm = BluefinserialSend(port_name, 460800)

	fanled_fw_upgrade = FanledAPIFwUpgrade(comm)

	fanled_fw_upgrade.UpgradeFirmware(file_path)
