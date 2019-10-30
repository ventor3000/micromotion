#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED


#include "Config.h"

struct TMachine {
	struct TMotor* motors;
	struct TRingBuffer* ringBuffer;
	void(*DebugCallback)(COORDINATE x,COORDINATE y);
};


void Machine_Init(struct TMachine* machine);
bool Machine_QueInterpolation(struct TMachine* m, int64_t microseconds, COORDINATE* endpos);
bool Machine_QueSetDigital(struct TMachine* m, int index, bool on);
bool Machine_QueWait(struct TMachine* m, long milliSeconds);
void Machine_StartCycle(struct TMachine* m);



#endif


