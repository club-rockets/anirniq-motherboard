/*
 * APP_ledDriver.c
 *
 *  Created on: Apr 20, 2019
 *      Author: Marc-Andre
 */


#include "stm32f4xx_hal.h"
#include "APP_ledDriver.h"
#include "bsp_can.h"
#include "main.h"
#include "bsp_can_regdef.h"

#define LED_NEW_DATA 1

volatile enum{
	off = 0,
	status,
	party
} ledState = off;

void ledTest();
void stateChangeCallback(uint32_t board,uint32_t id);

const RGBVal_t moduleLedColors[] = {
		//green
		{
			.R = 0x0,
			.G = 0xffff,
			.B = 0x0
		},
		//blue
		{
			.R = 0x0,
			.G = 0x0,
			.B = 0xffff
		}
};

const uint32_t moduleLedColorsSize = sizeof(moduleLedColors)/sizeof(RGBVal_t);

void tsk_ledDriver(void const * argumen){
	ledDriver_init(RED_MAX_CURRENT,GREEN_MAX_CURRENT,BLUE_MAX_CURRENT);
	can_setRegisterCallback(MOTHERBOARD,CAN_MOTHERBOARD_STATUS_INDEX,stateChangeCallback);
	can_setRegisterCallback(COMMUNICATION,CAN_COMMUNICATION_STATUS_INDEX,stateChangeCallback);
	can_setRegisterCallback(MISSION,CAN_MISSION_STATUS_INDEX,stateChangeCallback);
	can_setRegisterCallback(ACQUISITION,CAN_ACQUISITION_STATUS_INDEX,stateChangeCallback);

	can_regData_u regData = {0};
	osEvent signalValue = {0};
	while(1){

		//ledTest();

		//poll for signal
		signalValue = osSignalWait(LED_NEW_DATA,osWaitForever);
		if(signalValue.value.v == 1);
		else continue;

		if(ledState == off){
			ledDriver_clearAll();
			ledDriver_sendAll();
			continue;
		}


		else if(ledState == status){
			//clear led data
					ledDriver_clearAll();

					//read all registers
					//motherboard

					can_getRegisterData(MOTHERBOARD,CAN_MOTHERBOARD_STATUS_INDEX,&regData);
					if(regData.UINT32_T < moduleLedColorsSize){
						ledDriver_setLED(MOTHERBOARD_LED_INDEX,moduleLedColors[regData.UINT32_T]);
					}

					can_getRegisterData(ACQUISITION,CAN_ACQUISITION_STATUS_INDEX,&regData);
					if(regData.UINT32_T < moduleLedColorsSize){
						ledDriver_setLED(ACQUISITION_LED_INDEX,moduleLedColors[regData.UINT32_T]);
					}
					can_getRegisterData(COMMUNICATION,CAN_COMMUNICATION_STATUS_INDEX,&regData);
					if(regData.UINT32_T < moduleLedColorsSize){
						ledDriver_setLED(COMMUNICATION_LED_INDEX,moduleLedColors[regData.UINT32_T]);
					}
					can_getRegisterData(MISSION,CAN_MISSION_STATUS_INDEX,&regData);
					if(regData.UINT32_T < moduleLedColorsSize){
						ledDriver_setLED(MISSION_LED_INDEX,moduleLedColors[regData.UINT32_T]);
					}
					//send data

					ledDriver_sendAll();
					osDelay(100);
		}

		else{
			while(ledState == party){
				ledTest();
			}
		}


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


void stateChangeCallback(uint32_t board,uint32_t id){
	osSignalSet(tsk_ledDriverID,LED_NEW_DATA);
}

void ledDriverButtonPressed(){
	static uint32_t counter = 0;
	if(HAL_GetTick() - counter > 10){
		counter = HAL_GetTick();
		osSignalSet(tsk_ledDriverID,LED_NEW_DATA);
		ledState++;
		ledState%=3;
	}

}
