#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Buzzer.h"

sbit Motor = P1^0;

unsigned char Counter,Compare;
unsigned char KeyNum,Speed;

void main()
{
	Timer0Init();
	Compare = 5;
    while(1)
    {
        KeyNum = Key(); 
		if(KeyNum == 1)
		{
			Speed++;
			Speed%=4;     //0,1,2,3
			if(Speed == 0){Compare = 0;}     //设置阈值 
			if(Speed == 1){Compare = 40;}
			if(Speed == 2){Compare = 70;}
			if(Speed == 3){Compare = 100;}   //Counter < 100
		}
		Nixie(1,Speed);
    }
}

void Timer0_Routine() interrupt 1    //中断函数  【100us】
{
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	Counter++;
	Counter %= 100;
	if(Counter < Compare)    //Counter 小于 阈值
	{
		Motor = 1;
	}
	else
	{
		Motor = 0;
	}
}