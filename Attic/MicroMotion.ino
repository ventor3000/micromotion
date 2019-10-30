
#include "HAL.h"
#include "Machine.h"
#include "Motor.h"
#include "Config.h"


static bool ledOn = false;

static TMachine machine;





char debugbuf[100];
void DebugCallback(COORDINATE x, COORDINATE y) {
  // sprintf(debugbuf,"X:%d Y:%d\n",x,y);
  // HAL_PrintLn(debugbuf);
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);


  HAL_Initialize();
  Machine_Init(&machine);
  machine.DebugCallback = DebugCallback;


  if(MOTOR0_STEP>=0) {pinMode(MOTOR0_STEP,OUTPUT);digitalWrite(MOTOR0_STEP,LOW);}
  if(MOTOR0_DIR>=0) {pinMode(MOTOR0_DIR,OUTPUT);digitalWrite(MOTOR0_DIR,LOW);}
  if(MOTOR0_ENABLE>=0) {pinMode(MOTOR0_ENABLE,OUTPUT);digitalWrite(MOTOR0_ENABLE,LOW);}
  if(MOTOR1_STEP>=0) {pinMode(MOTOR1_STEP,OUTPUT);digitalWrite(MOTOR1_STEP,LOW);}
  if(MOTOR1_DIR>=0) {pinMode(MOTOR1_DIR,OUTPUT);digitalWrite(MOTOR1_DIR,LOW);}
  if(MOTOR1_ENABLE>=0) {pinMode(MOTOR1_ENABLE,OUTPUT);digitalWrite(MOTOR1_ENABLE,LOW);}
  if(MOTOR2_STEP>=0) {pinMode(MOTOR2_STEP,OUTPUT);digitalWrite(MOTOR2_STEP,LOW);}
  if(MOTOR2_DIR>=0) {pinMode(MOTOR2_DIR,OUTPUT);digitalWrite(MOTOR2_DIR,LOW);}
  if(MOTOR2_ENABLE>=0) {pinMode(MOTOR2_ENABLE,OUTPUT);digitalWrite(MOTOR2_ENABLE,LOW);}

  
}



void loop() {


  COORDINATE endpos[AXIS_COUNT];
  memset(endpos, 0, AXIS_COUNT * sizeof(COORDINATE));

  
  
  

  Motor_SetPins(&machine.motors[0], MOTOR0_STEP, MOTOR0_DIR, MOTOR0_ENABLE);
  Motor_SetPins(&machine.motors[1], MOTOR1_STEP, MOTOR1_DIR, MOTOR1_ENABLE);
  Motor_SetPins(&machine.motors[2], MOTOR2_STEP, MOTOR2_DIR, MOTOR2_ENABLE);

  machine.DebugCallback = DebugCallback;

  
  endpos[0] = 3200;
  endpos[1] = 0;
  endpos[2] = 0;

  Machine_QueInterpolation(&machine, 300000, endpos);

  endpos[0] = 0;
  endpos[1] = 0;
  endpos[2] = 0;

  Machine_QueInterpolation(&machine, 300000, endpos);


  endpos[0] = 1600;
  endpos[1] = 0;
  endpos[2] = 0;

  Machine_QueInterpolation(&machine, 200000, endpos);


  
  int64_t t0 = HAL_GetTimeInt();
  //FixedMachine_StartCycle(&machine);
  Machine_StartCycle(&machine);
  int64_t t1 = HAL_GetTimeInt();
  
  printf("%lld\n",t1-t0);

  
    delay(300);

}
