#include "auvflags.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ncom_protocol.h"
#include "ncom_tx.h"

// Defined in main.c
extern volatile AUV_VehicleState_t vehicleStatus;
extern bool isMagUpdated;
extern bool isDepthUpdated;

extern volatile uint8_t hbStateEstimateTask;
extern volatile uint8_t hbControlTask;
extern volatile uint8_t hbNCOMTask;

uint8_t ncom_flag_bit_meanings[32] = {0};
uint32_t ncom_flag_bit_interrupts_mask = 0;

void AUV_Flags_Update(void) {
    uint32_t oldFlags = configurableFlags;
    uint32_t newFlags = 0;

    for (int i = 0; i < 32; i++) {
        uint8_t meaning = ncom_flag_bit_meanings[i];
        bool isSet = false;

        switch (meaning) {
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_TASK_HB_STATEESTIMATE:
                isSet = (hbStateEstimateTask == AUV_TASK_ALIVE);
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_TASK_HB_CONTROL:
                isSet = (hbControlTask == AUV_TASK_ALIVE);
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_TASK_HB_NCOM:
                isSet = (hbNCOMTask == AUV_TASK_ALIVE);
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_IS_ARMED:
                isSet = (vehicleStatus == AUV_ARMED || vehicleStatus == AUV_ARMED_MANUAL || vehicleStatus == AUV_ARMED_ERR || vehicleStatus == AUV_ARMED_MANUAL_ERR);
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_IS_PREARMED:
                isSet = (vehicleStatus == AUV_PREARMED || vehicleStatus == AUV_PREARMED_MANUAL || vehicleStatus == AUV_PREARMED_ERR || vehicleStatus == AUV_PREARMED_MANUAL_ERR);
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_MISSION_RUNNING:
                isSet = (vehicleStatus == AUV_MISSION);
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_MAG_VALID:
                isSet = isMagUpdated;
                break;
            case NCOM_ASSIGN_FLAG_BIT_BIT_MEANING_BAR30_VALID:
                isSet = isDepthUpdated;
                break;
            default:
                break;
        }

        if (isSet) {
            newFlags |= (1 << i);
        }
    }

    taskENTER_CRITICAL();
    configurableFlags = newFlags;
    taskEXIT_CRITICAL();

    uint32_t changedFlags = (oldFlags ^ newFlags);
    uint32_t triggerFlags = (changedFlags & newFlags) & ncom_flag_bit_interrupts_mask;

    if (triggerFlags) {
        NCOM_Payload_SEND_FLAG_t msg;
        msg.flag_uint = newFlags;
        uint8_t buf[NCOM_LEN_SEND_FLAG];
        ncom_pack_send_flag(buf, &msg);
        NCOM_TX_SendPacket(NCOM_MSG_SEND_FLAG, buf, NCOM_LEN_SEND_FLAG);
    }
}
