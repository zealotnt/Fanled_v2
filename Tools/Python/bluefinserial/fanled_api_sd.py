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
import hashlib

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
		file_idx = 0
		list_files = []
		while True:
			cmd = self.pkt.Packet('\x90', '\x10', chr(file_idx))
			rsp = self._datalink.Exchange(cmd)
			if rsp is None:
				return []
			if len(rsp) < 3:
				return []
			if rsp[2] != '\x00':
				print_err("Error when list file, errCode=%2x" % ord(rsp[2]))
				return []

			# Only processing the text return
			list_file_str = rsp[4:]
			list_files.extend(list_file_str.split('\n'))

			# If no more files
			if rsp[3] == '\x00':
				return list_files
			else:
				file_idx = len(list_files)

	def Inspect(self, name):
		cmd = self.pkt.Packet('\x90', '\x12')
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of inspect: ")

	def Read(self, name):
		name += '\x00'
		offset = 0
		contents = ""
		while True:
			argument = struct.pack('<I', offset) + name
			cmd = self.pkt.Packet('\x90', '\x14', argument)
			rsp = self._datalink.Exchange(cmd)
			if rsp is None:
				return ""
			if len(rsp) < 3:
				return ""
			if rsp[2] != '\x00':
				print_err("Error when read, code = " + str(ord(rsp[2])))
				return ""

			# Only processing the text return
			contents += rsp[4:]

			# If no more content
			if rsp[3] == '\x00':
				return contents
			else:
				offset = len(contents)

	def Delete(self, name):
		name += '\x00'
		cmd = self.pkt.Packet('\x90', '\x16', name)
		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			return False
		if len(rsp) < 3:
			return False
		if rsp[2] != '\x00':
			print_err("Error when delete, code = " + str(ord(rsp[2])))
			return False

		return True

	def WriteRaw(self, name, content):
		offset = 0
		name += '\x00'
		argument = struct.pack('<II', offset, len(content)) + name + content
		cmd = self.pkt.Packet('\x90', '\x18', argument)
		rsp = self._datalink.Exchange(cmd)
		dump_hex(rsp, "Response of write: ")

	def WriteFile(self, file_name, file_path):
		# 2b: CMD + CTR code
		# 4b: file offset
		# 4b: file content length
		# xb: file name
		# 1b: Null of filename
		# xb; file contents
		MAX_CONTENT_IN_FILE = self.pkt.DATA_CMD_MAX_LEN - 2 - 4 - 4 - len(file_name) - 1

		last_packet = False
		offset = 0
		file_name += '\x00'
		content = GetFileContent(file_path)
		while True:
			if offset + MAX_CONTENT_IN_FILE > len(content):
				content_to_send = content[offset:]
				last_packet = True
			else:
				content_to_send = content[offset:offset+MAX_CONTENT_IN_FILE]

			argument = struct.pack('<II', offset, len(content_to_send)) + file_name + content_to_send
			cmd = self.pkt.Packet('\x90', '\x18', argument)

			rsp = self._datalink.Exchange(cmd)
			if rsp is None:
				return False
			if len(rsp) < 3:
				return False
			if rsp[2] != '\x00':
				print_err("Error when write, code = " + str(ord(rsp[2])))
				return False

			# If no more content
			if last_packet == True :
				# Check md5 of written file
				file_without_null = file_name[:len(file_name) - 1]
				checksum = self.GetFileMd5(file_without_null)
				if checksum.encode("hex") == hashlib.md5(open(file_path,'rb').read()).hexdigest():
					print_ok("File download and checksum ok")
					return True
				print_err("File download but checksum fail")
				return False
			else:
				offset += MAX_CONTENT_IN_FILE

	def GetFileMd5(self, file_name):
		# 2b: CMD + CTR code
		# xb: file name
		# 1b: Null of filename
		file_name += '\x00'
		cmd = self.pkt.Packet('\x90', '\x20', file_name)
		rsp = self._datalink.Exchange(cmd)
		if rsp is None:
			return ""
		if len(rsp) < 3:
			return ""
		if rsp[2] != '\x00':
			print_err("Error when get md5, code = " + str(ord(rsp[2])))
			return ""

		if len(rsp[3:]) != 16:
			print_err("Wrong md5 len returned")
			return ""
		return rsp[3:]