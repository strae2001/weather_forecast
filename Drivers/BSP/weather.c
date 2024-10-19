#include "weather.h"
#include "cJSON.h"
#include "esp8266.h"
#include "delay.h"
#include "OLED.h"
#include <stdlib.h>

char weather_data[RX2BUF_SIZE] = {0};

typedef struct 
{
    char date[20];
    char code_day[5];       // 天气代码
    char high[5];
    char low[5];
}daily_weather_t;

daily_weather_t weather_threeDays[3] = {0};

void get_threeDays_weather(void)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        memset(weather_data, 0, sizeof(weather_data));
        get_weatherData_fromServer(i);
        if(get_weatherData_fromServer(i) == RECV_OK)
        {
            copy_Rx2Buf_data(weather_data);

            //1. 将JSON字符串转换成JSON结构体
            cJSON* jsonStruct = cJSON_Parse((const char*)weather_data);
            
            //2. 解析JSON各个字段
            cJSON* resArr = cJSON_GetObjectItem(jsonStruct, "results");       //2.1 获取根对象里的results字段的值（数组1）
            cJSON* ArrObj = cJSON_GetArrayItem(resArr, 0);                   //2.2 获取resArr数组中的唯一元素（对象1）
            cJSON* dailyArr = cJSON_GetObjectItem(ArrObj, "daily");           //2.3 获取对象1中的daily字段的值（数组2）
            cJSON* res_data = cJSON_GetArrayItem(dailyArr, 0);                 //2.4 获取数组2中的第一个元素（对象2）
            
            cJSON* date = cJSON_GetObjectItem(res_data, "date");            //2.5 获取对象2中的date字段的值
            const char* date_str = date->valuestring;
			strcpy(weather_threeDays[i].date, date_str);
            printf("date:%s\r\n", weather_threeDays[i].date);

            cJSON* code_day = cJSON_GetObjectItem(res_data, "code_day");
            const char* code_day_str = code_day->valuestring;
			strcpy(weather_threeDays[i].code_day, code_day_str);
            printf("code_day:%s\r\n", weather_threeDays[i].code_day);

            cJSON* high = cJSON_GetObjectItem(res_data, "high");
            const char* high_str = high->valuestring;
			strcpy(weather_threeDays[i].high, high_str);
            printf("high:%s\r\n", weather_threeDays[i].high);

            cJSON* low = cJSON_GetObjectItem(res_data, "low");
            const char* low_str = low->valuestring;
			strcpy(weather_threeDays[i].low, low_str);
            printf("low:%s\r\n", weather_threeDays[i].low);

            printf("\r\n");

            //3. 清除JSON结构体
            cJSON_Delete(jsonStruct);

            delay_ms(100);
        }       
    }
}

void show_weather(uint8_t day)
{
    // 显示日期时间
    OLED_ShowString(1, 2, weather_threeDays[day].date);

    // 显示天气状况
    uint8_t weather_status = atoi(weather_threeDays[day].code_day);
    switch (weather_status)
    {
        case 0:case 1:case 2:case 3:
			OLED_ShowString(2, 10, "      ");          // 清空旧天气状况
            OLED_ShowChineseWord(2, 10, WORD_SUNNY);
            break;

        case 4:case 5:case 6:case 7:case 8:
			OLED_ShowString(2, 10, "      ");          // 清空旧天气状况
            OLED_ShowChineseWord(2, 10, WORD_MULTI);
            OLED_ShowChineseWord(2, 12, WORD_CLOUD);
            break;

        case 9:
			OLED_ShowString(2, 10, "      ");          // 清空旧天气状况
            OLED_ShowChineseWord(2, 10, WORD_SHADE);
            break;

        case 10:case 11:case 12:case 13:case 14:
        case 15:case 16:case 17:case 18:
			OLED_ShowString(2, 10, "      ");          // 清空旧天气状况
            OLED_ShowChineseWord(2, 10, WORD_RAINY);
            break;
    }

    // 显示气温
    OLED_ShowString(3, 12, weather_threeDays[day].high);    // 最高气温
    OLED_ShowString(4, 12, weather_threeDays[day].low);     // 最低气温
}
