#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "XPT2046.h"

unsigned int ADValue;      // 【转换的AD值】

// 上电时拔掉【DS18B20数字温度传感器】，否则数据可能以32为单位跳转

void main()
{
	LCD_Init();
	LCD_ShowString(1,1,"ADJ  NTC  RG");   //【电位器  热敏电阻  光敏电阻】
    while(1)
    {
        ADValue = XPT2046_ReadAD(XPT2046_XP_8);     // 读电位器
		LCD_ShowNum(2,1,ADValue,3);
		ADValue = XPT2046_ReadAD(XPT2046_YP_8);     // 读热敏电阻
		LCD_ShowNum(2,6,ADValue,3);
		ADValue = XPT2046_ReadAD(XPT2046_VBAT_8);   // 读光敏电阻
		LCD_ShowNum(2,11,ADValue,3);
		Delay(10);
    }
}