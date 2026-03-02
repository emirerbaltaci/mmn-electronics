#ifndef INC_TASK_CONFIG_H_
#define INC_TASK_CONFIG_H_

/**
 * FreeRTOS Task Sleep / Period Configurations
 * Defines the loop rates for the various RTOS tasks (in milliseconds).
 */
#define TASK_STATEESTIMATE_SLEEP_MS 1 // 1000 Hz State Estimation Loop Rate
#define TASK_CONTROL_SLEEP_MS 5       // 200 Hz PID Control Loop Rate
#define TASK_NCOM_SLEEP_MS 20         // 50 Hz Communication/Telemetry Loop Rate
#define TASK_SENSOR_SLEEP_MS                                                   \
  100 // 10 Hz Slow Sensor (e.g., Barometer) Loop Rate
#define TASK_SYSMONITOR_SLEEP_MS 50 // 20 Hz System Monitoring Loop Rate

/**
 * Application Timing Constants
 */
#define TASK_CONTROL_PID_DT                                                    \
  0.005f // Delta Time used in PID derivative/integral calculations (Control
         // Rate -> 1/200Hz)

#endif /* INC_TASK_CONFIG_H_ */
