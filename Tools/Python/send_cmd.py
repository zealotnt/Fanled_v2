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

VERBOSE = 0

AUTHOR = u"zealotnt"

VERSION = "0.0.1"

PROG = "send_cmd"

COPYRIGHT = u"Copyright © 2016"

# ---- GLOBALS
def string_function_parse(argument):
	switcher = {
		"basic": basic_api,
		"1"    : basic_api,
		"upg"  : fw_upg_api,
		"2"    : fw_upg_api,
	}
	# Get the function from switcher dictionary
	func = switcher.get(argument, lambda: "not found")
	# Execute the function
	return func()

def basic_api():
	# fanled_basic_api test suite
	print_ok("Basic api application selected")
	PROMT = u"Input your command:"
	HELP = u"""
Support comand:
- gt       : get rtc value
- st       : set rtc value
- ver      : get firmware version
- hf       : cause device to hardfault (for WDT testing)
- le       : Get fanled bootloader last err
- e_bl     : Jump back from application to bootloader
"""

	while True:
		user_promt = raw_input(PROMT)
		if user_promt == "gt":
			rtc_get_val = fanled_basic_api.GetFanledUnixTime()
			print "rtc return from fanled: " + str(rtc_get_val)
			print "Date format: " + GetDateString(rtc_get_val)
		elif user_promt == "st":
			value_to_set = raw_input("Value to set: ")
			fanled_basic_api.SetFanledUnixTime(int(value_to_set))
		elif user_promt == "ver":
			fanled_basic_api.GetFirmwareVersion()
		elif user_promt == "hf":
			fanled_basic_api.MakeFanledHardfault()
		elif user_promt == "le":
			fanled_fw_upgrade.GetLastErr()
		elif user_promt == "e_bl":
			fanled_fw_upgrade.UpgradeRequest()
		else:
			print HELP	

def fw_upg_api():
	# Fw upgrade test suite
	print_ok("Firmware upgrade application selected")
	PROMT = u"Input your command:"
	HELP = u"""
Support comand:
- erase | e: erase firmware flash
- up    | u: request app to upgrade firmware, jump back to bootloader
- write | w: continue write firmware content to target device
- reset | r: reset file and index sequence
- le       : Get fanled bootloader last err
"""
	FW_PATH = "/home/zealot/workspace_NotCategorized/Fanled_v2/Firmware/Fanled_v2/Build-App/Fanled_v2.bin"
	fw = open(FW_PATH, "rb")
	fw_max_packet = GetNumOfPacket(FW_PATH)
	fw_contents = GetFileContent(FW_PATH)
	fw_idx = 0
	while True:
		user_promt = raw_input(PROMT)
		if user_promt == "erase" or user_promt == "e":
			fanled_fw_upgrade.EraseAppFwRequest()
		elif user_promt == "write" or user_promt == "w":
			print "Writing firmware index " + str(fw_idx)
			first_idx = fw_idx * PACKET_MAX_SIZE
			end_idx = ((fw_idx + 1) * PACKET_MAX_SIZE)
			fanled_fw_upgrade.WriteFirmwarePacket(fw_idx, PACKET_MAX_SIZE, fw_contents[first_idx:end_idx])
			fw_idx += 1
		elif user_promt == "write all" or user_promt == "wa":
			fanled_fw_upgrade.DownloadFirmware(FW_PATH)
		elif user_promt == "checksum" or user_promt == "ck":
			fanled_fw_upgrade.DownloadChecksum(FW_PATH)
		elif user_promt == "reset" or user_promt == "r":
			fw_idx = 0
		elif user_promt == "up" or user_promt == "u":
			fanled_fw_upgrade.UpgradeRequest()
		elif user_promt == "le":
			fanled_fw_upgrade.GetLastErr()			
		else:
			print HELP

# ---- MAIN

if __name__ == "__main__":

	return_code = 0

	parser = OptionParser()

	parser.add_option(  "-s", "--serial", 
						dest="serial", 
						type="string", 
						help="define the serial port to use")
	parser.add_option(  "-v", "--verbose", 
						action="count", 
						dest="verbose", 
						help="enable verbose mode")
	parser.add_option(  "-l", "--list-serial", 
						action="store_true", 
						dest="list_serial", 
						default=False, 
						help="display available serial ports")
	parser.add_option(  "-a", "--app-select", 
						dest="app_selected",
						type="string", 
						help="application to select")

	(options, args) = parser.parse_args()

	if options.list_serial:
		print "Available serial ports:"
		for port_name in scan():
			print '  - ' + port_name
		sys.exit(0)

	if options.serial is not None:
		serial = options.serial
	else:
		serial = BLUEFINSERIAL_DEFAULT_SERIAL_PORT
		print_ok("No serial port specified, use " + BLUEFINSERIAL_DEFAULT_SERIAL_PORT + " as default")

	port_name = serial

	if options.verbose >= VERBOSE:
		print 'Open serial port: ' + port_name
	comm = BluefinserialSend(port_name, BLUEFINSERIAL_BAUDRATE)

	fanled_basic_api = FanledAPIBasic(comm)
	fanled_fw_upgrade = FanledAPIFwUpgrade(comm)

	APP_PROMT = u"""No application selected !!!
To choose application:
	./send_cmd.py -a <app-select>
Application support:
	basic | 1        : basic fanled api
	upg   | 2        : fanled firmware upgrade api
"""
	if options.app_selected is None:
		print APP_PROMT
		sys.exit(1)

	string_function_parse(options.app_selected)
