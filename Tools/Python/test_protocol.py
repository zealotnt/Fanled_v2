#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# datalink_deliver.py


# ---- IMPORTS
import os
import re
import time
import sys
import serial
import struct
from optparse import OptionParser, OptionGroup

sys.path.insert(0, 'bluefinserial')
from datalink_deliver import *
from fanled_api_basic import *
from scan import scan
from utils import *

# ---- CONSTANTS
USAGE = u"""Usage:
test_protocol <len-to-send> <len-expected-to-receive>
"""

# ---- GLOBALS

# Check input params
if len(sys.argv) != 3:
	print USAGE
	sys.exit(-1)
cmd_len = int(sys.argv[1])
rsp_len = int(sys.argv[2])

# Open port
comm = BluefinserialSend(BLUEFINSERIAL_DEFAULT_SERIAL_PORT, BLUEFINSERIAL_BAUDRATE)
fanled_basic_api = FanledAPIBasic(comm)

# Begin test
times = 1
while True:
	rsp = ''
	# dump_hex(cmd, "Command: ")
	start = float((time.time() * 1000))
	rsp = fanled_basic_api.ProtocolTest(times, cmd_len, rsp_len)
	if rsp is None:
		print_err("Protocol test fail")
		sys.exit(-1)

	# dump_hex(rsp, "Response: ")
	end = float((time.time() * 1000))
	print "Packet %d sent, %.2f ms" % (times, (end-start))
	times += 1

