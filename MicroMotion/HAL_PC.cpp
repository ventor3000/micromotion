
// Hardware abstraction layer for PC console

#if defined(_WIN32) // only use in windows

#include "Config.h"
#include "HAL.h"
#include <stdio.h>
#include <Windows.h>


static int64_t fixedPerfFreq=1;		// performance timer frequency
static int64_t nextTickTime;		// when we want the next timer callback to happen
static HANDLE timerThreadHandle;	// handle to thread managing timer
static int64_t timerMicroseconds;	// delay between timer ticks, if <=0  timer is stopped
static void(*timerCallback)(struct TMachine* m) = NULL; // the function we call on timer tick



static int64_t GetMicrosecondsRaw() {
	__int64 cnt;
	QueryPerformanceCounter((LARGE_INTEGER*)&cnt);
	return cnt * 1000000LL / fixedPerfFreq;
}

static DWORD WINAPI TimerThreadFunction(LPVOID lpParam)
{

	TMachine* machine = (TMachine*)lpParam;

	while (true) {

		if (timerMicroseconds > 0) { // timer not stopped
			int64_t curt = GetMicrosecondsRaw();

			if (curt - nextTickTime >= timerMicroseconds) {
				nextTickTime = curt;
				timerCallback(machine);
				//printf("\n%lld",curt);
			}
		}
	}
}

void HAL_Initialize(struct TMachine* m, void(*callback)(struct TMachine* m)) {
	QueryPerformanceFrequency((LARGE_INTEGER*)&fixedPerfFreq);
	
	timerCallback = callback;
	timerMicroseconds = 0; // 0 => timer is stopped
	nextTickTime = 0; // no need to set because timer is stopped but we zero anyway...

	timerThreadHandle = CreateThread(NULL, 0, TimerThreadFunction, m, 0, NULL);

	if (timerThreadHandle == NULL)
		HAL_PrintLn("Failed to create timer thread");
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

void HAL_SetTimer(int64_t microSeconds) {
	timerMicroseconds = microSeconds;

	if (timerMicroseconds > 0) {
		nextTickTime = GetMicrosecondsRaw() + timerMicroseconds;
	}
}

#endif // WINDOWS

