
// Hardware abstraction layer for PC console


#include "Config.h"

#ifdef HAL_PC

#include <stdio.h>
#include <Windows.h>


static int64_t fixedBaseTime;
static int64_t fixedPerfFreq;



int64_t GetMicrosecondsRaw() {
	__int64 cnt;
	QueryPerformanceCounter((LARGE_INTEGER*)&cnt);
	return cnt * 1000000LL / fixedPerfFreq;
}

void HAL_Initialize() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&fixedPerfFreq);
	fixedBaseTime = GetMicrosecondsRaw();
}



int64_t HAL_GetTimeInt() {
	return GetMicrosecondsRaw() - fixedBaseTime;
}


void HAL_SetDigitalOn(int index) {
	//TODO: implement
	int debug = 0;
}

void HAL_SetDigitalOff(int index) {
	// TODO: implement
	int debug = 0;
}



void HAL_PrintLn(const char* buf) {
	printf(buf);
}

#endif // HAL_PC
