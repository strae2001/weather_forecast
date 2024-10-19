#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "uart1.h"
#include "cJSON.h"
#include "esp8266.h"
#include "OLED.h"
#include "weather.h"

// uint8_t Rx2_Buf[RX2BUF_SIZE] = {0};			// 串口2接收缓冲区

void GUI_ShowInfo(void);

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
	delay_init(72);						// 延时初始化 依据时钟频率为72mhz
	
    led_init();
	key_init();
	UART1_Init();
	OLED_Init();
	
	GUI_ShowInfo();			// 显示天气界面

	// esp8266入网并访问心知天气服务器
	Esp8266_Init();
	Esp8266_asClient(TCP_SER_IP, TCP_SER_PORT);
	
	get_threeDays_weather();		// 获取天气数据
	
	uint8_t i = 0;
	show_weather(i);				// oled显示第i天天气

    while(1)
    { 
		if(!key_check(KEY2))		// 按下key2键 切换日期
		{
			if(i++ >= 2)
				i = 0;
			show_weather(i);				
		}
		
    }
}

void GUI_ShowInfo(void)
{
	if (LOCATION == "fuzhou")
	{
		OLED_ShowChineseWord(2, 2, WORD_FU);
		OLED_ShowChineseWord(2, 4, WORD_ZHOU);
	}
	else if(LOCATION == "jinan")
	{
		OLED_ShowChineseWord(2, 2, WORD_JI);
		OLED_ShowChineseWord(2, 4, WORD_NAN);
	}
	else if(LOCATION == "qingdao")
	{
		OLED_ShowChineseWord(2, 2, WORD_QING);
		OLED_ShowChineseWord(2, 4, WORD_DAO);
	}
	else if(LOCATION == "zhuhai")
	{
		OLED_ShowChineseWord(2, 2, WORD_ZHU);
		OLED_ShowChineseWord(2, 4, WORD_HAI);
	}

	// OLED_ShowChineseWord(2, 10, WORD_SUNNY);

	OLED_ShowChineseWord(3, 2, WORD_ZUI);
	OLED_ShowChineseWord(3, 4, WORD_HIGH);
	OLED_ShowChineseWord(3, 6, WORD_QI);
	OLED_ShowChineseWord(3, 8, WORD_WEN);
	// OLED_ShowNum(3, 12, 32, 2);
	OLED_ShowChineseWord(3, 14, WORD_DU);

	OLED_ShowChineseWord(4, 2, WORD_ZUI);
	OLED_ShowChineseWord(4, 4, WORD_LOW);
	OLED_ShowChineseWord(4, 6, WORD_QI);
	OLED_ShowChineseWord(4, 8, WORD_WEN);
	// OLED_ShowNum(4, 12, 25, 2);
	OLED_ShowChineseWord(4, 14, WORD_DU);
}
