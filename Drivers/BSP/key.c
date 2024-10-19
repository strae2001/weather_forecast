#include "key.h"
#include "delay.h"

void key_init(void)
{
	// ��ʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	// ��ʼ��GPIO
	GPIO_InitTypeDef gpio_initStruct;
	gpio_initStruct.Pin = KEY1| KEY2;
	gpio_initStruct.Mode = GPIO_MODE_INPUT;
	gpio_initStruct.Pull = GPIO_PULLUP;
	gpio_initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_initStruct);
}

/*
	����ֵ0:		key������		
	����ֵ1:		key����
*/
uint8_t key_check(uint16_t key)
{
	if(!HAL_GPIO_ReadPin(GPIOA, key))
	{
		delay_ms(20);		// ����
		while(!HAL_GPIO_ReadPin(GPIOA, key));
		delay_ms(20);		// ����
		return 0;
	}
	return 1;
}
