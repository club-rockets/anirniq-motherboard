

#ifndef APP_LED_DRIVER_H_
#define APP_LED_DRIVER_H_

#include "cmsis_os.h"
#include "BSP_ledDriver.h"

void tsk_ledDriver(void const * argument);

void ledDriverButtonPressed();

#define MOTHERBOARD_LED_INDEX	1
#define ACQUISITION_LED_INDEX	2
#define COMMUNICATION_LED_INDEX	3
#define MISSION_LED_INDEX		4

osThreadId tsk_ledDriverID;


#endif //APP_LED_DRIVER_H_
