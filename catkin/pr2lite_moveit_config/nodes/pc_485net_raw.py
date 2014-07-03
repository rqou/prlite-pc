#!/usr/bin/env python

import roslib
roslib.load_manifest('pr2lite_moveit_config')
import rospy
#from packets_485net.msg import *
from pr2lite_moveit_config.msg import *
import serial
import binascii
import struct
import time

ser = None

crc8_table = [
	#  0	 1	   2	 3	   4	 5	   6	 7	   8	 9	   A	 B	   C	 D	   E	 F
	0x00, 0xF7, 0xB9, 0x4E, 0x25, 0xD2, 0x9C, 0x6B, 0x4A, 0xBD, 0xF3, 0x04, 0x6F, 0x98, 0xD6, 0x21,
	0x94, 0x63, 0x2D, 0xDA, 0xB1, 0x46, 0x08, 0xFF, 0xDE, 0x29, 0x67, 0x90, 0xFB, 0x0C, 0x42, 0xB5,
	0x7F, 0x88, 0xC6, 0x31, 0x5A, 0xAD, 0xE3, 0x14, 0x35, 0xC2, 0x8C, 0x7B, 0x10, 0xE7, 0xA9, 0x5E,
	0xEB, 0x1C, 0x52, 0xA5, 0xCE, 0x39, 0x77, 0x80, 0xA1, 0x56, 0x18, 0xEF, 0x84, 0x73, 0x3D, 0xCA,
	0xFE, 0x09, 0x47, 0xB0, 0xDB, 0x2C, 0x62, 0x95, 0xB4, 0x43, 0x0D, 0xFA, 0x91, 0x66, 0x28, 0xDF,
	0x6A, 0x9D, 0xD3, 0x24, 0x4F, 0xB8, 0xF6, 0x01, 0x20, 0xD7, 0x99, 0x6E, 0x05, 0xF2, 0xBC, 0x4B,
	0x81, 0x76, 0x38, 0xCF, 0xA4, 0x53, 0x1D, 0xEA, 0xCB, 0x3C, 0x72, 0x85, 0xEE, 0x19, 0x57, 0xA0,
	0x15, 0xE2, 0xAC, 0x5B, 0x30, 0xC7, 0x89, 0x7E, 0x5F, 0xA8, 0xE6, 0x11, 0x7A, 0x8D, 0xC3, 0x34,
	0xAB, 0x5C, 0x12, 0xE5, 0x8E, 0x79, 0x37, 0xC0, 0xE1, 0x16, 0x58, 0xAF, 0xC4, 0x33, 0x7D, 0x8A,
	0x3F, 0xC8, 0x86, 0x71, 0x1A, 0xED, 0xA3, 0x54, 0x75, 0x82, 0xCC, 0x3B, 0x50, 0xA7, 0xE9, 0x1E,
	0xD4, 0x23, 0x6D, 0x9A, 0xF1, 0x06, 0x48, 0xBF, 0x9E, 0x69, 0x27, 0xD0, 0xBB, 0x4C, 0x02, 0xF5,
	0x40, 0xB7, 0xF9, 0x0E, 0x65, 0x92, 0xDC, 0x2B, 0x0A, 0xFD, 0xB3, 0x44, 0x2F, 0xD8, 0x96, 0x61,
	0x55, 0xA2, 0xEC, 0x1B, 0x70, 0x87, 0xC9, 0x3E, 0x1F, 0xE8, 0xA6, 0x51, 0x3A, 0xCD, 0x83, 0x74,
	0xC1, 0x36, 0x78, 0x8F, 0xE4, 0x13, 0x5D, 0xAA, 0x8B, 0x7C, 0x32, 0xC5, 0xAE, 0x59, 0x17, 0xE0,
	0x2A, 0xDD, 0x93, 0x64, 0x0F, 0xF8, 0xB6, 0x41, 0x60, 0x97, 0xD9, 0x2E, 0x45, 0xB2, 0xFC, 0x0B,
	0xBE, 0x49, 0x07, 0xF0, 0x9B, 0x6C, 0x22, 0xD5, 0xF4, 0x03, 0x4D, 0xBA, 0xD1, 0x26, 0x68, 0x9F
]

