#ifndef TRAJECTORYNODE_H_INCLUDED
#define TRAJECTORYNODE_H_INCLUDED


#include "Config.h"
#include "Machine.h"

enum TrajectoryType {
        TRN_NONE,
        TRN_INTERPOLATE,
        TRN_DWELL,
        TRN_SET_DIGITAL
};

struct TTrajectoryNode {
  TrajectoryType type; // enum TrajectoryTypeT
  
  union {
    int ioindex;  // set_digital / set_analog
    COORDINATE endpos[AXIS_COUNT]; // interpolate
    
  };

  union {
    bool on;        //set_digital
	  int64_t microSeconds; // interpolate
    long milliseconds; // dwell
  };
};

void TrajectoryNode_InitInterpolation(struct TTrajectoryNode* res, int64_t microseconds, COORDINATE* endpos);
void TrajectoryNode_InitSetDigital(struct TTrajectoryNode* res,int index, bool on);
bool TrajectoryNode_Begin(struct TTrajectoryNode* n, struct TMachine* m);
bool TrajectoryNode_Update(struct TTrajectoryNode* n, struct TMachine* m);

#endif

