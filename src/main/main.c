#include "mcu_support_package/inc/stm32f4xx.h"
#include "led.h"
#include "timer.h"
#include "button.h"
#include "main/Lab6_Test.h"

#include <stdint.h>

// Фамилия
const char *lastName = "Pankov";

int main(void)
{
	// Вариант задания
	static uint8_t variant = 0;	
	variant = Lab6_Test_ini((char *)lastName);

    static uint32_t frequency_Hz = 0;
    frequency_Hz = (variant + 1) * 200;

	// Контрольная сумма
	static uint32_t checksum = 0;
	static uint16_t decision[] = {0, 0};
	
	// Инициализируем светодиоды
	static Led *led = NULL;
	led = initLed();
	
	// Инициализируем таймер
	initTimer(frequency_Hz);
    
    // Инициализируем контрольный вывод
    initControlPin();

	while(1)
	{
        // Считаем контрольную сумму
		checksum = while_Test(decision);
    }
	
	return 0;
}

#ifdef USE_FULL_ASSERT

// эта функция вызывается, если assert_param обнаружил ошибку
void assert_failed(uint8_t *file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	(void)file;
	(void)line;
	
	__disable_irq();
	while(1)
	{
		// это ассемблерная инструкция "отладчик, стой тут"
		// если вы попали сюда, значит вы ошиблись в параметрах вызова функции из SPL. 
		// Смотрите в call stack, чтобы найти ее
		__BKPT(0xAB);
	}
}

#endif
