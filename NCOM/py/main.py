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

import ncom_protocol as ncom
import ncom_driver as ncd
import ncom_handlers as nch
import serial
import serial.tools.list_ports
import time
from datetime import datetime
import threading
import sys

def find_stm32g4(): # Find the STM32G4 COM port by VID and PID
    for port in serial.tools.list_ports.comports():
        if port.vid == 0x0483 and port.pid == 0x5740:
            return port.device
    return None

def parser_error_handler(error):
    print(f"[NCOMParser ERROR]: {error}")

last_uptime = -1
print_hb = False

def handle_packet(msg_id, data, ncom_parser):
    global last_uptime, print_hb
    is_hb = (msg_id == ncom.Messages.NAME_TO_ID["HEARTBEAT"])
    is_ack = (msg_id == ncom.Messages.NAME_TO_ID["ACKNOWLEDGEMENT"])
    
    if is_hb and not print_hb: # Update last_uptime without printing anything
        last_uptime = nch.heartbeat_handler(data, last_uptime, print_hb=False)
        return
    
    timestamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
    print(f"\r[{timestamp}]\nSequence: {ncom_parser.seq}\nMessage: {ncom.Messages.ID_TO_NAME[msg_id]} (ID: {msg_id})")
    
    if is_hb:
        last_uptime = nch.heartbeat_handler(data, last_uptime, print_hb=True)
    elif is_ack:
        nch.ack_handler(msg_id, data)
    else:
        parsed = data
        print("  Payload parameters (in order):")
        for i, value in enumerate(parsed):
            print(f"    [{i}]: {value}")
        print("")

def rx_thread_func(ser, parser):
    while ser.is_open:
        try:
            time.sleep(MAINLOOP_SLEEP)
            if ser.in_waiting: # Check if there are any bytes in the RX buffer
                chunk = ser.read(ser.in_waiting)   # Read all available bytes
                for byte in chunk: # Parse each byte
                    packet = parser.parse_byte(byte) # Get a packet if available
                    if packet:  # If a packet is received
                        msg_id, seq, data = packet # Unpack the packet
                        handle_packet(msg_id, data, parser) # Handle the packet
        except OSError:
            print("\n[RX] Connection lost.")
            break
        except Exception as e:
            if ser.is_open:
                print(f"\n[RX] Error: {e}")
            break

def udp_listen_func(ser):
    import socket
    import json
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("127.0.0.1", 5005))
    sock.settimeout(1.0)
    
    print("Listening for CLI commands on UDP 127.0.0.1:5005\n")
    
    while ser.is_open:
        try:
            data, addr = sock.recvfrom(1024)
            msg = json.loads(data.decode('utf-8'))
            
            if msg.get("command") == "TOGGLE_HB":
                global print_hb
                print_hb = not print_hb
                status = "ON" if print_hb else "OFF"
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
                time.sleep(1)
                continue
                
        try:
            with serial.Serial(port, baudrate=115200, timeout=0.1) as ser:  # No matter the baudrate, USB CDC runs at full speed
                print("Device connected.\n")
            
                parser = ncd.NCOMParser(on_error=parser_error_handler)   # Create NCOM parser instance
                
                # Start background thread for listening UDP
                udp_thread = threading.Thread(target=udp_listen_func, args=(ser,), daemon=True)
                udp_thread.start()
                
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
                                    handle_packet(msg_id, data, parser) # Handle the packet
                    except OSError:
                        raise serial.SerialException("Connection lost")
                
                udp_thread.join(timeout=1.0)
                
        except (serial.SerialException, OSError):
            print("\nDevice disconnected. Trying to reconnect....")
            port = None
            time.sleep(1)

if __name__ == "__main__":
    main()