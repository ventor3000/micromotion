#include "Motor.h"
#include "HAL.h"
#include "Config.h"


void Motor_Init(struct TMotor* m)
{
    m->stepPulse = false;
    //m->direction = false;
	m->dir = 0;
	m->position = 0;

	m->pinDirection = -1;
	m->pinEnable = -1;
	m->pinStep = -1;
}


void Motor_SetPins(struct TMotor* m, int stepPin, int dirPin, int enablePin) {
	m->pinStep = stepPin;
	m->pinDirection = dirPin;
	m->pinEnable = enablePin;
}

void Motor_SetStepPulseOn(struct TMotor* m) {
	// this function is assumed to actually move the motor, ie the motors position is changed
	if (m->pinStep >= 0 && m->stepPulse == false) {
		m->stepPulse = true;
		HAL_SetDigitalOn(m->pinStep);
		m->position += m->dir;
	}
}

void Motor_SetStepPulseOff(struct TMotor* m) {
	if (m->pinStep >= 0 && m->stepPulse == true) {
		m->stepPulse = false;
		HAL_SetDigitalOff(m->pinStep);
	}
}

void Motor_SetDirection(struct TMotor* m,COORDINATE dir) {
  if(m->pinDirection>=0 && m->dir!=dir) {
    m->dir=dir;
    if(dir<0)
      HAL_SetDigitalOff(m->pinDirection);
    else
      HAL_SetDigitalOn(m->pinDirection);
  }
}


