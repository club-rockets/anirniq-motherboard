/*
 * BSP_ledDriver.h
 *
 *  Created on: Apr 20, 2019
 *      Author: Marc-Andre
 */

#ifndef BSP_BSP_LEDDRIVER_H_
#define BSP_BSP_LEDDRIVER_H_

#include "BSP_ledDriver.h"
#include "main.h"

#define TLC5971_MAX_CURRENT 0X7F

#define RED_MAX_CURRENT 0X20
#define GREEN_MAX_CURRENT 0X20
#define BLUE_MAX_CURRENT 0X20

typedef struct __attribute__ ((__packed__)) {
	unsigned R : 16;
	unsigned G : 16;
	unsigned B : 16;
} RGBVal_t;

typedef struct HsvColor {
    uint16_t h;
	uint16_t s;
	uint16_t v;
} HSVVal_t;

uint32_t ledDriver_init(uint8_t currentR,uint8_t currentG,uint8_t currentB);

uint32_t ledDriver_setLED(uint8_t index,RGBVal_t color);

uint32_t ledDriver_clearAll();

uint32_t ledDriver_sendAll();

RGBVal_t HsvToRgb(HSVVal_t hsv);

#endif /* BSP_BSP_LEDDRIVER_H_ */
