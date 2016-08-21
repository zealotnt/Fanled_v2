#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# datalink_deliver.py
# [CRC32 online tool](http://www.sunshine2k.de/coding/javascript/crc/crc_js.html)

#---- IMPORTS
import os
import serial
import struct
import binascii
import time

from crc8 import crc8
from utils import *
from datalink_deliver import *

PACKET_MAX_SIZE = 480

def GetFileSize(path):
	return os.path.getsize(path)

def GetNumOfPacket(path):
	file_size = GetFileSize(path)
	if file_size % PACKET_MAX_SIZE == 0:
		return (file_size / PACKET_MAX_SIZE)
	else:
		return ((file_size / PACKET_MAX_SIZE) + 1)

def GetFileContent(path):
	f = open(path, 'rb')
	return f.read()

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

	def EraseAppFwRequest(self):
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x36')
		rsp = ''

		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			print_err("Firmware erase request fail")
			return None
		return rsp

	def WriteFirmwarePacket(self, packet_number, packet_len, packet_data):
		if packet_len > PACKET_MAX_SIZE:
			raise Exception("Packet len should be less than " + str(PACKET_MAX_SIZE))
		if len(packet_data) > (PACKET_MAX_SIZE * 2):
			raise Exception("Packet data size should be less than " + str(PACKET_MAX_SIZE))

		# Build download_packet
		download_packet = struct.pack('<HH', packet_number, packet_len) + packet_data
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x32', download_packet)
		rsp = ''

		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			print_err("Firmware write request fail")
			return None
		print_ok("Firmware write packet num " + str(packet_number) + " successfully")
		return rsp

	def DownloadFirmware(self, file_path):
		fw = open(file_path, "rb")
		fw_max_packet = GetNumOfPacket(file_path)
		fw_contents = GetFileContent(file_path)
		fw_idx = 0

		while fw_idx <= fw_max_packet:
			if fw_idx != fw_max_packet:
				first_idx = fw_idx * PACKET_MAX_SIZE
				end_idx = ((fw_idx + 1) * PACKET_MAX_SIZE)
				ret = self.WriteFirmwarePacket(fw_idx, PACKET_MAX_SIZE, fw_contents[first_idx:end_idx])
			else:
				first_idx = fw_idx * PACKET_MAX_SIZE
				ret = self.WriteFirmwarePacket(fw_idx, PACKET_MAX_SIZE, fw_contents[first_idx:])
			if ret is None:
				print_err("Firmware donwload error")
				return False

			fw_idx += 1
		# Upgrade successfully
		print_ok("Firmware download successfully")
		return True

	def DownloadChecksum(self, file_path):
		fw = open(file_path, "rb")
		fw_size = GetFileSize(file_path)
		fw_contents = GetFileContent(file_path)
		crc32 = CRC32_BZIP(fw_contents)
		print "Crc32 = " + hex(crc32)

		# Build checksum_packet
		checksum_packet = struct.pack('<HL', fw_size, crc32)
		pkt = BluefinserialCommand()
		cmd = pkt.Packet('\x8b', '\x34', checksum_packet)
		rsp = ''

		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			print_err("Firmware checksum request fail")
			return False
		if len(rsp) < 3:
			print_err("Checksum: response packet not expected")
			return False			
		if rsp[2] == '\x00':
			print_ok("Firmware checksum passed")
		else:
			print_err("Firmware checksum mismatch")
		return True
