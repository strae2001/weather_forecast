#ifndef __OLED_H
#define __OLED_H

#include "sys.h"

enum ChinaWord{
    WORD_YEAR = 0,
    WORD_MONTH,
    WORD_MDAY,
    WORD_FU,
    WORD_ZHOU,
    WORD_JI,
    WORD_NAN,
    WORD_QING,
    WORD_DAO,
    WORD_ZHU,
    WORD_HAI,
    WORD_SUNNY,
    WORD_MULTI,
    WORD_CLOUD,
    WORD_RAINY,
    WORD_ZUI,
    WORD_HIGH,
    WORD_LOW,
    WORD_QI,
    WORD_WEN,
    WORD_DU,
    WORD_SHADE,
};

enum CITY_CODE{
    FU_ZHOU = 0,
    JI_NAN,
    QING_DAO,
    ZHU_HAI,
};

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// ChinaWord: 要显示的汉字
void OLED_ShowChineseWord(uint8_t Line, uint8_t Column, uint8_t ChinaWord);

#endif
