#ifndef VEHICLE_INC_AUVFLAGS_H_
#define VEHICLE_INC_AUVFLAGS_H_

typedef enum{
	AUV_ARMED,
	AUV_PREARMED,
	AUV_DISARMED
}AUV_ArmState_t;

typedef enum{
	AUV_TASK_DEAD,
	AUV_TASK_ALIVE
}AUV_TaskState_t;

#endif /* VEHICLE_INC_AUVFLAGS_H_ */
