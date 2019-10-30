#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

struct TRingBuffer {
  struct TTrajectoryNode* nodes;
  //int capacity; // number of nodes allocated in above pointer
  int first; // index of the first unpopped node in buffer
  int last; // always kept larger the first
};


struct TRingBuffer* RingBuf_Create();
struct TTrajectoryNode* RingBuf_Push(struct TRingBuffer* r);
struct TTrajectoryNode* RingBuf_Pop(struct TRingBuffer* r);
int RingBuf_SlotsAvailable(struct TRingBuffer* r);


#endif
