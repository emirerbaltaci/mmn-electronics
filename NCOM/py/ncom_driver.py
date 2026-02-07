# MIT License
#
# Copyright (c) 2026 MM Nautronics
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import struct
import ncom_protocol as ncom

class CRC16:
    _table = []
    
    @classmethod
    def _init_table(cls):
        poly = 0x1021
        for i in range(256):
            crc = i << 8
            for _ in range(8):
                if crc & 0x8000: crc = (crc << 1) ^ poly
                else: crc <<= 1
                crc &= 0xFFFF
            cls._table.append(crc)
    
    @classmethod
    def calc(cls, data):
        if not cls._table: cls._init_table()
        crc = 0xFFFF
        for byte in data:
            i = (crc >> 8) ^ byte
            crc = (crc << 8) ^ cls._table[i]
            crc &= 0xFFFF
        return crc

def create_frame(msg_id, *args):
    payload = ncom.Messages.pack(msg_id, *args)
    if payload is None: return None
    data = struct.pack(ncom.ENDIAN_CHAR + "BB", msg_id, len(payload)) + payload
    crc = CRC16.calc(data)
    return struct.pack(ncom.ENDIAN_CHAR + "B", ncom.SYNC_BYTE) + data + struct.pack(ncom.ENDIAN_CHAR + "H", crc)

class NCOMParser:
    STATE_WAIT_SYNC = 0
    STATE_WAIT_ID = 1
    STATE_WAIT_LEN = 2
    STATE_WAIT_PAYLOAD = 3
    STATE_WAIT_CRC = 4
    
    def __init__(self, on_error=None):
        self.state = self.STATE_WAIT_SYNC
        self.msg_id = 0
        self.payload_len = 0
        self.payload_buf = bytearray()
        self.crc_buf = bytearray()
        self.on_error = on_error
    
    def parse_byte(self, byte):
    
        if self.state == self.STATE_WAIT_SYNC:
            if byte == ncom.SYNC_BYTE:
                self.state = self.STATE_WAIT_ID
                
        elif self.state == self.STATE_WAIT_ID:
            self.msg_id = byte
            self.state = self.STATE_WAIT_LEN
            
        elif self.state == self.STATE_WAIT_LEN:
            self.payload_len = byte
            self.payload_buf = bytearray()
            if self.payload_len == 0:
                self.state = self.STATE_WAIT_CRC
                self.crc_buf = bytearray()
            else: self.state=self.STATE_WAIT_PAYLOAD
        
        elif self.state == self.STATE_WAIT_PAYLOAD:
            self.payload_buf.append(byte)
            if len(self.payload_buf) == self.payload_len:
                self.state = self.STATE_WAIT_CRC
                self.crc_buf = bytearray()
            
        elif self.state == self.STATE_WAIT_CRC:
            self.crc_buf.append(byte)
            if len(self.crc_buf) == 2:
                return self._finalize()
        
        else:
            self.state = self.STATE_WAIT_SYNC
        
        return None
    
    def _finalize(self):
        rx_crc = struct.unpack(ncom.ENDIAN_CHAR + "H", self.crc_buf)[0]
        header = struct.pack(ncom.ENDIAN_CHAR + "BB", self.msg_id, self.payload_len)
        calc_crc = CRC16.calc(header + self.payload_buf)
        self.state = self.STATE_WAIT_SYNC
        if calc_crc == rx_crc:
            unpacked = ncom.Messages.unpack(self.msg_id, self.payload_buf)
            if unpacked:
                return (self.msg_id, unpacked)
            elif self.on_error:
                self.on_error(f"Unpack failed for ID {self.msg_id}")
        else:
            if self.on_error:
                self.on_error(f"CRC mismatch for ID {self.msg_id}: calculated {calc_crc}, received {rx_crc}")
        return None