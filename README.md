# mmn-electronics

Main code repository for MM Nautronics - Electronics subteam. Includes firmware and host communication software for the MM Nautronics AUV controller.

## MCU Firmware

The microcontroller firmware (under `MCU/` directory) is built on **FreeRTOS** to ensure real-time performance and efficient task management. The system will leverage FreeRTOS tasks for sensor data acquisition, PID control, NCOM protocol handling, and system monitoring.
The microcontroller firmware (under `c0pdb/` directory) is the firmware for the microcontroller present on power distribution board.

## Drivers

The `Drivers/` directory contains sensor drivers implemented using the **STM32 HAL (Hardware Abstraction Layer)**. These drivers provide high-level interfaces for communicating with the onboard sensors:

- **`bar30**: Driver for the Blue Robotics Bar30 High-Resolution Pressure Sensor.
- **`imu-icm42688p`**: Driver for the TDK InvenSense ICM-42688-P 6-axis MotionTracking device.
- **`mag-lis2mdl`**: Driver for the STMicroelectronics LIS2MDL ultra-low-power digital 3-axis magnetometer.

## NCOM Protocol

The Nautronics Communication Protocol (NCOM) facilitates robust binary communication between the MCU and the Host.

### Working Principle

NCOM operates on a "Single Source of Truth" principle using a JSON definition file.

1.  **Definition**: The protocol structure (message IDs, payloads, data types) is defined in `NCOM/ncom_def.json` and other files in `NCOM/msg/`.
2.  **Code Generation**: The `NCOM/codegen.py` script parses this JSON file and automatically generates:
    -   **C Source/Header** (`NCOM/c/`): Used by the MCU firmware to pack/unpack messages into C structs.
    -   **Python Module** (`NCOM/py/`): Used by the host software to interface with the device.

Currently, the communication frame structure is as follows:

`[SYNC1 (0xA5)] [SYNC2 (0x5A)] [SEQ] [MSG_ID] [PAYLOAD_LEN] [PAYLOAD...] [CRC16]`

Note that this shows the current structure to inform the reader, the frame will most likely be changed in the future.

-   **SYNC1 and SYNC2**: Synchronization bytes (0xA5 and 0x5A) to identify the start of a frame.
-   **CRC16**: Checksum to ensure data integrity.

### How to Edit the Protocol

To add new messages or modify existing ones:

1.  **Edit Definition**: Open relevant file in `NCOM/msg/` and add your message definition.
    ```json
    {
      "id": 10,
      "name": "MY_NEW_MSG",
      "payload": [
        {"name": "value1", "type": "uint16"},
        {"name": "value2", "type": "float"}
      ]
    }
    ```
2.  **Generate Code**: Run the code generation script to update both C and Python implementations.
    ```bash
    python NCOM/codegen.py
    ```
    This ensures that both the firmware and host software are always in sync.

## License

This project is licensed under the MIT License.

### Third-Party Licenses

- **STM32 HAL Drivers** — Licensed by STMicroelectronics under the Ultimate Liberty license (SLA0044).
- **CMSIS-DSP Library** (`MCU/Third_Party/ARM/CMSIS/DSP/`) — Copyright © Arm Limited. Licensed under the Apache License, Version 2.0.
- **FreeRTOS** (`MCU/Third_Party/FreeRTOS/`) — Copyright © Amazon.com, Inc. Licensed under the MIT License.
