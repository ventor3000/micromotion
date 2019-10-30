
#include "Config.h"
#include "RingBuffer.h"
#include "TrajectoryNode.h"
#include <stdlib.h>

TRingBuffer* RingBuf_Create() {
  TRingBuffer* res=(TRingBuffer*)malloc(sizeof(TRingBuffer));
  res->nodes=(TTrajectoryNode*)malloc(sizeof(TTrajectoryNode)*RINGBUFFER_SIZE);
  res->first=0;
  res->last=0;
  return res;
}


TTrajectoryNode* RingBuf_Push(struct TRingBuffer* r)
{
    int a = RingBuf_SlotsAvailable(r);
    if (a <= 0)
        return NULL;

    TTrajectoryNode* res = &r->nodes[r->last % RINGBUFFER_SIZE];
    r->last++;
    return res;
}


TTrajectoryNode* RingBuf_Pop(struct TRingBuffer* r)
{
    if (r->first == r->last) // empty buffer
        return NULL;

    // grab result
    TTrajectoryNode* res = &r->nodes[r->first];
    r->first++;

    // keep first between 0 and capacity. Keep last larger.
    
    if (r->first >= RINGBUFFER_SIZE)
    {  
        // normalize index
        r->first -= RINGBUFFER_SIZE;
        r->last -= RINGBUFFER_SIZE;
    }

    
    return res;
}

int RingBuf_SlotsAvailable(struct TRingBuffer* r)
{
    return RINGBUFFER_SIZE - (r->last - r->first); // note last is always kept larger than first!
}


