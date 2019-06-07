/*
 * heartBeat.h
 *
 *  Created on: 14 mars 2019
 *      Author: Marc-Andre
 *
 *      This thread blinks a led to show that the board is alive
 *
 */


#include "APP_heartBeat.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "bsp_can.h"
#include "bsp_can_regdef.h"
#include "enums.h"
#include "id.h"

void tsk_heartBeat(void const * argument){

	can_regData_u statusData = {0};

	while(1){
		HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,1);
		osDelay(50);
		HAL_GPIO_WritePin(LED_4_GPIO_Port,LED_4_Pin,0);
		statusData.INT32_T = 1;
		can_canSetRegisterData(CAN_MOTHERBOARD_STATUS_INDEX,&statusData);
		osDelay(1000);
	}
}
