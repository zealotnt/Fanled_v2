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
from datalink_deliver import *
from scan import scan
from utils import *


# ---- CONSTANTS

VERBOSE = 0

AUTHOR = u"zealotnt"

VERSION = "0.0.1"

PROG = "send_cmd"

COPYRIGHT = u"Copyright © 2016"

DEFAULT_SERIAL_PORT = "/dev/rfcomm0"

# ---- GLOBALS

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
		serial = DEFAULT_SERIAL_PORT
		print_err("No serial port specified, use " + DEFAULT_SERIAL_PORT + " as default")

	port_name = serial

	if options.verbose >= VERBOSE:
		print 'Open serial port: ' + port_name
	comm = BluefinserialSend(port_name, 460800)

	fanled_basic_api = FanledAPIBasic(comm)
	rsp = ''

	while True:
		start = time.clock()
		rsp = fanled_basic_api.GetFirmwareVersion()
		if rsp is None:
			print_err("Transmit fail")
			sys.exit(-1)
		end = time.clock()
		print "%.2gms" % ((end-start)*1000)

