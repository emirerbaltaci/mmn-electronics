/*
 * MIT License
 *
 * Copyright (c) 2026 MM Nautronics
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ncom_handlers.h"
#include "auvcontrol.h"

extern Setpoint_t setPoint;
extern Setspeed_t setSpeed;
extern uint8_t armStatus;

bool NCOM_Handlers_Selector(NCOM_RX_t *rx){

	if(rx == NULL) return false;

	bool res = false;

	switch(rx->parser.msgId){

	case NCOM_MSG_COMMAND:
		if(rx->parser.payloadLen != NCOM_LEN_COMMAND) break;
		res = NCOM_Handlers_Command(rx);
		break;

	case NCOM_MSG_CONFIG_REQ:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_REQ) break;
		res = NCOM_Handlers_ConfigReq(rx);
		break;

	case NCOM_MSG_CONFIG_SET_MCU:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_MCU) break;
		res = NCOM_Handlers_ConfigSetMcu(rx);
		break;

	case NCOM_MSG_CONFIG_SET_FREERTOS:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_FREERTOS) break;
		res = NCOM_Handlers_ConfigSetFreertos(rx);
		break;

	case NCOM_MSG_CONFIG_SET_SENSOR:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_SENSOR) break;
		res = NCOM_Handlers_ConfigSetSensor(rx);
		break;

	case NCOM_MSG_CONFIG_SET_ACTUATOR:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_ACTUATOR) break;
		res = NCOM_Handlers_ConfigSetActuator(rx);
		break;

	case NCOM_MSG_CONFIG_SET_PID:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_PID) break;
		res = NCOM_Handlers_ConfigSetPID(rx);
		break;

	case NCOM_MSG_CONFIG_SET_EKF:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_EKF) break;
		res = NCOM_Handlers_ConfigSetEKF(rx);
		break;

	case NCOM_MSG_CONFIG_SET_STARTUP:
		if(rx->parser.payloadLen != NCOM_LEN_CONFIG_SET_STARTUP) break;
		res = NCOM_Handlers_ConfigSetStartup(rx);
		break;

	default:
		break;

	}

	return res;
}

bool NCOM_Handlers_Command(NCOM_RX_t *rx){

	NCOM_Payload_COMMAND_t payload;
	memcpy(&payload, rx->parser.payloadBuf, rx->parser.payloadLen);

	switch (payload.cmd_id){

	case NCOM_COMMAND_CMD_ID_DISARM:
		break;

	case NCOM_COMMAND_CMD_ID_PREARM:
		break;

	case NCOM_COMMAND_CMD_ID_ARM:
		break;

	case NCOM_COMMAND_CMD_ID_START_MISSION:
		break;

	case NCOM_COMMAND_CMD_ID_ABORT_MISSION:
		break;

	case NCOM_COMMAND_CMD_ID_CLEAR_FLAGS:
		break;

	case NCOM_COMMAND_CMD_ID_SET_MODE:
		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_DEPTH_MM:

		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_ROLL_DEG:
		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_PITCH_DEG:
		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_YAW_DEG:
		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_SURGE_MM_S:
		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_SWAY_MM_S:
		break;

	case NCOM_COMMAND_CMD_ID_SET_TARGET_HEAVE_MM_S:
		break;

	case NCOM_COMMAND_CMD_ID_SET_LIGHTS:
		break;

	case NCOM_COMMAND_CMD_ID_REBOOT_MCU:
		break;

	case NCOM_COMMAND_CMD_ID_CALIBRATE_DEPTH_0:
		break;

	case NCOM_COMMAND_CMD_ID_CALIBRATE_AXES_0:
		break;

	case NCOM_COMMAND_CMD_ID_SET_HEARTBEAT_RATE:
		break;

	case NCOM_COMMAND_CMD_ID_EMERGENCY:
		break;

	default:
		break;
	}

	return true;
}

bool NCOM_Handlers_ConfigReq(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetMcu(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetFreertos(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetSensor(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetActuator(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetPID(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetEKF(NCOM_RX_t *rx){

	return true;
}

bool NCOM_Handlers_ConfigSetStartup(NCOM_RX_t *rx){

	return true;
}
