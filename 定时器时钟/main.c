#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Timer0.h"

unsigned char Sec=55,Min=59,Hour=23;

void main()
{
	LCD_Init();
	Timer0Init();
	
	LCD_ShowString(1,1,"Clock:");
	LCD_ShowString(2,1,"  :  :");
	while(1)
	{
		LCD_ShowNum(2,1,Hour,2);
		LCD_ShowNum(2,4,Min,2);
		LCD_ShowNum(2,7,Sec,2);
	}
}

void Timer0_Routine() interrupt 1    //中断函数
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值     //1ms进一次中断，一次中断后，重新计数
	T0Count++;
	if(T0Count >= 1000)
	{
		T0Count = 0;
		Sec++;
		if(Sec >= 60)
		{
			Sec = 0;
			Min++;
			if(Min >= 60)
			{
				Min = 0;
				Hour++;
				if(Hour >= 24)
				{
					Hour = 0;
				}
			}
		}
	}
}
