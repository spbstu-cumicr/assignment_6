#include "timer.h"
#include "led.h"

#include <stdio.h>

void TIM3_IRQHandler(void)
{
    static volatile Led *led = NULL;
    led = getLed();
    
    static volatile uint32_t ledSize = 0;
    ledSize = getLedSize();
    
    for (uint8_t ledNumber = 0; ledNumber < ledSize; ledNumber++)
    {
        Led *currLed = (Led *)&led[ledNumber];
        
        if (currLed->isOn(currLed) == true)
        {
            currLed->turnOff(currLed);
        }
        else
        {
            currLed->turnOn(currLed);
        }
    }
    
    GPIO_ToggleBits(GPIOA, GPIO_Pin_2);
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void initTimer(const uint32_t frequency_Hz)
{
	// Вычисляем предделитель и период таймера в тактах
	uint16_t prescaler = SystemCoreClock / 2 / REFERENCE_FREQUENCY_HZ - 1;
	uint32_t period = REFERENCE_FREQUENCY_HZ / frequency_Hz - 1;
	
	// Подаём питание на порт таймера общего назначения TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// Настраиваем таймер общего назначения TIM3
	TIM_TimeBaseInitTypeDef ledTimer;
	TIM_TimeBaseStructInit(&ledTimer);
	
	ledTimer.TIM_Prescaler = prescaler;
	ledTimer.TIM_CounterMode = TIM_CounterMode_Up;
	ledTimer.TIM_Period = period;
	
	TIM_TimeBaseInit(TIM3, &ledTimer);		
	
	// Включаем таймер общего назначения TIM3
	TIM_Cmd(TIM3, ENABLE);
    
    // Включаем прерывания при переполнении счётчика
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	// Разрешаем прерывания для таймера общего назначения TIM3
	NVIC_EnableIRQ(TIM3_IRQn);
}

void initControlPin(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
    // Устанавливаем пин светодиода в режим вывода c двухтактным каскадом с низкой частотой
    GPIO_InitTypeDef controlPin;
    GPIO_StructInit(&controlPin);

    controlPin.GPIO_Pin   = GPIO_Pin_2;
    controlPin.GPIO_Mode  = GPIO_Mode_OUT;
    controlPin.GPIO_Speed = GPIO_Low_Speed;
    controlPin.GPIO_OType = GPIO_OType_PP;
    
    GPIO_Init(GPIOA, &controlPin);
}
