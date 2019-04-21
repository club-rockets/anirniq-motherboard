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

void tsk_heartBeat(void const * argument){

	while(1){
		HAL_GPIO_WritePin(LED_1_GPIO_Port,LED_1_Pin,1);
		osDelay(50);
		HAL_GPIO_WritePin(LED_1_GPIO_Port,LED_1_Pin,0);
		osDelay(1000);
	}
}
