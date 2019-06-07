/*
 * APP_CANTest.c
 *
 *  Created on: 23 mars 2019
 *      Author: Marc-Andre
 */


#include "APP_CANTest.h"
#include "bsp_can.h"
#include "can/can_driver.h"
#include "bsp_can_regdef.h"

void sendDataYeild(uint32_t board,uint32_t index,can_regData_u* data){
	while(!can_canSetRegisterLoopback(board,index,data)){
		osThreadYield();
	}
}

void tsk_CANTest(void const * argument){
	can_regData_u data = {.UINT32_T = 123};
	//canRXpacket_t p;

	can_regData_u dat;
	can_canInit();

	while(1){
		osDelay(1000);
	}

	while(1){
		data.UINT32_T = 0x123;
		can_canSetRegisterLoopback(1,0,&data);
		data.UINT32_T = 1;
		//can_canSetRegisterData(0,&data);
		sendDataYeild(MOTHERBOARD,CAN_MOTHERBOARD_STATUS_INDEX,&data);
		sendDataYeild(MISSION,CAN_MISSION_STATUS_INDEX,&data);
		sendDataYeild(COMMUNICATION,CAN_COMMUNICATION_STATUS_INDEX,&data);
		sendDataYeild(ACQUISITION,CAN_ACQUISITION_STATUS_INDEX,&data);
		osDelay(1000);
		data.UINT32_T = 2;
		sendDataYeild(MOTHERBOARD,CAN_MOTHERBOARD_STATUS_INDEX,&data);
		sendDataYeild(MISSION,CAN_MISSION_STATUS_INDEX,&data);
		sendDataYeild(COMMUNICATION,CAN_COMMUNICATION_STATUS_INDEX,&data);
		sendDataYeild(ACQUISITION,CAN_ACQUISITION_STATUS_INDEX,&data);
		osDelay(1000);
		can_getRegisterData(1,0,&dat);
	}
}
