#ifndef MULTI_BRESENHAM_H_INCLUDED
#define MULTI_BRESENHAM_H_INCLUDED


#include "Config.h"

COORDINATE DDA_Init(struct TMotor* motors, COORDINATE* end);
bool DDA_Step();



#endif
