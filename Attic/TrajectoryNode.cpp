
#include "TrajectoryNode.h"
#include "HAL.h"
#include <string.h>
#include "DDA.h"
#include "Motor.h"


bool TrajectoryNode_BeginInterpolation(struct TMachine* machine, int64_t microSeconds, COORDINATE* endpos);
bool TrajectoryNode_UpdateInterpolation(struct TMachine* machine);


// timing variables for interpolation
static int64_t lastFlankTime = 0;
static int64_t nextPulseTime = 0;
static int64_t timePerFlank = 0;
static bool motorPulse = false; // false means all motors in low state, true means some motors might have high step pulse (tha major motor is always true in this case)



void TrajectoryNode_InitInterpolation(struct TTrajectoryNode* res, int64_t microseconds, COORDINATE* endpos) {
	res->type = TRN_INTERPOLATE;
	memcpy(res->endpos, endpos, sizeof(COORDINATE) * AXIS_COUNT);
	res->microSeconds = microseconds;
}



void TrajectoryNode_InitSetDigital(struct TTrajectoryNode* res,int index, bool on) {
  res->type = TRN_SET_DIGITAL;
  res->ioindex = index;
  res->on = on;
}


bool TrajectoryNode_Begin(struct TTrajectoryNode* n, struct TMachine* m) {
	switch (n->type) {
	case TRN_INTERPOLATE:
		return TrajectoryNode_BeginInterpolation(m, n->microSeconds, n->endpos);
	case TRN_SET_DIGITAL:
		if (n->ioindex >= 0) {
			// TODO: protect from to large indices
			if (n->on)
				HAL_SetDigitalOn(n->ioindex);
			else
				HAL_SetDigitalOff(n->ioindex);
		}
		return true; // done already
	default:
		// dont know what this is, actually an error, this cannot happen
		return true; // just skip
	}
}


bool TrajectoryNode_Update(struct TTrajectoryNode* n, struct TMachine* m) {
	switch (n->type) {
	case TRN_INTERPOLATE:
		return TrajectoryNode_UpdateInterpolation(m);
	case TRN_SET_DIGITAL:
		return true; // pointless to call update on this
	default:
		return true; // dont know what this is, this should never happen, skip it
	}
}



bool TrajectoryNode_BeginInterpolation(struct TMachine* machine, int64_t microSeconds, COORDINATE* endpos) {
	int64_t startTime,timePerStep;
	COORDINATE numSteps;


	numSteps = DDA_Init(machine->motors, endpos);

	if (numSteps == 0)
		return true; // done => no steps to do

	timePerStep = microSeconds / numSteps;
	timePerFlank = timePerStep >> 1; // half time per flank than time per step!
	startTime = HAL_GetTimeInt();
	nextPulseTime = startTime + timePerFlank;

	return false; // not done yet
}



bool TrajectoryNode_UpdateInterpolation(struct TMachine* machine) {
	int i;
	int64_t time = HAL_GetTimeInt(); // TODO: reset time and run from zero in each step

	if (time < nextPulseTime)
		return false;
	nextPulseTime += timePerFlank;

	if (motorPulse) {  // hi=>lo flank
		for (i = 0; i < AXIS_COUNT; i++)
			Motor_SetStepPulseOff(&machine->motors[i]);
		motorPulse = false;
		return false;
	}
	else { // lo=>hi flank, motors move!
		bool res = DDA_Step();

		if (machine->DebugCallback)
			machine->DebugCallback(machine->motors[0].position, machine->motors[1].position);

		motorPulse = true;

		return res;

	}

}
