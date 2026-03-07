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
import threading
import ncom_protocol as ncom

import time

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
        if len(cls._table) < 256: cls._init_table()
        crc = 0xFFFF
        for byte in data:
            i = (crc >> 8) ^ byte
            crc = (crc << 8) ^ cls._table[i]
            crc &= 0xFFFF
        return crc

_tx_seq = 0
_tx_seq_lock = threading.Lock()

_unacked_buffer = {}
_unacked_lock = threading.Lock()
RETRY_TIMEOUT = 1.0 # seconds

def create_frame(msg_id, *args):
    global _tx_seq
    payload = ncom.Messages.pack(msg_id, *args)
    if payload is None or len(payload) > ncom.MAX_PAYLOAD_LEN: return None
    
    with _tx_seq_lock:
        seq = _tx_seq
        _tx_seq = (_tx_seq + 1) % 256
    
    # Calculate CRC independently of the prepended Sync Bytes
    # (Matches STM32 Hardware CRC which resets explicitly upon receiving SYNC_BYTE_2)
    data = struct.pack(ncom.ENDIAN_CHAR + "BBB", seq, msg_id, len(payload)) + payload
    crc = CRC16.calc(data)
    
    frame = bytearray()
    frame.extend([ncom.SYNC_BYTE_1, ncom.SYNC_BYTE_2])
    frame.extend(data)
    frame.extend(struct.pack(ncom.ENDIAN_CHAR + "H", crc))
    
    with _unacked_lock:
        _unacked_buffer[seq] = {
            'timestamp': time.time(),
            'frame': bytes(frame),
            'msg_id': msg_id
        }
    
    return bytes(frame)

def process_ack(seq, response):
    with _unacked_lock:
        if seq in _unacked_buffer:
            if response == 15: # ACK
                del _unacked_buffer[seq]
            elif response == 0: # NACK
                _unacked_buffer[seq]['timestamp'] = 0 # Force immediate retry

def get_retry_frames():
    now = time.time()
    retry_list = []
    with _unacked_lock:
        for seq, data in _unacked_buffer.items():
            if now - data['timestamp'] >= RETRY_TIMEOUT:
                data['timestamp'] = now
                retry_list.append((seq, data['msg_id'], data['frame']))
    return retry_list

class NCOMParser:
    STATE_WAIT_SYNC_1 = 0
    STATE_WAIT_SYNC_2 = 1
    STATE_WAIT_SEQ = 2
    STATE_WAIT_ID = 3
    STATE_WAIT_LEN = 4
    STATE_WAIT_PAYLOAD = 5
    STATE_WAIT_CRC = 6
    
    def __init__(self, on_error=None):
        self.state = self.STATE_WAIT_SYNC_1
        self.seq = 0
        self.msg_id = 0
        self.payload_len = 0
        self.payload_buf = bytearray()
        self.crc_buf = bytearray()
        self.on_error = on_error
    
    def parse_byte(self, byte):
    
        if self.state == self.STATE_WAIT_SYNC_1:
            if byte == ncom.SYNC_BYTE_1:
                self.state = self.STATE_WAIT_SYNC_2
                
        elif self.state == self.STATE_WAIT_SYNC_2:
            if byte == ncom.SYNC_BYTE_2:
                self.state = self.STATE_WAIT_SEQ
            elif byte == ncom.SYNC_BYTE_1:
                # Stay in SYNC_2 state, treating this byte as a new potential SYNC_1
                pass
            else:
                self.state = self.STATE_WAIT_SYNC_1
                
        elif self.state == self.STATE_WAIT_SEQ:
            self.seq = byte
            self.state = self.STATE_WAIT_ID
                
        elif self.state == self.STATE_WAIT_ID:
            self.msg_id = byte
            self.state = self.STATE_WAIT_LEN
            
        elif self.state == self.STATE_WAIT_LEN:
            if byte > ncom.MAX_PAYLOAD_LEN:
                self.state = self.STATE_WAIT_SYNC_1
                if self.on_error:
                    self.on_error(f"Length {byte} exceeds maximum allowed ({ncom.MAX_PAYLOAD_LEN}). Resetting parser.")
                return None
            
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
            self.state = self.STATE_WAIT_SYNC_1
        
        return None
    
    def _finalize(self):
        rx_crc = struct.unpack(ncom.ENDIAN_CHAR + "H", self.crc_buf)[0]
        header = struct.pack(ncom.ENDIAN_CHAR + "BBB", self.seq, self.msg_id, self.payload_len)
        calc_crc = CRC16.calc(header + self.payload_buf)
        self.state = self.STATE_WAIT_SYNC_1
        if calc_crc == rx_crc:
            unpacked = ncom.Messages.unpack(self.msg_id, self.payload_buf)
            if unpacked:
                return (self.msg_id, self.seq, unpacked)
            elif self.on_error:
                self.on_error(f"Unpack failed for ID {self.msg_id}")
        else:
            if self.on_error:
                self.on_error(f"CRC mismatch for ID {self.msg_id}: calculated {calc_crc}, received {rx_crc}")
        return None