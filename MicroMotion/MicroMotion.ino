
#include "HAL.h"
#include "Machine.h"
#include "Motor.h"
#include "Config.h"
#include "CmdParse.h"

static bool ledOn = false;

static TMachine machine;



char debugbuf[100];
void DebugCallback(COORDINATE x, COORDINATE y) {
  //sprintf(debugbuf, "X:%d Y:%d Z:%d\n", x, y, machine.motors[2].position);
  //HAL_PrintLn(debugbuf);
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Machine_Init(&machine);
  machine.DebugCallback = DebugCallback;


  if (MOTOR0_STEP >= 0) {
    pinMode(MOTOR0_STEP, OUTPUT);
    digitalWrite(MOTOR0_STEP, LOW);
  }
  if (MOTOR0_DIR >= 0) {
    pinMode(MOTOR0_DIR, OUTPUT);
    digitalWrite(MOTOR0_DIR, LOW);
  }
  if (MOTOR0_ENABLE >= 0) {
    pinMode(MOTOR0_ENABLE, OUTPUT);
    digitalWrite(MOTOR0_ENABLE, LOW);
  }
  if (MOTOR1_STEP >= 0) {
    pinMode(MOTOR1_STEP, OUTPUT);
    digitalWrite(MOTOR1_STEP, LOW);
  }
  if (MOTOR1_DIR >= 0) {
    pinMode(MOTOR1_DIR, OUTPUT);
    digitalWrite(MOTOR1_DIR, LOW);
  }
  if (MOTOR1_ENABLE >= 0) {
    pinMode(MOTOR1_ENABLE, OUTPUT);
    digitalWrite(MOTOR1_ENABLE, LOW);
  }
  if (MOTOR2_STEP >= 0) {
    pinMode(MOTOR2_STEP, OUTPUT);
    digitalWrite(MOTOR2_STEP, LOW);
  }
  if (MOTOR2_DIR >= 0) {
    pinMode(MOTOR2_DIR, OUTPUT);
    digitalWrite(MOTOR2_DIR, LOW);
  }
  if (MOTOR2_ENABLE >= 0) {
    pinMode(MOTOR2_ENABLE, OUTPUT);
    digitalWrite(MOTOR2_ENABLE, LOW);
  }


  Motor_SetPins(&machine.motors[0], MOTOR0_STEP, MOTOR0_DIR, MOTOR0_ENABLE);
  Motor_SetPins(&machine.motors[1], MOTOR1_STEP, MOTOR1_DIR, MOTOR1_ENABLE);
  Motor_SetPins(&machine.motors[2], MOTOR2_STEP, MOTOR2_DIR, MOTOR2_ENABLE);


  

  machine.DebugCallback = DebugCallback;


  
}



void MoveTo(COORDINATE x,COORDINATE y,COORDINATE z,COORDINATE t) {
    COORDINATE endpos[AXIS_COUNT];
    memset(endpos, 0, AXIS_COUNT * sizeof(COORDINATE));
    endpos[0]=x;
    endpos[1]=y;
    endpos[2]=z;
    Machine_QueInterpolation(&machine,t,endpos);
}

bool first = true;

void loop() {

  if (first) {

    CmdParse_ExecLine(&machine,"LI X6400");
    
    CmdParse_ExecLine(&machine,"BG");

    first=false;
    /*
    MoveTo(3200,0,100,1000000); 
    Machine_QueDwell(&machine,500);
    MoveTo(0,0,0,1000000); 
    Machine_QueDwell(&machine,1500);
    MoveTo(6400,6400,6400,1000000); 
    Machine_StartCycle(&machine);
    first = false;*/
  }
}



