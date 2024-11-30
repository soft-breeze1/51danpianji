#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"

/*
BCD码转十进制：DEC = BCD/16*10 + BCD%16; (2位BCD) 只能转2位的BCD
十进制转BCD码：BCD = DEC/10*16 + DEC%10; (2位BCD)

因为DS1302时钟模块需要输入输出的是BCD码，所以在【DS1302_SetTime()】时需要
将输入的十进制数字转换为BCD码，而输出时需要将BCD码转换为人们更想要看到和理
解的十进制。
*/

void main()
{
	LCD_Init();
	DS1302_Init();
	
	LCD_ShowString(1,1,"  -  -  ");
	LCD_ShowString(2,1,"  :  :  ");
	
	DS1302_SetTime();
	
    while(1)
    {
		DS1302_ReadTime();
		LCD_ShowNum(1,1,DS1302_Time[0],2);
		LCD_ShowNum(1,4,DS1302_Time[1],2);
		LCD_ShowNum(1,7,DS1302_Time[2],2);
		LCD_ShowNum(2,1,DS1302_Time[3],2);
		LCD_ShowNum(2,4,DS1302_Time[4],2);
		LCD_ShowNum(2,7,DS1302_Time[5],2);
    }
}