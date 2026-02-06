# MIT License
#
# Copyright (c) 2026 Emir Erbaltacı
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

MAINLOOP_SLEEP = 0.1 # Seconds

import serial
import serial.tools.list_ports
import time
import ncom_protocol as ncom
import ncom_driver as ncd

def find_stm32g4(): # Find the STM32G4 COM port by VID and PID
    for port in serial.tools.list_ports.comports():
        if port.vid == 0x0483 and port.pid == 0x5740:
            return port.device
    return None

def main():
    port = None
    print("Searching for STM32G4...")
    while port == None:
        port = find_stm32g4()
        time.sleep(1)
    ser = serial.Serial(port, baudrate=115200, timeout=0.1)  # No matter the baudrate, USB CDC runs at full speed
    print("Device connected.")
    
    parser = ncd.NCOMParser()   # Create NCOM parser instance
    
    while True: # Main loop
        time.sleep(MAINLOOP_SLEEP)
        if ser.in_waiting: # Check if there are any bytes in the RX buffer
            chunk = ser.read(ser.in_waiting)   # Read all available bytes
            for byte in chunk: # Parse each byte
                packet = parser.parse_byte(byte) # Get a packet if available
                if packet:  # If a packet is received
                    msg_id, data = packet # Unpack the packet
                    handle_packet(msg_id, data) # Handle the packet

def handle_packet(msg_id, data):
    if msg_id == ncom.Messages.ID_TO_NAME["HEARTBEAT"]:
        print(f"Device ID: {data[0]}\nMode: {data[1]}\nFlags: {data[2]}\nUptime: {data[3]}")

if __name__ == "__main__":
    ncom.Messages.NAME_TO_ID = {v: k for k, v in ncom.Messages.ID_TO_NAME.items()}
    main()