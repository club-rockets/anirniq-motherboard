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
#include "string.h"

#define INA3221_CR_ADDR 0x00

#define INA3221_BUS_VOLTAGE_BASE_ADDR 0x02
#define INA3221_SHUNT_VOLTAGE_BASE_ADDR 0x01

#define SHUNT_VALUE 100.0f


#define INA3221_DEVICE_ADR 0x41

union{
	struct __attribute__ ((__packed__)) {
		unsigned mode : 3;
		unsigned iCnvTime : 3;
		unsigned vCnvTime : 3;
		unsigned avgMode : 3;
		unsigned ch3EN : 1;
		unsigned ch2EN : 1;
		unsigned ch1EN : 1;
		unsigned rst : 1;

	}reg;
	uint8_t UINT8[2];
	uint16_t UINT16;
} INA3221CR = {0};

union INA3221BV_u{
	struct __attribute__ ((__packed__)) {
		unsigned : 3;
		signed adcVal: 13;
	}reg;
	uint8_t UINT8[2];
	uint16_t UINT16;
};

union INA3221BV_u bvBuff[3] = {0};
union INA3221BV_u svBuff[3] = {0};



//assume a 16bit register
void regByteFlip(void* reg){
	uint8_t* r = reg;
	uint8_t tmp;
	tmp = r[0];
	r[0] = r[1];
	r[1] = tmp;
}

void INA3221_Init(){

	memset(&(INA3221CR),0,sizeof(INA3221CR));
	uint8_t regAddr = INA3221_CR_ADDR;
	INA3221CR.reg.rst = 0;
	INA3221CR.reg.ch1EN = 1;
	INA3221CR.reg.ch2EN = 1;
	INA3221CR.reg.ch3EN = 1;
	INA3221CR.reg.vCnvTime = 7;
	INA3221CR.reg.iCnvTime = 7;
	//shunt and bus voltage continuous
	INA3221CR.reg.mode = 7;
	regByteFlip(&INA3221CR);

	HAL_I2C_Master_Transmit(&hi2c2,INA3221_DEVICE_ADR<<1,&regAddr,1,100);
	HAL_I2C_Master_Transmit(&hi2c2,INA3221_DEVICE_ADR<<1,INA3221CR.UINT8,sizeof(INA3221CR),100);
}

void INA3221_ReadBusVoltage(uint8_t channel){
	uint8_t regAddr = INA3221_BUS_VOLTAGE_BASE_ADDR + 2*channel;

	HAL_I2C_Master_Transmit(&hi2c2,INA3221_DEVICE_ADR<<1,&regAddr,1,100);
	HAL_I2C_Master_Receive(&hi2c2,INA3221_DEVICE_ADR<<1,bvBuff[channel].UINT8,sizeof(bvBuff[channel]),100);
}

void INA3221_ReadShuntVoltage(uint8_t channel){
	uint8_t regAddr = INA3221_SHUNT_VOLTAGE_BASE_ADDR+ 2*channel;

	HAL_I2C_Master_Transmit(&hi2c2,INA3221_DEVICE_ADR<<1,&regAddr,1,100);
	HAL_I2C_Master_Receive(&hi2c2,INA3221_DEVICE_ADR<<1,svBuff[channel].UINT8,sizeof(svBuff[channel]),100);
}

float INA3221_getBusVoltage(uint8_t channel){
	union INA3221BV_u raw;
	memcpy(&raw,&(bvBuff[channel]),sizeof(raw));
	regByteFlip(&raw);
	float voltage = (float)(raw.reg.adcVal) * 32.76f / 0xFFF;
	return voltage;
}

float INA3221_getShuntCurrent(uint8_t channel){
	union INA3221BV_u raw;
	memcpy(&raw,&(svBuff[channel]),sizeof(raw));
	regByteFlip(&raw);
	//in mv
	float voltage = (float)(raw.reg.adcVal) * 163.8f / 0xFFF;
	voltage /= SHUNT_VALUE;
	//in momh momh
	return voltage;
}
