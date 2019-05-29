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


RGBVal_t HsvToRgb(HSVVal_t hsv)
{
    RGBVal_t rgb;
    uint16_t region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.R = hsv.v;
        rgb.G = hsv.v;
        rgb.B = hsv.v;
        return rgb;
    }

    region = hsv.h / 0x2AAA;
    remainder = (hsv.h - (region * 0x2AAA)) * 6; 

    p = (hsv.v * (0xFFFF - hsv.s)) >> 8;
    q = (hsv.v * (0xFFFF - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (0xFFFF - ((hsv.s * (0xFFFF - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.R = hsv.v; 
			rgb.G = t;
			rgb.B = p;
            break;
        case 1:
            rgb.R = q;
			rgb.G = hsv.v;
			rgb.B = p;
            break;
        case 2:
            rgb.R = p; 
			rgb.G = hsv.v; 
			rgb.B = t;
            break;
        case 3:
            rgb.R = p;
			rgb.G = q;
			rgb.B = hsv.v;
            break;
        case 4:
            rgb.R = t;
			rgb.G = p;
			rgb.B = hsv.v;
            break;
        default:
            rgb.R = hsv.v;
			rgb.G = p;
			rgb.B = q;
            break;
    }

    return rgb;
}
