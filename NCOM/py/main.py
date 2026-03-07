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

MAINLOOP_SLEEP = 0.01 # Seconds
RECONNECT_SLEEP = 0.1 # Seconds

import ncom_protocol as ncom
import ncom_driver as ncd
import ncom_handlers as nch
import serial
import serial.tools.list_ports
import time
from datetime import datetime
import threading
import sys
import socket
import json

def find_stm32g4(): # Find the STM32G4 COM port by VID and PID
    for port in serial.tools.list_ports.comports():
        if port.vid == 0x0483 and port.pid == 0x5740:
            return port.device
    return None

def parser_error_handler(error):
    print(f"[NCOMParser ERROR]: {error}")

# Use a threading Event for thread-safe cross-thread UI toggle
print_hb_event = threading.Event()

def handle_packet(msg_id, data, ncom_parser, heartbeat_state):
    # Process known handlers directly
    is_handled = nch.process_packet(msg_id, ncom_parser.seq, data, heartbeat_state, print_hb_event.is_set())
    
    # If the packet isn't handled implicitly, print the payload sequentially
    if not is_handled:
        timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
        print(f"\r[{timestamp}]\nSequence: {ncom_parser.seq}\nMessage: {ncom.Messages.ID_TO_NAME[msg_id]} (ID: {msg_id})")
        
        print("  Payload parameters (in order):")
        for i, value in enumerate(data):
            print(f"    [{i}]: {value}")
        print("")

def udp_listen_func(ser):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("127.0.0.1", 5005))
    sock.settimeout(1.0)
    
    print("Listening for CLI commands on UDP 127.0.0.1:5005\n")
    
    while ser.is_open:
        try:
            data, addr = sock.recvfrom(1024)
            msg = json.loads(data.decode('utf-8'))
            
            if msg.get("command") == "TOGGLE_HB":
                if print_hb_event.is_set():
                    print_hb_event.clear()
                    status = "OFF"
                else:
                    print_hb_event.set()
                    status = "ON"
                print(f"\n[UDP] Heartbeat printing toggled {status}\n")
                continue
                
            msg_id = msg.get("msg_id")
            args = msg.get("args")
            
            if msg_id is not None and args is not None:
                # Create frame
                frame = ncd.create_frame(msg_id, *args)
                if frame:
                    ser.write(frame)
                    msg_name = ncom.Messages.ID_TO_NAME.get(msg_id, "UNKNOWN")
                    timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
                    print(f"\n[{timestamp}] [UDP] Forwarded to MCU: {msg_name} {args}\n")
                else:
                    print("\n[UDP] Error packing frame.\n")
                    
        except socket.timeout:
            continue
        except Exception as e:
            if ser.is_open:
                print(f"\n[UDP] Server Error: {e}")
                
    sock.close()

def main():
    port = None
    print("Searching for STM32G4...")
    
    while True:
        if port is None:
            port = find_stm32g4()
            if port is None:
                time.sleep(RECONNECT_SLEEP)
                continue
                
        try:
            with serial.Serial(port, baudrate=115200, timeout=0.1) as ser:  # No matter the baudrate, USB CDC runs at full speed
                print("Device connected.\n")
            
                parser = ncd.NCOMParser(on_error=parser_error_handler)   # Create NCOM parser instance
                
                # Start background thread for listening UDP
                udp_thread = threading.Thread(target=udp_listen_func, args=(ser,), daemon=True)
                udp_thread.start()
                
                # Wrapped mutable state for the packet handler loop
                heartbeat_state = {'last_uptime': -1}
                
                # Run the RX loop blockingly in the main thread now
                while ser.is_open:
                    time.sleep(MAINLOOP_SLEEP)
                    try:
                        if ser.in_waiting: # Check if there are any bytes in the RX buffer
                            chunk = ser.read(ser.in_waiting)   # Read all available bytes
                            for byte in chunk: # Parse each byte
                                packet = parser.parse_byte(byte) # Get a packet if available
                                if packet:  # If a packet is received
                                    msg_id, seq, data = packet # Unpack the packet
                                    handle_packet(msg_id, data, parser, heartbeat_state) # Handle the packet
                    except OSError:
                        raise serial.SerialException("Connection lost")
                        
                    # Check for retries
                    for seq, r_msg_id, frame in ncd.get_retry_frames():
                        msg_name = ncom.Messages.ID_TO_NAME.get(r_msg_id, "UNKNOWN")
                        timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
                        print(f"\n[{timestamp}] [RETRY] Resending un-ACK'd message: {msg_name} (Seq: {seq})\n")
                        ser.write(frame)
                        
        except (serial.SerialException, OSError):
            print("\nDevice disconnected. Trying to reconnect....")
            port = None
            time.sleep(RECONNECT_SLEEP)

if __name__ == "__main__":
    main()