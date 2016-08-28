#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# datalink_deliver.py


#---- IMPORTS
import serial
import struct
import binascii
import time
import datetime

from crc8 import crc8
from utils import *
from datalink_deliver import *

def GetUnixTime():
	return int(time.time())

def GetDateString(unix_time):
	return datetime.datetime.fromtimestamp(unix_time).strftime('%Y-%m-%d %H:%M:%S')

def increase_str_by_one(value):
	int_val = ord(value)
	int_val += 1
	int_val &= 0xFF
	return chr(int_val)

def set_packet_id(id, buff):
	# buff[6] = chr(id & 0xFF)
	# buff[7] = chr((id & 0xFF00) >> 8)
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

	def GetFanledUnixTime(self):
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x02', '\x00')

		# Send to target
		rsp = ''
		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			return 0
		if len(rsp) != 7:
			print_err("Len response not expected")
			return 0

		if rsp[2] == '\x00':
			return ord(rsp[3]) + (ord(rsp[4]) << 8) + (ord(rsp[5]) << 16) + (ord(rsp[6]) << 24)
		else:
			print_err("Can't get Unix time from Fanled")
			return 0

	def SetFanledUnixTime(self, set_value):
		cmd_data = struct.pack('<BL', 0x01, set_value)
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x02', cmd_data)

		# Send to target
		rsp = ''
		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			return False
		if len(rsp) < 3:
			print_err("Len response not expected")
			return False
		if rsp[2] == '\x00':
			print_ok("Set time ok")
			return True
		else:
			print_err("Can't get Unix time from Fanled")
			return False

	def MakeFanledHardfault(self):
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x72')
		rsp = self._datalink.Exchange(cmd)

