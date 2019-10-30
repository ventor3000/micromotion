#include "Machine.h"
#include "Config.h"
#include "Motor.h"
#include "RingBuffer.h"
#include "TrajectoryNode.h"
#include "HAL.h"
#include <stdlib.h>



void Machine_Init(struct TMachine* machine) {
  
  machine->motors =  (TMotor*)malloc(AXIS_COUNT*sizeof(TMotor));

  for(int i= 0;i < AXIS_COUNT;i++) 
    Motor_Init(&machine->motors[i]);

  machine->ringBuffer=RingBuf_Create();
  machine->DebugCallback = NULL;
}

bool Machine_QueInterpolation(struct TMachine* m, int64_t microseconds, COORDINATE* endpos)
{
	TTrajectoryNode* n = RingBuf_Push(m->ringBuffer);
	if (n == NULL)
		return false;
	TrajectoryNode_InitInterpolation(n, microseconds, endpos);
	return true;
}


bool Machine_QueSetDigital(struct TMachine* m, int index, bool on)
{
    TTrajectoryNode* n = RingBuf_Push(m->ringBuffer);
    if (n == NULL)
        return false;
    TrajectoryNode_InitSetDigital(n, index, on);
    return true;
}


void Machine_StartCycle(struct TMachine* m) {
	TTrajectoryNode* current = NULL;
	while (true)
	{
		current = RingBuf_Pop(m->ringBuffer);
		if (current == NULL)
			break; // no more nodes available

		bool done = TrajectoryNode_Begin(current, m);

		while (!done)
		{
			done = TrajectoryNode_Update(current, m);
		}
	}
}

