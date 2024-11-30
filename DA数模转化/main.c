#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Nixie.h"
#include "Timer0.h"

sbit DA = P2^1;     // DAC(PWM)模块 【P21引脚】

unsigned char Counter,Compare;
unsigned char i;

void main()
{
	Timer0Init();
    while(1)
    {
        for(i=0;i<100;i++)     // 【Compare】阈值先变大
		{
			Compare=i;
			Delay(10);
		}
		for(i=100;i>0;i--)     // 【Compare】阈值再变小
		{
			Compare=i;
			Delay(10);
		}
    }
}

void Timer0_Routine() interrupt 1    //中断函数  【100us】
{
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	Counter++;
	Counter %= 100;     // 0~100变化
	if(Counter < Compare)    //Counter 小于 阈值
	{
		DA = 1;
	}
	else
	{
		DA = 0;
	}
}