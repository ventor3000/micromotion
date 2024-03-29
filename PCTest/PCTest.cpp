#include "HAL.h"
#include "Machine.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "DDA.h"
#include "Motor.h"
#include "CmdParse.h"



static TMachine machine;






void DebugCallback(COORDINATE x, COORDINATE y) {
	printf("X:%d Y:%d\n", x, y);
}


void setup() {
	Machine_Init(&machine);
	machine.DebugCallback = DebugCallback;


	

	Motor_SetPins(&machine.motors[0], MOTOR0_STEP, MOTOR0_DIR, MOTOR0_ENABLE);
	Motor_SetPins(&machine.motors[1], MOTOR1_STEP, MOTOR1_DIR, MOTOR1_ENABLE);
	Motor_SetPins(&machine.motors[2], MOTOR2_STEP, MOTOR2_DIR, MOTOR2_ENABLE);




	machine.DebugCallback = DebugCallback;
}





int main()
{


	setup();

	

	CmdParse_ExecLine(&machine, "LI X100T1000");
	CmdParse_ExecLine(&machine, "LI X0Y100T1000");
	CmdParse_ExecLine(&machine, "LI X-1000T1000");
	CmdParse_ExecLine(&machine, "LI Y-100T3000");
	CmdParse_ExecLine(&machine, "BG");


	_getch();
}
