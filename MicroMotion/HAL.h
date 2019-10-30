#ifndef HAL_H_INCLUDED
#define HAL_H_INCLUDED

#include "Config.h"

void HAL_Initialize(struct TMachine* m,void(*timerCallback)(struct TMachine* m));

void HAL_SetDigitalOn(int index);
void HAL_SetDigitalOff(int index);
void HAL_PrintLn(const char* buf);
void HAL_SetTimer(int64_t microSeconds);



#endif



