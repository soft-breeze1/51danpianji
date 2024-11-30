#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"

float T;    //全局变量

void main()
{
	DS18B20_ConvertT();
	Delay(1000);            //提前发生转换指令，然后延迟 1ms ,防止读到不正确的值【85】
	LCD_Init();
	LCD_ShowString(1,1,"Temperature:");
    while(1)
    {
        DS18B20_ConvertT(); 
		T = DS18B20_ReadT();
		if(T<0)
		{
			LCD_ShowChar(2,1,'-');
			T = -T;
		}
		else
		{
			LCD_ShowChar(2,1,'+');
		}
		LCD_ShowNum(2,2,T,3);    //温度T会转化为无符号整数，整数部分要3位
		LCD_ShowChar(2,5,'.');
		LCD_ShowNum(2,6,(unsigned long)(T*10000)%10000,4);
		/*
		T*10000 将【低四位】的小数点部分 弄到 整数部分，再通过(unsigned long)
		将其强转为(unsigned long)，因为 T*10000 后 超出了【int】型范围。
		对其取余可以取【低四位】。
		*/
    }
}