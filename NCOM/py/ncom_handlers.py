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

from datetime import datetime
import ncom_protocol as ncom
import ncom_driver as ncd

def heartbeat_handler(msg_id, seq, data, heartbeat_state, print_hb):
    device_id, vehicle_state, flags, uptime_ms = data
    
    # uint32 max is ~49 days (4,294,967,295 ms). Wrap-around is masked via upper bounds check.
    wrap_threshold = 0xF0000000 # 45 days
    is_wrap_around = (heartbeat_state['last_uptime'] > wrap_threshold and uptime_ms < 0x10000000)
    
    obvious_reset = (heartbeat_state['last_uptime'] != -1 and uptime_ms < heartbeat_state['last_uptime'] and not is_wrap_around)
    
    if obvious_reset:
        print(f"\n[NCOM WARNING] MCU reset detected!\n")
    
    heartbeat_state['last_uptime'] = uptime_ms
    
    if print_hb:
        seconds = int(uptime_ms / 1000)
        minutes, seconds = divmod(seconds, 60)
        hours, minutes = divmod(minutes, 60)
        days, hours = divmod(hours, 24)
        uptime_str = f"{days}d {hours}h {minutes}m {seconds}s"
        
        timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
        print(f"\r[{timestamp}]\nSequence: {seq}\nMessage: HEARTBEAT (ID: {msg_id})")
        print(f"Device ID: {device_id}\nMode: {vehicle_state}\nFlags: {flags}\nUptime: {uptime_str}\n")
        
    return True

def ack_handler(msg_id, seq, data, _heartbeat_state, _print_hb):
    requested_msg_id, requested_seq, response = data
    req_msg_name = ncom.Messages.ID_TO_NAME.get(requested_msg_id, f"Unknown ({requested_msg_id})")
    
    response_map = {0: "NACK", 1: "ACK", 254: "INVALID_PARAMETER", 255: "UNKNOWN_ERROR"}
    response_str = response_map.get(response, f"Unknown ({response})")
    
    timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
    print(f"\r[{timestamp}]\nSequence: {seq}\nMessage: ACKNOWLEDGEMENT (ID: {msg_id})")
    print(f"  Req Msg: {req_msg_name}")
    print(f"  Req Seq: {requested_seq}")
    print(f"  Response: {response_str}\n")
    
    ncd.process_ack(requested_seq, response)

    return True

DISPATCH_TABLE = {
    ncom.Messages.NAME_TO_ID["HEARTBEAT"]: heartbeat_handler,
    ncom.Messages.NAME_TO_ID["ACKNOWLEDGEMENT"]: ack_handler
}

def is_handled(msg_id):
    return msg_id in DISPATCH_TABLE

def process_packet(msg_id, seq, data, heartbeat_state, print_hb=True):
    handler = DISPATCH_TABLE.get(msg_id)
    if handler:
        return handler(msg_id, seq, data, heartbeat_state, print_hb)
    return False