#pragma once

#include "mcu_support_package/inc/stm32f4xx.h"
#include "stdbool.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

enum LED_COLOUR
{
    LED_GREEN  = 0,
    LED_YELLOW = 1,
    LED_RED    = 2,
    LED_BLUE   = 3
};

typedef struct Led
{
/*
private:
*/
	uint32_t periphPort;
	GPIO_TypeDef *gpioPort;
	uint16_t gpioPin;
	uint16_t gpioPinSrc;
	bool state;
/*
public:
*/	
	void (*turnOn)(struct Led *led);
	void (*turnOff)(struct Led *led);
	bool (*isOn)(struct Led *led);
} Led;

Led *initLed(void);
Led *getLed(void);
uint32_t getLedSize(void);

void turnOnLed(Led *led);
void turnOffLed(Led *led);
bool isLedOn(Led *led);
