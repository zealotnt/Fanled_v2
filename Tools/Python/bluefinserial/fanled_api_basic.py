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

def increase_str_by_one(value):
	int_val = ord(value)
	int_val += 1
	int_val &= 0xFF
	return chr(int_val)

def set_packet_id(id, buff):
	# buff[10] = chr(id & 0xFF)
	# buff[11] = chr((id & 0xFF00) >> 8)
	buff = buff[:6] + chr(id & 0xFF) + buff[6 + 1:]
	buff = buff[:7] + chr((id & 0xFF00) >> 8) + buff[7 + 1:]
	return buff

#---- CLASSES
class FanledAPIBasic():
	"""
	FanledAPIBasic class, implement basic API of Fanled
	"""
	def __init__(self, bluefin_serial):
		"""
		"""
		self._datalink = bluefin_serial
		self._cmd_len_backup = 0
		self._rsp_len_backup = 0
		self._send_buff_backup = ''
	def GetFirmwareVersion(self):
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x00', '\x03')
		rsp = ''

		start = time.clock()
		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			print_err("Get version fail")
			return None
		return rsp

	def ProtocolTest(self, packet_id, cmd_len, rsp_len):
		# Check input params
		if cmd_len < 12:
			raise Exception("Command len should be bigger than 12 (2b cmd_ctr code -- 4b cmd_len -- 4b rsp_len -- 2b packet_id")
		if cmd_len > 510:
			raise Exception("Command len should be less than or equal to 510 (protocol spec)")
		if rsp_len < 8:
			raise Exception("Expected response len should be bigger than 8 (2b Cmd + Ctr -- 4b rsp_len -- 2b packet_id)")
		if rsp_len > 510:
			raise Exception("Expected response len should be less than or equal to 510")

		# If cmd_len and rsp_len is the same, use cached data instead
		if cmd_len != self._cmd_len_backup or rsp_len != self._rsp_len_backup:
			self._cmd_len_backup = cmd_len
			self._rsp_len_backup = rsp_len
			# Prepare send buffer
			self._send_buff_backup = ''
			self._send_buff_backup = struct.pack('<II', cmd_len, rsp_len)

			idx = 0
			pos_val = '\x00'
			while idx < cmd_len:
				pos_val = increase_str_by_one(pos_val)
				self._send_buff_backup += pos_val
				idx += 1

		# Update packet_id
		pkt = BluefinserialCommand()
		self._send_buff_backup = set_packet_id(packet_id, self._send_buff_backup)
		cmd = pkt.Packet('\x8b', '\x70', self._send_buff_backup)

		# Send to target
		rsp = ''
		rsp = self._datalink.Exchange(cmd)
		return rsp
