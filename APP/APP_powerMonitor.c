/*
 * APP_powerMonitor.c
 *
 *  Created on: 6 juin 2019
 *      Author: Marc-Andre Denis
 */

#include "APP_powerMonitor.h"
#include "cmsis_os.h"
#include "bsp_can.h"
#include "BSP_INA3221.h"


void tsk_powerMonitor(void const * argument){


	while(1){
		INA3221_Init();
		osDelay(100);
		INA3221_ReadBusVoltage(0);
		INA3221_ReadShuntVoltage(1);
		osDelay(100);
		INA3221_getBusVoltage(0);
		INA3221_getShuntCurrent(1);
		osDelay(100);
	}
}
