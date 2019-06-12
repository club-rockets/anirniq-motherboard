/*
 * BSP_INA3221.h
 *
 *  Created on: Jun 11, 2019
 *      Author: Marc-Andre Denis
 */

#ifndef BSP_BSP_INA3221_H_
#define BSP_BSP_INA3221_H_


void INA3221_Init();

void INA3221_ReadBusVoltage(uint8_t channel);

void INA3221_ReadShuntVoltage(uint8_t channel);

float INA3221_getBusVoltage(uint8_t channel);

float INA3221_getShuntCurrent(uint8_t channel);

#endif /* BSP_BSP_INA3221_H_ */
