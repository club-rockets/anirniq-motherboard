/*
 * BSP_ledDriver.c
 *
 *  Created on: Apr 20, 2019
 *      Author: Marc-Andre
 */

#include "BSP_ledDriver.h"
#include "spi.h"
#include "main.h"
#include "string.h"

extern SPI_HandleTypeDef hspi1;

typedef struct __attribute__ ((__packed__)) {

	RGBVal_t LED[4];

	unsigned BCR  : 7;
	unsigned BCG  : 7;
	unsigned BCB  : 7;

	unsigned BLANK : 1;
	unsigned DSPRPT: 1;
	unsigned TMGRST: 1;
	unsigned EXTGCK: 1;
	unsigned OUTTMG: 1;
	unsigned WCOMMAND: 6;
} tlc5971Reg_t;

static tlc5971Reg_t led_drv[2] = {0};

uint8_t buff1[sizeof(led_drv)];

//the spi peripheral must be initialised
uint32_t ledDriver_init(uint8_t currentR,uint8_t currentG,uint8_t currentB){

	led_drv[0].WCOMMAND = 0x25;
	led_drv[0].DSPRPT = 1;
	led_drv[1].WCOMMAND = 0x25;
	led_drv[1].DSPRPT = 1;

	led_drv[0].BCR = currentR;
	led_drv[0].BCG = currentG;
	led_drv[0].BCB = currentB;
	led_drv[1].BCR = currentR;
	led_drv[1].BCG = currentG;
	led_drv[1].BCB = currentB;

	uint32_t i = 0;
	uint8_t*ptr = (uint8_t*)(led_drv);
	for(;i<sizeof(led_drv);i++){
		buff1[sizeof(led_drv)-i-1] = ptr[i];
	}

	HAL_SPI_Transmit_IT(&hspi1,buff1,sizeof(led_drv));
	return 1;
}

uint32_t ledDriver_setLED(uint8_t index,RGBVal_t color){
	led_drv[index/4].LED[index%4] = color;
	return 1;
}

uint32_t ledDriver_clearAll(){
	memset((void*)(led_drv[0].LED),0,4*6);
	memset((void*)(led_drv[1].LED),0,4*6);
	return 1;
}

uint32_t ledDriver_sendAll(){
	uint32_t i = 0;
	uint8_t*ptr = (uint8_t*)(led_drv);
	for(;i<sizeof(led_drv);i++){
		buff1[sizeof(led_drv)-i-1] = ptr[i];
	}

	HAL_SPI_Transmit_IT(&hspi1,buff1,sizeof(led_drv));
	return 1;
}


RGBVal_t HsvToRgb(HSVValf_t in)
{
    float hh, p, q, t, ff;
    long  i;
    float r, g, b;

    hh = in.h;
    if(hh >= 360.0f) {
		hh = 0.0f;
	}

    hh /= 60.0f;
    i = (long) hh;
    ff = hh - i;

    p = in.v * (1.0f - in.s);
    q = in.v * (1.0f - (in.s * ff));
    t = in.v * (1.0f - (in.s * (1.0 - ff)));

    switch(i) {
		case 0:
			r = in.v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = in.v;
			b = p;
			break;
		case 2:
			r = p;
			g = in.v;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = in.v;
			break;
		case 4:
			r = t;
			g = p;
			b = in.v;
			break;
		case 5:
		default:
			r = in.v;
			g = p;
			b = q;
			break;
    }

	RGBVal_t ret = {
		.R = r * 0xFFFF,
		.G = g * 0xFFFF,
		.B = b * 0xFFFF
	};
	return ret;
}