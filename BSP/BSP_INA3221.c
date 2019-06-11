/*
 * BSP_INA3221.c
 *
 *  Created on: Jun 11, 2019
 *      Author: Marc-Andre
 */


#include "i2c.h"
#include "main.h"
#include "BSP_INA3221.h"
#include "cmsis_os.h"

union{
	struct __attribute__ ((__packed__)) {
		unsigned rst : 1;
		unsigned ch1EN : 1;
		unsigned ch2EN : 1;
		unsigned ch3EN : 1;
		unsigned avgMode : 3;
		unsigned vCnvTime : 3;
		unsigned iCnvTime : 3;
		unsigned mode : 3;
	}reg;
	uint8_t UINT8[2];
	uint16_t UINT16;
} INA3221CR;


void BSP_INA3221Init(){

	uint8_t txdata[] = {0x00};
	HAL_I2C_Master_Transmit(&hi2c2,0b1000001,txdata,1,0);
	osDelay(100);
	HAL_I2C_Master_Receive(&hi2c2,0b1000001,INA3221CR.UINT8,sizeof(INA3221CR),0);


}
