#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# datalink_deliver.py


#---- IMPORTS
import serial
import struct
import binascii
import time

from crc8 import crc8
from utils import *
from datalink_deliver import *



#---- CLASSES
class FanledAPIFwUpgrade():
	"""
	FanledAPIFwUpgrade class, implement firmware upgrade API of Fanled
	"""
	def __init__(self, bluefin_serial):
		"""
		"""
		self._datalink = bluefin_serial

	def UpgradeRequest(self):
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x30')
		rsp = ''

		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			print_err("Firmware upgrade request fail")
			return None
		return rsp