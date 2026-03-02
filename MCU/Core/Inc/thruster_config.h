#ifndef INC_THRUSTER_CONFIG_H_
#define INC_THRUSTER_CONFIG_H_

/**
 * Thruster Allocation Matrix (TAM)
 * Defines the mapping from Body Frame Forces/Torques (Surge, Sway, Heave, Roll,
 * Pitch, Yaw) to individual thruster forces. Array size MUST match the physical
 * configuration (e.g., [8][6] for an 8-thruster AUV)
 */
#define AUV_TAM_MATRIX                                                         \
  {{0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 1.15f},                                    \
   {0.25f, -0.25f, 0.0f, 0.0f, 0.0f, -1.15f},                                  \
   {-0.25f, 0.25f, 0.0f, 0.0f, 0.0f, -1.15f},                                  \
   {-0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.15f},                                  \
   {0.0f, 0.0f, 0.25f, 1.15f, 1.15f, 0.0f},                                    \
   {0.0f, 0.0f, 0.25f, -1.15f, 1.15f, 0.0f},                                   \
   {0.0f, 0.0f, 0.25f, 1.15f, -1.15f, 0.0f},                                   \
   {0.0f, 0.0f, 0.25f, -1.15f, -1.15f, 0.0f}}

/**
 * PWM Generation Constants
 * Parameters used to convert a desired Force (Newtons) into a PWM signal
 * (microseconds) Equation generally follows: PWM = Center +/- (Coef_Sqrt *
 * sqrt(|F|) + Coef_Lin * |F|)
 */
#define AUV_THRUST_MAX 40.0f  // Maximum force per thruster (Newtons)
#define AUV_THRUST_MIN -40.0f // Minimum force per thruster (Newtons)
#define AUV_THRUST_DEADBAND                                                    \
  0.15f // Forces below this magnitude are ignored (Newtons)

#define AUV_PWM_CENTER 1500 // Neutral/Stopped PWM value (us)
#define AUV_PWM_MAX 1900    // Maximum Forward PWM value (us)
#define AUV_PWM_MIN 1100    // Maximum Reverse PWM value (us)

#define AUV_THRUST_TO_PWM_COEF_SQRT                                            \
  12.5f // Coefficient for the square root term of the thrust curve
#define AUV_THRUST_TO_PWM_COEF_LIN                                             \
  3.2f // Coefficient for the linear term of the thrust curve

#endif /* INC_THRUSTER_CONFIG_H_ */
