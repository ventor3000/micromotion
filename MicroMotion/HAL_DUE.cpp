
// Hardware abstraction layer for arduino due

#if defined __SAM3X8E__  // only use on due



#include "Config.h"
#include "HAL.h"


#include <Arduino.h>

//static volatile int64_t baseTimer;
//#define TIMER_INTERVAL 5



static Tc* timer_tc = TC1;         // we user TC1 which contains timer irq 3
static uint32_t timer_channel = 0; // timer 3 is on channel 0
static IRQn_Type timer_irq = TC3_IRQn; // timer 3 irq
static void(*timer_callback)(struct TMachine* m) = NULL;

static struct TMachine* machine; // our global machine used in timer callbacks

/*void timerHandler(){
  baseTimer+=TIMER_INTERVAL;
  }*/


//TC1 ch 0
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  //digitalWrite(13, l = !l);

  timer_callback(machine);
  //HAL_PrintLn("Hello timer!");
}



static void initTimer() {

  // this code from https://forum.arduino.cc/index.php?topic=130423.90
  // also some good information at http://2manyprojects.net/timer-interrupts

  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)timer_irq);
  TC_Configure(timer_tc, timer_channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2);
  //uint32_t rc = VARIANT_MCK/8/frequency; //8 because we selected TIMER_CLOCK2 above, so we have a 10.5 MHz timer
  //TC_SetRA(tc, channel, rc/2); //50% high, 50% low
  // TC_SetRC(tc, channel, rc);
  //TC_Start(tc, channel);

  TC_Stop(timer_tc, timer_channel); // make sure timer is stopped, we start it with HAL_SetFrequency
  timer_tc->TC_CHANNEL[timer_channel].TC_IER = TC_IER_CPCS;
  timer_tc->TC_CHANNEL[timer_channel].TC_IDR = ~TC_IER_CPCS;
  NVIC_EnableIRQ(timer_irq);
}


void HAL_Initialize(struct TMachine* m, void(*timerCallback)(struct TMachine* m)) {


  /*Timer3.attachInterrupt(timerHandler);
    Timer3.start(TIMER_INTERVAL);*/

  timer_callback = timerCallback;
  machine = m;
  initTimer();
  Serial.begin(115200);

}





void HAL_SetTimer(int64_t microSeconds) {


  TC_Stop(timer_tc, 0); // have to stop timer to safely change RC

  if (microSeconds <= 0)
    return;

  //int64_t rc=VARIANT_MCK/8/frequency;
  int64_t rc = VARIANT_MCK * microSeconds / 8000000; // 8*1000000, 8 is for timer divisor for 10.5 Mhz timer
  TC_SetRC(timer_tc, 0, rc);


  TC_Start(timer_tc, 0);

}

void HAL_AdjustTimer(int64_t microSeconds) {

  //int64_t rc=VARIANT_MCK/8/frequency;
  int64_t rc = VARIANT_MCK * microSeconds / 8000000; // 8*1000000, 8 is for timer divisor for 10.5 Mhz timer
  TC_SetRC(timer_tc, 0, rc);
}


/*
  int64_t HAL_GetTimeInt() {
  return baseTimer;
  }*/

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



#endif // _SAM3XA_
