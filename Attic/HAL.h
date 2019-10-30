#ifndef HAL_H_INCLUDED
#define HAL_H_INCLUDED

#include "Config.h"

void HAL_Initialize();
int64_t HAL_GetTimeInt();
void HAL_SetDigitalOn(int index);
void HAL_SetDigitalOff(int index);
void HAL_PrintLn(const char* buf);


#endif


