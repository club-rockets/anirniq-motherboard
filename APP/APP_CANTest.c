/*
 * APP_CANTest.c
 *
 *  Created on: 23 mars 2019
 *      Author: Marc-Andre
 */


#include "APP_CANTest.h"
#include "BSP_CAN.h"
#include "canDriver.h"

void tsk_CANTest(void const * argument){
	can_regData_u data = {.UINT32_T = 123};
	canRXpacket_t p;

	can_regData_u dat;
	can_canInit();

	while(1){
		can_canSetRegisterLoopback(1,0,&data);
		osDelay(1000);
		can_getRegisterData(1,0,&dat);
	}
}
