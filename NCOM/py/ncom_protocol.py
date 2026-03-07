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
Auto-generated on: 07.03.2026 17:53:46
Version: 0.1
'''

import struct

ENDIAN_CHAR = '<'

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
    'SET_LIGHTS': 40,
    'REBOOT_MCU': 50,
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
    'ACK': 15,
    'INVALID_PARAMETER': 240,
    'UNKNOWN_ERROR': 255,
}
SET_POINT_UNITS = {
    'M': 0,
    'MM': 255,
}
SET_SPEED_UNITS = {
    'M_S': 0,
    'MM_S': 255,
}
SET_X_OR_SURGE_X_OR_SURGE = {
    'X_M': 0,
    'X_MM': 15,
    'SURGE_M_S': 240,
    'SURGE_MM_S': 255,
}
SET_Y_OR_SWAY_Y_OR_SWAY = {
    'Y_M': 0,
    'Y_MM': 15,
    'SWAY_M_S': 240,
    'SWAY_MM_S': 255,
}
SET_Z_OR_HEAVE_Z_OR_HEAVE = {
    'Z_M': 0,
    'Z_MM': 15,
    'HEAVE_M_S': 240,
    'HEAVE_MM_S': 255,
}
SET_ROLL_OR_P_ROLL_OR_P = {
    'ROLL_DEG': 0,
    'ROLL_RAD': 15,
    'P_DEG_S': 240,
    'P_RAD_S': 255,
}
SET_PITCH_OR_Q_PITCH_OR_Q = {
    'PITCH_DEG': 0,
    'PITCH_RAD': 15,
    'Q_DEG_S': 240,
    'Q_RAD_S': 255,
}
SET_YAW_OR_R_YAW_OR_R = {
    'YAW_DEG': 0,
    'YAW_RAD': 15,
    'R_DEG_S': 240,
    'R_RAD_S': 255,
}
CONFIG_REQ_CONFIG_ID = {
    'SYS_ID': 0,
    'COMP_ID': 1,
    'TELEM_RATE_HZ': 2,
    'ADC_SAMPLING_TIME': 3,
    'TIM_PRESCALER': 4,
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
    'ADC_SAMPLING_TIME': 3,
    'TIM_PRESCALER': 4,
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
    'FLOAT64': 7,
}
CONFIG_SET_MCU_CONFIG_ID = {
    'SYS_ID': 0,
    'COMP_ID': 1,
    'TELEM_RATE_HZ': 2,
    'ADC_SAMPLING_TIME': 3,
    'TIM_PRESCALER': 4,
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
ASSIGN_FLAG_BIT_BIT_MEANING = {
    'CLEAR': 0,
    'TASK_HB_STATEESTIMATE': 1,
    'TASK_HB_CONTROL': 2,
    'TASK_HB_NCOM': 3,
    'IS_ARMED': 4,
    'IS_PREARMED': 5,
    'DEPTH_HOLD_ACTIVE': 6,
    'HEADING_HOLD_ACTIVE': 7,
    'IMU_VALID': 8,
    'MAG_VALID': 9,
    'BAR30_VALID': 10,
    'IMU_TEMP_WARNING': 11,
    'BATTERY_LOW': 12,
    'LEAK_DETECTED': 13,
    'MISSION_RUNNING': 14,
    'COMM_TIMEOUT_WARNING': 15,
}
ASSIGN_FLAG_BIT_BIT_INTERRUPT = {
    'DISABLED': 0,
    'ENABLED': 255,
}
INIT_ERROR_ERROR_CODE = {
    'IMU_INIT_ERR': 0,
    'MAG_INIT_ERR': 1,
    'BAR30_INIT_ERR': 2,
}
HARDFAULT_ERROR_ERROR_CODE = {
    'STACK_OVERFLOW': 0,
    'SEGMENTATION_FAULT': 1,
}
DYING_GASP_FLAGS = {
    'EKF_TASK_DEAD': (1 << 0),
    'CONTROL_TASK_DEAD': (1 << 1),
    'NCOM_TASK_DEAD': (1 << 2),
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
        10: 'SET_POINT',
        11: 'SET_SPEED',
        12: 'SET_X_OR_SURGE',
        13: 'SET_Y_OR_SWAY',
        14: 'SET_Z_OR_HEAVE',
        15: 'SET_ROLL_OR_P',
        16: 'SET_PITCH_OR_Q',
        17: 'SET_YAW_OR_R',
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
        50: 'SEND_FLAG',
        51: 'ASSIGN_FLAG_BIT',
        52: 'CLEAR_FLAG',
        240: 'INIT_ERROR',
        241: 'HARDFAULT_ERROR',
        255: 'DYING_GASP',
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
        'SET_POINT': 10,
        'SET_SPEED': 11,
        'SET_X_OR_SURGE': 12,
        'SET_Y_OR_SWAY': 13,
        'SET_Z_OR_HEAVE': 14,
        'SET_ROLL_OR_P': 15,
        'SET_PITCH_OR_Q': 16,
        'SET_YAW_OR_R': 17,
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
        'SEND_FLAG': 50,
        'ASSIGN_FLAG_BIT': 51,
        'CLEAR_FLAG': 52,
        'INIT_ERROR': 240,
        'HARDFAULT_ERROR': 241,
        'DYING_GASP': 255,
    }

    FORMATS = {
        0: '<BBBI', # HEARTBEAT
        1: '<BHf', # SYS_STATUS
        2: '<Bi', # COMMAND
        3: '<fffB', # ATTITUDE
        4: '<fff', # ROTATION_RATES
        5: '<fff', # LINEAR_VELOCITY
        6: '<iiB', # DEPTH
        7: '<H', # MODE_STATUS
        8: '<Bifff', # TARGETS
        9: '<BBB', # ACKNOWLEDGEMENT
        10: '<Biiiiii', # SET_POINT
        11: '<Biiiiii', # SET_SPEED
        12: '<Bi', # SET_X_OR_SURGE
        13: '<Bi', # SET_Y_OR_SWAY
        14: '<Bi', # SET_Z_OR_HEAVE
        15: '<Bi', # SET_ROLL_OR_P
        16: '<Bi', # SET_PITCH_OR_Q
        17: '<Bi', # SET_YAW_OR_R
        40: '<B', # CONFIG_REQ
        41: '<BBi', # CONFIG_RESP
        42: '<BBi', # CONFIG_SET_MCU
        43: '<HBi', # CONFIG_SET_FREERTOS
        44: '<HBi', # CONFIG_SET_SENSOR
        45: '<HBi', # CONFIG_SET_ACTUATOR
        46: '<HBi', # CONFIG_SET_PID
        47: '<HB9i', # CONFIG_SET_EKF
        48: '<', # CONFIG_REQ_STARTUP
        49: '<', # CONFIG_SET_STARTUP
        50: '<I', # SEND_FLAG
        51: '<BBB', # ASSIGN_FLAG_BIT
        52: '<B', # CLEAR_FLAG
        240: '<B', # INIT_ERROR
        241: '<B', # HARDFAULT_ERROR
        255: '<B', # DYING_GASP
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
