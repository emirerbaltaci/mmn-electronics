#include "ncom_rx.h"
#include "auvconfig.h"

typedef void (*NCOM_Handler)(NCOM_RX_t *rx);

bool NCOM_Handlers_Config_Set_Startup(NCOM_RX_t *rx, AUV_Config_t *auvConfig){

	NCOM_Payload_CONFIG_SET_STARTUP_t msg;
	ncom_unpack_config_set_startup(rx->parser.payloadBuf, &msg);

	return true;
}

static void NCOM_Handlers_Command(NCOM_RX_t *rx){

	NCOM_Payload_COMMAND_t msg;
	ncom_unpack_command(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Req(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_REQ_t msg;
	ncom_unpack_config_req(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Set_Mcu(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_SET_MCU_t msg;
	ncom_unpack_config_set_mcu(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Set_Freertos(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_SET_FREERTOS_t msg;
	ncom_unpack_config_set_freertos(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Set_Sensor(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_SET_SENSOR_t msg;
	ncom_unpack_config_set_sensor(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Set_Actuator(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_SET_ACTUATOR_t msg;
	ncom_unpack_config_set_actuator(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Set_PID(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_SET_PID_t msg;
	ncom_unpack_config_set_pid(rx->parser.payloadBuf, &msg);
}

static void NCOM_Handlers_Config_Set_EKF(NCOM_RX_t *rx){

	NCOM_Payload_CONFIG_SET_EKF_t msg;
	ncom_unpack_config_set_ekf(rx->parser.payloadBuf, &msg);
}

static const NCOM_Handler NCOM_HandlersTable[256] __attribute__((section(".ccmram"))) = {
		[NCOM_MSG_COMMAND] = NCOM_Handlers_Command,
		[NCOM_MSG_CONFIG_REQ] = NCOM_Handlers_Config_Req,
		[NCOM_MSG_CONFIG_SET_MCU] = NCOM_Handlers_Config_Set_Mcu,
		[NCOM_MSG_CONFIG_SET_FREERTOS] = NCOM_Handlers_Config_Set_Freertos,
		[NCOM_MSG_CONFIG_SET_SENSOR] = NCOM_Handlers_Config_Set_Sensor,
		[NCOM_MSG_CONFIG_SET_ACTUATOR] = NCOM_Handlers_Config_Set_Actuator,
		[NCOM_MSG_CONFIG_SET_PID] = NCOM_Handlers_Config_Set_PID,
		[NCOM_MSG_CONFIG_SET_EKF] = NCOM_Handlers_Config_Set_EKF
};

void NCOM_Handlers_Selector(NCOM_RX_t *rx){

	uint8_t id = rx->parser.msgId;

	if(NCOM_HandlersTable[id] != NULL) NCOM_HandlersTable[id](rx);
	else rx->stats.unknownIDErrors++;
}
