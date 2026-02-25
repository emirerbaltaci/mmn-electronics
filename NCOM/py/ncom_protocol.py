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

'''
NCOM Python Module
Auto-generated on: 25.02.2026 17:53:02
Version: 0.1
'''

ENDIAN_CHAR = '<'

import struct

SYNC_BYTE_1 = 0xA5
SYNC_BYTE_2 = 0x5A

MAX_PAYLOAD_LEN = 255
HEADER_LEN = 5
FOOTER_LEN = 2
OVERHEAD_LEN = HEADER_LEN + FOOTER_LEN
SYNCBYTE_COUNT = 2

HEARTBEAT_VEHICLE_STATE = {
    'INIT': 0,
    'DISARMED': 1,
    'PREARMED': 2,
    'ARMED': 3,
    'MISSION': 4,
    'DISARMED_MANUAL': 11,
    'PREARMED_MANUAL': 12,
    'ARMED_MANUAL': 13,
    'DISARMED_ERR': 21,
    'PREARMED_ERR': 22,
    'ARMED_ERR': 23,
    'DISARMED_MANUAL_ERR': 31,
    'PREARMED_MANUAL_ERR': 32,
    'ARMED_MANUAL_ERR': 33,
    'MCU_HARDFAULT': 255,
}
HEARTBEAT_FLAGS = {
    'LEAK_DETECTED': (1 << 0),
    'BATTERY_LOW': (1 << 1),
    'INIT_ERR': (1 << 2),
    'NAV_ERR': (1 << 3),
    'BAR30_ERR': (1 << 4),
    'PERIPHERAL_ERR': (1 << 5),
    'RTOS_TASK_ERR': (1 << 6),
    'MISSION_ABORTED': (1 << 7),
}
COMMAND_CMD_ID = {
    'DISARM': 0,
    'PREARM': 1,
    'ARM': 2,
    'START_MISSION': 3,
    'ABORT_MISSION': 4,
    'CLEAR_FLAGS': 5,
    'SET_MODE': 10,
    'SET_TARGET_DEPTH_MM': 20,
    'SET_TARGET_ROLL_DEG': 21,
    'SET_TARGET_PITCH_DEG': 22,
    'SET_TARGET_YAW_DEG': 23,
    'SET_TARGET_SURGE_MM_S': 24,
    'SET_TARGET_SWAY_MM_S': 25,
    'SET_TARGET_HEAVE_MM_S': 26,
    'SET_LIGHTS': 30,
    'REBOOT_MCU': 40,
    'CALIBRATE_DEPTH_0': 100,
    'CALIBRATE_AXES_0': 101,
    'SET_HEARTBEAT_RATE': 200,
    'EMERGENCY': 255,
}
MODE_STATUS_FLAGS = {
    'DISARMED': (1 << 0),
    'PREARMED': (1 << 1),
    'ARMED': (1 << 2),
    'AUTO': (1 << 3),
    'MANUAL': (1 << 4),
    'HOLD_DEPTH': (1 << 5),
    'HOLD_ATTITUDE': (1 << 6),
    'STABILIZE': (1 << 7),
    'MISSION_ACTIVE': (1 << 8),
}
TARGETS_FLAGS = {
    'TARGET_DEPTH_IS_ACTIVE': (1 << 0),
    'TARGET_ROLL_IS_ACTIVE': (1 << 1),
    'TARGET_PITCH_IS_ACTIVE': (1 << 2),
    'TARGET_YAW_IS_ACTIVE': (1 << 3),
}
ACKNOWLEDGEMENT_RESPONSE = {
    'NACK': 0,
    'ACK': 1,
    'INVALID_PARAMETER': 254,
    'UNKNOWN_ERROR': 255,
}
CONFIG_REQ_CONFIG_ID = {
    'SYS_ID': 0,
    'COMP_ID': 1,
    'TELEM_RATE_HZ': 2,
    'TASK_RATE_TELEM': 10,
    'TASK_RATE_CTRL': 11,
    'IMU_RATE_HZ': 20,
    'BAR30_RATE_HZ': 21,
    'MAG_RATE_HZ': 22,
    'PWM_MIN_PULSE': 30,
    'PWM_MAX_PULSE': 31,
    'ESC_MODE': 32,
    'PID_ROLL_P': 40,
    'PID_ROLL_I': 41,
    'PID_ROLL_D': 42,
    'PID_PITCH_P': 43,
    'PID_PITCH_I': 44,
    'PID_PITCH_D': 45,
    'PID_YAW_P': 46,
    'PID_YAW_I': 47,
    'PID_YAW_D': 48,
    'PID_DEPTH_P': 49,
    'PID_DEPTH_I': 50,
    'PID_DEPTH_D': 51,
    'EKF_PROCESS_NOISE': 60,
    'EKF_MEAS_NOISE': 61,
}
CONFIG_RESP_CONFIG_ID = {
    'SYS_ID': 0,
    'COMP_ID': 1,
    'TELEM_RATE_HZ': 2,
    'TASK_RATE_TELEM': 10,
    'TASK_RATE_CTRL': 11,
    'IMU_RATE_HZ': 20,
    'BARO_RATE_HZ': 21,
    'MAG_RATE_HZ': 22,
    'PWM_MIN_PULSE': 30,
    'PWM_MAX_PULSE': 31,
    'ESC_MODE': 32,
    'PID_ROLL_P': 40,
    'PID_ROLL_I': 41,
    'PID_ROLL_D': 42,
    'PID_PITCH_P': 43,
    'PID_PITCH_I': 44,
    'PID_PITCH_D': 45,
    'PID_YAW_P': 46,
    'PID_YAW_I': 47,
    'PID_YAW_D': 48,
    'PID_DEPTH_P': 49,
    'PID_DEPTH_I': 50,
    'PID_DEPTH_D': 51,
    'EKF_PROCESS_NOISE': 60,
    'EKF_MEAS_NOISE': 61,
}
CONFIG_RESP_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
CONFIG_SET_MCU_CONFIG_ID = {
    'SYS_ID': 0,
    'COMP_ID': 1,
    'TELEM_RATE_HZ': 2,
}
CONFIG_SET_MCU_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
CONFIG_SET_FREERTOS_CONFIG_ID = {
    'TASK_RATE_TELEM': 10,
    'TASK_RATE_CTRL': 11,
}
CONFIG_SET_FREERTOS_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
CONFIG_SET_SENSOR_CONFIG_ID = {
    'IMU_RATE_HZ': 20,
    'BARO_RATE_HZ': 21,
    'MAG_RATE_HZ': 22,
}
CONFIG_SET_SENSOR_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
CONFIG_SET_ACTUATOR_CONFIG_ID = {
    'PWM_MIN_PULSE': 30,
    'PWM_MAX_PULSE': 31,
    'ESC_MODE': 32,
}
CONFIG_SET_ACTUATOR_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
CONFIG_SET_PID_CONFIG_ID = {
    'PID_ROLL_P': 40,
    'PID_ROLL_I': 41,
    'PID_ROLL_D': 42,
    'PID_PITCH_P': 43,
    'PID_PITCH_I': 44,
    'PID_PITCH_D': 45,
    'PID_YAW_P': 46,
    'PID_YAW_I': 47,
    'PID_YAW_D': 48,
    'PID_DEPTH_P': 49,
    'PID_DEPTH_I': 50,
    'PID_DEPTH_D': 51,
}
CONFIG_SET_PID_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
CONFIG_SET_EKF_CONFIG_ID = {
    'EKF_PROCESS_NOISE': 60,
    'EKF_MEAS_NOISE': 61,
}
CONFIG_SET_EKF_DATA_TYPE = {
    'INT8': 0,
    'UINT8': 1,
    'INT16': 2,
    'UINT16': 3,
    'INT32': 4,
    'UINT32': 5,
    'FLOAT32': 6,
}
INIT_ERROR_ERROR_CODE = {
    'IMU_INIT_ERR': 0,
    'MAG_INIT_ERR': 1,
    'BAR30_INIT_ERR': 2,
}

