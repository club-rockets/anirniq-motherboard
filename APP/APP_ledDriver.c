/*
 * APP_ledDriver.c
 *
 *  Created on: Apr 20, 2019
 *      Author: Marc-Andre
 */


#include "stm32f4xx_hal.h"
#include "APP_ledDriver.h"
#include "BSP_ledDriver.h"
#include "main.h"

void ledTest();

void tsk_ledDriver(void const * argumen){
	ledDriver_init(RED_MAX_CURRENT,GREEN_MAX_CURRENT,BLUE_MAX_CURRENT);

	while(1){

		ledTest();

	}

}

void ledTest(){

	static RGBVal_t led = {0};
	static uint8_t i = 0;

		led.R = 0xffff;
		led.G = 0;
		led.B = 0;
		for(i = 0;i<8;i++){
			ledDriver_setLED(i,led);
			ledDriver_sendAll();
			ledDriver_clearAll();
			osDelay(100);
		}
		led.R = 0;
		led.G = 0xffff;
		led.B = 0;
		for(i = 0;i<8;i++){
			ledDriver_setLED(i,led);
			ledDriver_sendAll();
			ledDriver_clearAll();
			osDelay(100);
		}
		led.R = 0;
		led.G = 0;
		led.B = 0xffff;;
		for(i = 0;i<8;i++){
			ledDriver_setLED(i,led);
			ledDriver_sendAll();
			ledDriver_clearAll();
			osDelay(100);
		}


}
