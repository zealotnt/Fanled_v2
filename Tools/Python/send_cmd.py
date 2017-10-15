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
import readline, glob

sys.path.insert(0, 'bluefinserial')
from fanled_api_basic import *
from fanled_api_fw_upgrade import *
from fanled_api_sd import *
from datalink_deliver import *
from scan import scan
from utils import *


# ---- CONSTANTS

VERBOSE = 0

AUTHOR = u"zealotnt"

VERSION = "0.0.1"

PROG = "send_cmd"

COPYRIGHT = u"Copyright © 2016"

APP_PROMT = u"""
Application support:
	basic | 1        : basic fanled api
	upg   | 2        : fanled firmware upgrade api
	sd    | 3        : fanled sd card api
"""

# ---- GLOBALS
def complete(text, state):
	return (glob.glob(text+'*')+[None])[state]

def string_function_parse(argument):
	switcher = {
		"basic": basic_api,
		"1"    : basic_api,
		"upg"  : fw_upg_api,
		"2"    : fw_upg_api,
		"sd"   : fw_sd_api,
		"3"    : fw_sd_api,
	}
	# Get the function from switcher dictionary
	func = switcher.get(argument, lambda: ("%s not supported" % argument))
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
- dbg      : Get debug var from Fanled
"""
	last_valid_key = ""
	while True:
		user_promt = raw_input(PROMT)
		# If user use up arrow, re-issue last command
		if user_promt == '\x1b[A':
			user_promt = last_valid_key

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
		elif user_promt == "dbg":
			fanled_basic_api.GetDbgVar()
		elif user_promt == "b" or user_promt == "back":
			return
		else:
			print HELP
			continue
		last_valid_key = user_promt

def fw_upg_api():
	# Fw upgrade test suite
	print_ok("Firmware upgrade application selected")
	PROMT = u"Input your command:"
	HELP = u"""
Support comand:
- erase     | e : erase firmware flash
- write     | w : continue write firmware content to target device
- write all | wa: write all firmware content to target device
- checksum  | ck: download checksum to board
- reset     | r : reset file and index sequence
- up        | u : request app to upgrade firmware, jump back to bootloader
- le            : Get fanled bootloader last err
- back      | b : return to main menu
"""
	FW_PATH = "/home/zealot/workspace_NotCategorized/Fanled_v2/Firmware/Fanled_v2/Build-App/Fanled_v2.bin"
	fw = open(FW_PATH, "rb")
	fw_max_packet = GetNumOfPacket(FW_PATH)
	fw_contents = GetFileContent(FW_PATH)
	fw_idx = 0
	while True:
		user_promt = raw_input(PROMT)
		if user_promt == "erase" or user_promt == "e":
			fanled_fw_upgrade.EraseFirmwareRequest("app")
		elif user_promt == "write" or user_promt == "w":
			print "Writing firmware index " + str(fw_idx)
			first_idx = fw_idx * PACKET_MAX_SIZE
			end_idx = ((fw_idx + 1) * PACKET_MAX_SIZE)
			fanled_fw_upgrade.WriteFirmwarePacket("app", fw_idx, PACKET_MAX_SIZE, fw_contents[first_idx:end_idx])
			fw_idx += 1
		elif user_promt == "write all" or user_promt == "wa":
			fanled_fw_upgrade.DownloadFirmware("app", FW_PATH)
		elif user_promt == "checksum" or user_promt == "ck":
			fanled_fw_upgrade.DownloadChecksum("app", FW_PATH)
		elif user_promt == "reset" or user_promt == "r":
			fw_idx = 0
		elif user_promt == "up" or user_promt == "u":
			fanled_fw_upgrade.UpgradeRequest("app")
		elif user_promt == "le":
			fanled_fw_upgrade.GetLastErr()
		elif user_promt == "b" or user_promt == "back":
			return
		else:
			print HELP

def fw_sd_api():
	# Fw sd test suite
	print_ok("Sd card application selected")
	PROMT = u"Input your command:"
	HELP = u"""
Support comand:
- ver      : Get firmware version
- le       : Get fanled bootloader last err
- ls       : List file
- ins      : Inspect file
- r        : Read file's value
- wr       : Write raw value to file
- wf       : Write a file in local system to target system
- rm       : Delete file
- md5      : Calculate md5 of file in target system
"""
	last_valid_key = ""
	while True:
		user_promt = raw_input(PROMT)
		# If user use up arrow, re-issue last command
		if user_promt == '\x1b[A':
			user_promt = last_valid_key

		if user_promt == "ver":
			fanled_basic_api.GetFirmwareVersion()
		elif user_promt == "le":
			fanled_fw_upgrade.GetLastErr()
		elif user_promt == "ls":
			list_files = fanled_sd_api.Ls()
			print list_files
		elif user_promt == "ins":
			fanled_sd_api.Inspect("abc")
		elif user_promt == "r":
			file_name = raw_input("File to read: ")
			contents = fanled_sd_api.Read(file_name)
			print contents
		elif user_promt == "wr":
			file_name = raw_input("File to write to target device: ")
			file_content = raw_input("Content to write: ")
			fanled_sd_api.WriteRaw(file_name, file_content)
		elif user_promt == "wf":
			file_name = raw_input("File to write to target device: ")
			file_content = raw_input("Path to local file: ")
			fanled_sd_api.WriteFile(file_name, file_content)
		elif user_promt == "rm":
			file_name = raw_input("File to delete: ")
			fanled_sd_api.Delete(file_name)
		elif user_promt == "md5":
			file_name = raw_input("File to checksum: ")
			checksum = fanled_sd_api.GetFileMd5(file_name)
			dump_hex(checksum, "Checksum = ")
		elif user_promt == "b" or user_promt == "back":
			return
		else:
			print "Wrong command, try again:"
			print HELP
			continue
		last_valid_key = user_promt
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
	fanled_sd_api = FanledAPISd(comm)

	# Auto complete setting for user input
	readline.set_completer_delims(' \t\n;')
	readline.parse_and_bind("tab: complete")
	readline.set_completer(complete)

	while True:
		print APP_PROMT
		app_selected = raw_input("Input your application: ")
		string_function_parse(app_selected)