class Messages:
    ID_TO_NAME = {
        0: 'HEARTBEAT',
        1: 'SYS_STATUS',
        2: 'COMMAND',
        3: 'ATTITUDE',
        4: 'ROTATION_RATES',
        5: 'LINEAR_VELOCITY',
        6: 'DEPTH',
        7: 'MODE_STATUS',
        8: 'TARGETS',
        9: 'ACKNOWLEDGEMENT',
        40: 'CONFIG_REQ',
        41: 'CONFIG_RESP',
        42: 'CONFIG_SET_MCU',
        43: 'CONFIG_SET_FREERTOS',
        44: 'CONFIG_SET_SENSOR',
        45: 'CONFIG_SET_ACTUATOR',
        46: 'CONFIG_SET_PID',
        47: 'CONFIG_SET_EKF',
        48: 'CONFIG_REQ_STARTUP',
        49: 'CONFIG_SET_STARTUP',
        200: 'INIT_ERROR',
    }

    NAME_TO_ID = {
        'HEARTBEAT': 0,
        'SYS_STATUS': 1,
        'COMMAND': 2,
        'ATTITUDE': 3,
        'ROTATION_RATES': 4,
        'LINEAR_VELOCITY': 5,
        'DEPTH': 6,
        'MODE_STATUS': 7,
        'TARGETS': 8,
        'ACKNOWLEDGEMENT': 9,
        'CONFIG_REQ': 40,
        'CONFIG_RESP': 41,
        'CONFIG_SET_MCU': 42,
        'CONFIG_SET_FREERTOS': 43,
        'CONFIG_SET_SENSOR': 44,
        'CONFIG_SET_ACTUATOR': 45,
        'CONFIG_SET_PID': 46,
        'CONFIG_SET_EKF': 47,
        'CONFIG_REQ_STARTUP': 48,
        'CONFIG_SET_STARTUP': 49,
        'INIT_ERROR': 200,
    }

    FORMATS = {
        0: '<BBBI', # HEARTBEAT
        1: '<BHf', # SYS_STATUS
        2: '<Biiiiii', # COMMAND
        3: '<fffB', # ATTITUDE
        4: '<fff', # ROTATION_RATES
        5: '<fff', # LINEAR_VELOCITY
        6: '<iiB', # DEPTH
        7: '<H', # MODE_STATUS
        8: '<Bifff', # TARGETS
        9: '<BBB', # ACKNOWLEDGEMENT
        40: '<H', # CONFIG_REQ
        41: '<HBi', # CONFIG_RESP
        42: '<HBiB', # CONFIG_SET_MCU
        43: '<HBiB', # CONFIG_SET_FREERTOS
        44: '<HBiB', # CONFIG_SET_SENSOR
        45: '<HBiB', # CONFIG_SET_ACTUATOR
        46: '<HBiB', # CONFIG_SET_PID
        47: '<HB9i', # CONFIG_SET_EKF
        48: '<', # CONFIG_REQ_STARTUP
        49: '<', # CONFIG_SET_STARTUP
        200: '<B', # INIT_ERROR
    }

    @staticmethod
    def pack(msg_id, *args):
        if msg_id not in Messages.FORMATS:
            raise ValueError(f'Unknown Message ID: {msg_id}')
        fmt = Messages.FORMATS[msg_id]
        try:
            return struct.pack(fmt, *args)
        except struct.error as e:
            print(f'Packing error for ID {msg_id}: {e}')
            return None

    @staticmethod
    def unpack(msg_id, data):
        if msg_id not in Messages.FORMATS:
            return None
        fmt = Messages.FORMATS[msg_id]
        try:
            return struct.unpack(fmt, data)
        except struct.error:
            return None
