#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#include "Config.h"

struct TMotor {
  bool stepPulse;
  COORDINATE dir;
  COORDINATE position;
  
  int pinEnable;
  int pinStep;
  int pinDirection;

};

void Motor_Init(struct TMotor* m);
void Motor_SetPins(struct TMotor* m, int stepPin, int dirPin, int enablePin);
void Motor_SetStepPulseOn(struct TMotor* m);
void Motor_SetStepPulseOff(struct TMotor* m);
void Motor_SetDirection(struct TMotor* m,COORDINATE dir);



#endif

