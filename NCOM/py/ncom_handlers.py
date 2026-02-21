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

import ncom_protocol as ncom

def heartbeat_handler(data, last_uptime, print_hb=True):
    uptime_ms = data[3]
    
    obvious_reset = (last_uptime != -1 and uptime_ms < last_uptime)
    
    if obvious_reset:
        print(f"\n[NCOM WARNING] MCU reset detected!\n")
    
    last_uptime = uptime_ms
    
    if print_hb:
        seconds = int(uptime_ms / 1000)
        minutes, seconds = divmod(seconds, 60)
        hours, minutes = divmod(minutes, 60)
        days, hours = divmod(hours, 24)
        uptime_str = f"{days}d {hours}h {minutes}m {seconds}s"
        print(f"Device ID: {data[0]}\nMode: {data[1]}\nFlags: {data[2]}\nUptime: {uptime_str}\n")
        
    return last_uptime

def ack_handler(msg_id, data):
    parsed_ack = data # data is already returned as a tuple from parse_byte -> unpack
    req_msg_name = ncom.Messages.ID_TO_NAME.get(parsed_ack[0], f"Unknown ({parsed_ack[0]})")
    
    response_map = {0: "NACK", 1: "ACK", 254: "INVALID_PARAMETER", 255: "UNKNOWN_ERROR"}
    response_str = response_map.get(parsed_ack[2], f"Unknown ({parsed_ack[2]})")
    
    print(f"  Req Msg: {req_msg_name}")
    print(f"  Req Seq: {parsed_ack[1]}")
    print(f"  Response: {response_str}\n")