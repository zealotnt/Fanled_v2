#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# fanled_api_sd.py

#---- IMPORTS
import serial
import struct
import binascii
import time
import datetime

from crc8 import crc8
from utils import *
from datalink_deliver import *

#---- CLASSES
class FanledAPISd():
	"""
	FanledAPISd class, calling Sd card API of Fanled
	"""
	def __init__(self, bluefin_serial):
		"""
		"""
		self._datalink = bluefin_serial
		self.pkt = BluefinserialCommand()
	def Ls(self):
		cmd = self.pkt.Packet('\x90', '\x10')
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of ls: ")

	def Inspect(self, name):
		cmd = self.pkt.Packet('\x90', '\x12')
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of inspect: ")

	def Read(self, name):
		cmd = self.pkt.Packet('\x90', '\x14')
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of read: ")

	def Delete(self, name):
		cmd = self.pkt.Packet('\x90', '\x16')
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of delete: ")

	def Write(self, name):
		cmd = self.pkt.Packet('\x90', '\x18')
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of write: ")
