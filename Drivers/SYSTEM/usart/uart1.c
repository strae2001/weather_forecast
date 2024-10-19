#include "uart1.h"
#include "esp8266.h"

UART_HandleTypeDef huart1;

void UART1_Init(void)
{
	huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.Mode = UART_MODE_TX_RX;        // �շ�ģʽ
    huart1.Init.Parity = UART_PARITY_NONE;     // ����ҪУ��λ
    huart1.Init.StopBits = UART_STOPBITS_1;    // 1λֹͣλ
    huart1.Init.WordLength = UART_WORDLENGTH_8B;       // ÿ�����ݴ���8λ
    HAL_UART_Init(&huart1);

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    // ʹ��UART1 ��ʹ��UART_IT_RXNE���������ж�
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		// ��ʼ��GPIO
		GPIO_InitTypeDef gpio_initStruct;
		gpio_initStruct.Pin = TX2;			
		gpio_initStruct.Mode = GPIO_MODE_AF_PP;
		gpio_initStruct.Pull = GPIO_PULLUP;
		gpio_initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpio_initStruct);
		gpio_initStruct.Pin = RX2;			
		gpio_initStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOA, &gpio_initStruct);
		
		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
		HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(huart->Instance == USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		// ��ʼ��GPIO
		GPIO_InitTypeDef gpio_initStruct;
		gpio_initStruct.Pin = TX1;			
		gpio_initStruct.Mode = GPIO_MODE_AF_PP;
		gpio_initStruct.Pull = GPIO_PULLUP;
		gpio_initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpio_initStruct);
		gpio_initStruct.Pin = RX1;			
		gpio_initStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOA, &gpio_initStruct);
		
		//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
		HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}


/*	 �������ݷ�װ	*/
void serial1_sendByte(uint8_t Byte)
{
	HAL_UART_Transmit(&huart1, &Byte, 1, 10);
}

void UART1_sendString(char* str)
{
	for(uint8_t i=0; str[i]!='\0'; i++)
	{
		serial1_sendByte(str[i]);
	}
}

int fputc(int ch, FILE* stream)
{
	serial1_sendByte(ch);
	return ch;
}
