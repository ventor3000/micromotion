#include "Machine.h"
#include "Config.h"
#include "Motor.h"
#include "RingBuffer.h"
#include "TrajectoryNode.h"
#include "HAL.h"
#include <stdlib.h>

static void Machine_UpdateByTimer(struct TMachine* m);

static TTrajectoryNode* currentNode=NULL; // the current node that is executed or null if none



void Machine_Init(struct TMachine* machine) {

  HAL_Initialize(machine,Machine_UpdateByTimer);
  
  machine->motors =  (TMotor*)malloc(AXIS_COUNT*sizeof(TMotor));

  for(int i= 0;i < AXIS_COUNT;i++) 
    Motor_Init(&machine->motors[i]);

  machine->ringBuffer=RingBuf_Create();
  machine->DebugCallback = NULL;
}

bool Machine_QueInterpolation(struct TMachine* m, int64_t microseconds, COORDINATE* relMove)
{
	TTrajectoryNode* n = RingBuf_Push(m->ringBuffer);
	if (n == NULL)
		return false;
	TrajectoryNode_InitInterpolation(n, microseconds, relMove);
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



bool Machine_QueWait(struct TMachine* m, long milliseconds)
{
    TTrajectoryNode* n = RingBuf_Push(m->ringBuffer);
    if (n == NULL)
        return false;
    n->type=TRN_DWELL;
    n->milliseconds=milliseconds;

    
    return true;
}




static void Machine_UpdateByTimer(struct TMachine* m) {
    
    if(currentNode==NULL) {
      currentNode = RingBuf_Pop(m->ringBuffer);
      if(TrajectoryNode_Begin(currentNode, m)) { // note: begin function handles NULL nodes from ringbuffer as done!
        // done already
        currentNode=NULL;
        return;
      }
    }

    // if we come here, we know we have a trajectory node, and it is not done yet
    if(TrajectoryNode_Update(currentNode, m)) {
      currentNode=NULL; // done with the current node. NULL it for next timer call      
    }
}

void Machine_StartCycle(struct TMachine* m) {
  HAL_SetTimer(500000); // start within one millisecond
}


