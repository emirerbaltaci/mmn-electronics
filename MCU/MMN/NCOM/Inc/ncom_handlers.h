#ifndef NCOM_INC_NCOM_HANDLERS_H_
#define NCOM_INC_NCOM_HANDLERS_H_

#include "ncom_rx.h"
#include "auvconfig.h"

bool NCOM_Handlers_Config_Set_Startup(NCOM_RX_t *rx, AUV_Config_t *auvConfig);
void NCOM_Handlers_Selector(NCOM_RX_t *rx);


#endif /* NCOM_INC_NCOM_HANDLERS_H_ */
