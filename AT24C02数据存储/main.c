#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Key.h"
#include "AT24C02.h"

/*
ST89C52单片机与 AT24C02(常见的 EEPROM（电可擦除可编程只读存储器）芯片)
通常通过 I²C 总线连接。连接方式如下：
将 ST89C52单片机的两个I/O引脚分别连接到AT24C02的SCL（时钟线和SDA（数据线）引脚。
通常需要在SCL 和 SDA 线上连接上拉电阻（一般为 4.7kΩ 左右）到电源 VCC。

AT24C02 是一个 2K 位（256 字节）的串行电可擦除可编程只读存储器（EEPROM）。
所以从机 AT24C02 可以存储 256 个字节的数据
*/

unsigned char KeyNum;
unsigned int Num;   //Num占两个字节

void main()
{
	LCD_Init();
	LCD_ShowNum(1,1,Num,5);
//	AT24C02_WriteByte(0,147);
//	Delay(5);
//	AT24C02_WriteByte(1,17);
//	Delay(5);
//	AT24C02_WriteByte(2,14);
//	Delay(5);     //在对 AT24C02 进行写操作后，需要等待5ms的写周期才能读出来
//	Data = AT24C02_ReadByte(1);
//	LCD_ShowNum(2,1,Data,3);
    while(1)
    {
        KeyNum = Key();
		if(KeyNum == 1)
		{
			Num++;
			LCD_ShowNum(1,1,Num,5);
		}
		if(KeyNum == 2)
		{
			Num--;
			LCD_ShowNum(1,1,Num,5);
		}
		if(KeyNum == 3)
		{
			AT24C02_WriteByte(0,Num%256);//Num的低8位存放在AT24C02第零个字节
			Delay(5);
			AT24C02_WriteByte(1,Num/256);//Num的高8位存放在AT24C02第一个字节
			Delay(5);
			LCD_ShowString(2,1,"Write OK");
			Delay(1000);
			LCD_ShowString(2,1,"        ");
		}
		if(KeyNum == 4)
		{
			Num = AT24C02_ReadByte(0); //Num的低8位数据，此时Num的高八位全为0
			Num |= AT24C02_ReadByte(1)<<8;
			// AT24C02第一个字节左移8位后为16位数据，可以让原本的数据(相或)赋给Num
			// Num的高8位(全为0)与AT24C02第一个字节左移8位相或(16位)，
			// 即【将Num的高8位写入到Num中】
			LCD_ShowNum(1,1,Num,5);
			LCD_ShowString(2,1,"Read OK");
			Delay(1000);
			LCD_ShowString(2,1,"        ");
		}
    }
}