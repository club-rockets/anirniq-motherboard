/*
 * APP_ledDriver.c
 *
 *  Created on: Apr 20, 2019
 *      Author: Marc-Andre
 */


#include "stm32f4xx_hal.h"
#include "APP_ledDriver.h"
#include "main.h"

void tsk_ledDriver(void const * argumen){

	while(1){
		HAL_GPIO_TogglePin(LED_2_GPIO_Port,LED_2_Pin);
		osDelay(1000);
	}

}
