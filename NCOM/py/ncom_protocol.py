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
Auto-generated on: 06.02.2026 18:37:17
Version: 0.1
'''

ENDIAN_CHAR = '<'

import struct

SYNC_BYTE = 0xA5

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
    'SET_MODE_AUTO': 10,
    'SET_MODE_MANUAL': 11,
    'SET_MODE_HOLD_DEPTH': 12,
    'SET_MODE_HOLD_ATTITUDE': 13,
    'SET_MODE_STABILIZE': 14,
    'SET_TARGET_DEPTH_MM': 20,
    'SET_TARGET_ROLL_DEG': 21,
    'SET_TARGET_PITCH_DEG': 22,
    'SET_TARGET_YAW_DEG': 23,
    'SET_TARGET_SURGE_MM_S': 24,
    'SET_TARGET_SWAY_MM_S': 25,
    'SET_TARGET_HEAVE_MM_S': 26,
    'REBOOT_MCU': 40,
    'SET_LIGHTS_MAIN': 30,
    'SET_LIGHTS_AUX': 31,
    'SET_SERVO_CAM_TILT': 50,
    'SET_SERVO_CAM_PAN': 51,
    'SET_GRIPPER_PWM': 52,
    'CALIBRATE_PRESSURE_ZERO': 60,
    'CALIBRATE_GYRO_OFFSET': 61,
    'SET_TELEM_RATE': 70,
    'EMERGENCY_SURFACE': 254,
    'EMERGENCY_STOP': 255,
}

class Messages:
    ID_TO_NAME = {
        0: 'HEARTBEAT',
        1: 'SYS_STATUS',
        2: 'COMMAND',
    }

    FORMATS = {
        0: '<BBBI', # HEARTBEAT
        1: '<BHH', # SYS_STATUS
        2: '<Bi', # COMMAND
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
