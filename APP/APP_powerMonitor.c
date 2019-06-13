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
#include "id.h"
#include "enums.h"

#define MONITOR_BAT_CHANNEL	1
#define MONITOR_5V_CHANNEL	0
#define MONITOR_3V3_CHANNEL	2

void tsk_powerMonitor(void const * argument){

	can_regData_u canReg = {0};
	INA3221_Init();
	while(1){
		//read vbat voltage
		osDelay(100);
		INA3221_ReadBusVoltage(MONITOR_BAT_CHANNEL);
		canReg.FLOAT = INA3221_getBusVoltage(MONITOR_BAT_CHANNEL);
		can_canSetRegisterData(CAN_MOTHERBOARD_VBATT_VOLTAGE,&canReg);
		//read bat current
		osDelay(100);
		INA3221_ReadShuntVoltage(MONITOR_BAT_CHANNEL);
		canReg.FLOAT = INA3221_getShuntCurrent(MONITOR_BAT_CHANNEL);
		can_canSetRegisterData(CAN_MOTHERBOARD_VBATT_AMPERAGE,&canReg);
		//read 5V voltage
		osDelay(100);
		INA3221_ReadBusVoltage(MONITOR_5V_CHANNEL);
		canReg.FLOAT = INA3221_getBusVoltage(MONITOR_5V_CHANNEL);
		can_canSetRegisterData(CAN_MOTHERBOARD_5V_VOLTAGE,&canReg);
		//read 5V current
		osDelay(100);
		INA3221_ReadShuntVoltage(MONITOR_5V_CHANNEL);
		canReg.FLOAT = INA3221_getShuntCurrent(MONITOR_5V_CHANNEL);
		can_canSetRegisterData(CAN_MOTHERBOARD_5V_AMPERAGE,&canReg);
		//read 3v3 voltage
		osDelay(100);
		INA3221_ReadBusVoltage(MONITOR_3V3_CHANNEL);
		canReg.FLOAT = INA3221_getBusVoltage(MONITOR_3V3_CHANNEL);
		can_canSetRegisterData(CAN_MOTHERBOARD_3V3_VOLTAGE,&canReg);
		//read 3v3 current
		osDelay(100);
		INA3221_ReadShuntVoltage(MONITOR_3V3_CHANNEL);
		canReg.FLOAT = INA3221_getShuntCurrent(MONITOR_3V3_CHANNEL);
		can_canSetRegisterData(CAN_MOTHERBOARD_3V3_AMPERAGE,&canReg);
		osDelay(5000);
	}
}