def do_checksum(data):
	csum = 0xFF;
	for b in data:
		byte = struct.unpack("B", b)[0]
		csum = crc8_table[(csum ^ byte) & 0xFF]
		csum = csum & 0xFF
	csum = csum ^ 0xFF
	return csum

def decode_blob(blob):
	out = []
	lastgoodbyte = 0;
	#this is a really horrible hack
	blob = decode_blob.leftovers + blob
	#print binascii.hexlify(blob)
	i = 0
	while i < len(blob):
	#for i in range(0, len(blob)):
		#print binascii.hexlify(blob)
		#print i
		if blob[i] == "\x00":
			i = i + 1
			continue
			#0 can never be a source address
			#otherwise we always pick up runs of 00 as valid packets
		#1/21/12: hack to look large to small instead
		asdfrange = range(3, min(len(blob)-i, 32))
		asdfrange.reverse()
		for l in asdfrange:
			#the range needs to be at least 3 for normal packets
			#otherwise 10 f0 in the header registers as a valid packet
			#1/21/12: change this to 4 (so every packet has >=1 byte payload)
			#1/28/12: change this back to 3 (0xC0 doesn't have a 1 byte payload. looking large to small may have helped)
			#print "from %d length %d" % (i, l)
			#this assumes all packets do have a checksum
			possiblepkt = blob[i:i+l]
			#print binascii.hexlify(possiblepkt)
			possiblecsum = struct.unpack("B", blob[i+l])[0]
			checksum = do_checksum(possiblepkt)
			#print "csum is %d %d" % (possiblecsum, checksum)
			if checksum == possiblecsum:
				#print binascii.hexlify(possiblepkt)
				pkt = packet_485net_raw()
				pkt.header.stamp = rospy.Time.now()
				pkt.data = possiblepkt + blob[i+l]
				out.append(pkt)
				lastgoodbyte = i+l+1
				#even more hacky
				i = lastgoodbyte
				#print "jumping to %d" % i
				break
		i = i + 1
	decode_blob.leftovers = blob[lastgoodbyte:]
	return out
			
#hack
decode_blob.leftovers = ""

def tx_packet(packet):
	#There is no good way to determine if the bus is free
	#We just transmit
	global ser
	delay150us = bytearray.fromhex(u'FF') * 19
	delay312us = bytearray.fromhex(u'FF') * 39
	#ser.setRTS(False)
	#ser.setDTR(False)
	#ser.write(packet.data)
	#ser.flush()
	ser.setRTS(False)
	ser.setDTR(False)
	ser.write(packet.data)
	ser.flush()
	ser.write(delay312us)
	ser.flush()
	ser.write(packet.data)
	ser.flush()
	ser.write(delay150us)
	ser.flush()
	#we really really need a fix for this
	#time.sleep(0.01)
	#change time from 0.2 to 0.01 and appear ok
	#send command twice in a row; the second time, no one should be on the bus
	#time.sleep(0.01)
	ser.setRTS(True)
	ser.setDTR(True)

def main():
	global ser
	print "PC 485net interface (raw serial)"
	rospy.init_node('pc_485net_raw_node')
	serport = rospy.get_param("~serport", "/dev/magellan-i2c-serial")
	baud = rospy.get_param("~baud", 1000000)
	print "Using serial port %s at baud rate %d" % (serport, baud)
	
	ser = serial.Serial(serport, baud, timeout=0)
	ser.setRTS(True)
	ser.setDTR(True)
	
	pub = rospy.Publisher("net_485net_incoming_packets", packet_485net_raw)
	rospy.Subscriber("net_485net_outgoing_packets", packet_485net_raw, tx_packet)
	
	while not rospy.is_shutdown():
		bytes = ser.read(500)
		if len(bytes) > 0:
			packets = decode_blob(bytes)
			for p in packets:
				pub.publish(p)

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException: pass