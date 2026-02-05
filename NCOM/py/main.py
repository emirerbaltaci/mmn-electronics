MAINLOOP_SLEEP = 0.01 # Seconds

import serial
import serial.tools.list_ports
import time
import ncom_protocol as ncom
import ncom_driver as ncd

def find_stm32g4():
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
    ser = serial.Serial(port, 115200, timeout=0.1)
    print("Device connected.")
    
    parser = ncd.NCOMParser()
    last_heartbeat = 0.0
    device_opened = time.time()
    
    while True:
        time.sleep(MAINLOOP_SLEEP)
        curr_time = time.time()
        if curr_time - last_heartbeat >= 1.0:
            packet = ncd.create_frame(0, 1, 3, 0, int(curr_time - device_opened))
            if packet:
                ser.write(packet)
            last_heartbeat = curr_time
        
        if ser.in_waiting:
            chunk = ser.read(ser.in_waiting)
            for byte in chunk:
                packet = parser.parse_byte(byte)
                if packet:
                    msg_id, data = packet
                    handle_packet(msg_id, data)

def handle_packet(msg_id, data):
    if msg_id == 0:
        print(f"Device ID: {data[0]}\nMode: {data[1]}\nFlags: {data[2]}\nUptime: {data[3]}")

if __name__ == "__main__":
    ncom.Messages.NAME_TO_ID = {v: k for k, v in ncom.Messages.ID_TO_NAME.items()}
    main()