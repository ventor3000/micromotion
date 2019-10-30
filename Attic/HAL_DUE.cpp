
// Hardware abstraction layer for arduino due


#include "Config.h"

#ifdef HAL_DUE

#include <DueTimer.h>
#include <Arduino.h>

static volatile int64_t baseTimer;
#define TIMER_INTERVAL 5

void timerHandler(){
  baseTimer+=TIMER_INTERVAL;
}


void HAL_Initialize() {
  Timer3.attachInterrupt(timerHandler);
  Timer3.start(TIMER_INTERVAL);
  Serial.begin(115200);
}


int64_t HAL_GetTimeInt() {
  return baseTimer;
}

void HAL_SetDigitalOn(int index) {
  // faster than digital write:
  // https://forum.arduino.cc/index.php?topic=175617.0
  g_APinDescription[index].pPort -> PIO_SODR = g_APinDescription[index].ulPin;
}

void HAL_SetDigitalOff(int index) {
	// faster than digital write:
	// https://forum.arduino.cc/index.php?topic=175617.0
	g_APinDescription[index].pPort->PIO_CODR = g_APinDescription[index].ulPin;
}


void HAL_PrintLn(const char* buf) {
  Serial.println(buf);
}

#endif // HAL_DUE
