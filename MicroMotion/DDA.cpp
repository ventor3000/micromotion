#include "Config.h"
#include "Motor.h"
#include <math.h>
#include <stdio.h>






static COORDINATE err[AXIS_COUNT];	// current error
static COORDINATE errorDelta[AXIS_COUNT]; 
static int majorAxis;	// index of the dominant axis (ie. axis with largest move)
static COORDINATE stepsLeft;	// number of steps to execute on the major axis
static TMotor* motors; // the motors we are managing during current DDA run

COORDINATE absValue(COORDINATE v) { // in case abs does not exist foor the define COORDINATE type
	if (v < 0)
		return -v;
	return v;
}

COORDINATE DDA_Init(struct TMotor* _motors, COORDINATE* relEnd) {
	// returns true (done) if 0 steps left, ie. motor positions equals end positions
	
	COORDINATE absDelta;
	
	motors = _motors;
	stepsLeft = 0;

	for (int i = 0; i < AXIS_COUNT; i++) {
		
		absDelta = absValue(relEnd[i]); // absValue(end[i] - motors[i].position);

		//point[i] = start[i];
		errorDelta[i] = absDelta << 1;
		Motor_SetDirection(&motors[i], relEnd > 0 ? 1 : -1); // (end[i] > motors[i].position) ? 1 : -1);
		
		if (absDelta >= stepsLeft) {
			majorAxis = i;
			stepsLeft = absDelta;
		}
	}

	// compute the initial error
	for (int i = 0; i < AXIS_COUNT; i++) 
		err[i] = errorDelta[i] - stepsLeft;

	return stepsLeft;
}

bool DDA_Step() {
	// returns true when done
	for (int i = 0; i < AXIS_COUNT; i++) {
		if (err[i] > 0) {
			//motors[i].position += inc[i];
			Motor_SetStepPulseOn(&motors[i]);
			err[i] -= errorDelta[majorAxis];
		}
		err[i] += errorDelta[i];
	}

	return --stepsLeft <= 0;
}

