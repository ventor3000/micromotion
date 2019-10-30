#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdint.h>


#define MOTOR_STEP_TIME_US 5.0
#define RINGBUFFER_SIZE 256


#define COORDINATE int


// the following should really be set in an separate GRBL definition file
#define AXIS_COUNT 8

#define MOTOR0_STEP		2
#define MOTOR0_DIR		5
#define MOTOR0_ENABLE	8
#define MOTOR1_STEP		3
#define MOTOR1_DIR		6
#define MOTOR1_ENABLE	8
#define MOTOR2_STEP		4
#define MOTOR2_DIR		7
#define MOTOR2_ENABLE	8




#endif


