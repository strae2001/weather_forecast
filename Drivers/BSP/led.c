#include "led.h"

// 初始化GPIO函数
void led_init(void)
{
	// 打开时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// 初始化GPIO
	GPIO_InitTypeDef gpio_initStruct;
	gpio_initStruct.Pin = LED1| LED2;
	gpio_initStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_initStruct.Pull = GPIO_PULLUP;
	gpio_initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_initStruct);
	
	// 默认关闭led
	HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, LED2, GPIO_PIN_SET);
}

// 点亮led1函数
void led_on(uint16_t led)
{
	HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_RESET);
}

// 熄灭led
void led_off(uint16_t led)
{
	HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_SET);
}

void led_toggle(uint16_t led)
{
	if(!HAL_GPIO_ReadPin(GPIOB, led))
	{
		led_off(led);
	}
	else
		led_on(led);
}
