#include <REGX52.H>
#include "Timer0.h"
#include "Key.h"
#include <INTRINS.H>

unsigned char KeyNum,LEDMode;

//void Timer0_Init()   //1毫秒@11.0592MHz
//{
//	//设置定时器模式
///*
//  	TMOD = TMOD&0xF0;       //把TMOD的低四位清零，高四位保持不变
//	1010 0011 & 1111 0000   ->   1010 0000
//	可以写为
//*/
//	TMOD &= 0xF0;
///*
//	
//	TMOD = TMOD|0x01;       //把TMOD的最低位置一，高四位保持不变
//	1010 0000 | 0000 0001   ->   1010 0001
//	可以写为
//*/
//	TMOD |= 0x01;
///*
//	TH0 = 64535/256;  //高8位
//	TL0 = 64535%256;  //低8位
//*/
//	TL0 = 0x18;		//设置定时初值
//	TH0 = 0xFC;		//设置定时初值
//	TF0 = 0;
//	TR0 = 1;
//	ET0 = 1;
//	EA = 1;
//	PT0 = 0;
//}

void main()
{
	P2 = 0xFE;   //1111 1110 点亮最低一位LED 即D0
	Timer0Init();
	while(1)
	{
		KeyNum = Key();
		if(KeyNum)
		{
			if(KeyNum == 1)     //按下独立按键1时，切换模式
			{
				LEDMode++;      //LEDMode要么为1，要么为0，两种模式
				if(LEDMode >= 2)LEDMode=0;
			}
		}
	}
}


void Timer0_Routine() interrupt 1    //中断函数
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值     //1ms进一次中断，一次中断后，重新计数
	T0Count++;
	if(T0Count >= 500)
	{
		T0Count = 0;
		if(LEDMode == 0)  //左移模式
		{
			P2 = _crol_(P2,1);   //左移一位
		}
		if(LEDMode == 1)  //右移模式
		{
			P2 = _cror_(P2,1);   //右移一位
		}
	}
}
