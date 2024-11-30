#include <REGX52.H>
#include  "Timer0.h"
#include  "Key.h"
#include  "Nixie.h"
#include  "Delay.h"
#include  "AT24C02.h"

/*
重要思路：
	    将子模块中的子函数打包的一个【循环检测】函数中，再将这个【循环检测】
	在主函数中的【中断函数】中不断的经过自己【设定的时间】来不断地调用这个
	【循环检测】函数，这样就不用在子模块中使用【中断函数】就可以设置时间中断
	了。
*/

unsigned char KeyNum;

unsigned char Min,Sec,MinSec;

unsigned char RunFlag;    //控制启动暂停，默认为 0

void main()
{
	Timer0Init();
    while(1)
    {
        KeyNum = Key();
		if(KeyNum == 1)           //时间【启动】/【暂停】
		{
			RunFlag=!RunFlag;     //变换【启动】/【暂停】状态
		}
		if(KeyNum == 2)           //时间全部清零
		{
			Min = 0;
			Sec = 0;
			MinSec = 0;
		}
		if(KeyNum == 3)           //存储当前的时间
		{
			AT24C02_WriteByte(0,Min);   //0地址存储分钟
			Delay(5);                   //写周期
			AT24C02_WriteByte(1,Sec);   //1地址存储秒
			Delay(5);                   //写周期
			AT24C02_WriteByte(2,MinSec);//2地址存储毫秒
			Delay(5);
		}
		if(KeyNum == 4)           //复现存储的时间
		{
			Min = AT24C02_ReadByte(0);
			Sec = AT24C02_ReadByte(1);
			MinSec = AT24C02_ReadByte(2);
		}
		Nixie_SetBuf(1,Min/10);     //将Min/10传给了Nixie_SetBuf的第1位
		Nixie_SetBuf(2,Min%10);
		Nixie_SetBuf(3,11);         //显示一个【-】
		Nixie_SetBuf(4,Sec/10);
		Nixie_SetBuf(5,Sec%10);
		Nixie_SetBuf(6,11);
		Nixie_SetBuf(7,MinSec/10);
		Nixie_SetBuf(8,MinSec%10);
    }
}

void Sec_Loop(void)         //10ms【MinSec】要加一
{
	if(RunFlag)     //如果RunFlag == 1，则为【启动】状态
	{
		MinSec++;
		if(MinSec >= 100)
		{
			MinSec = 0;
			Sec++;
			if(Sec >= 60)
			{
				Sec = 0;
				Min++;
				if(Min >= 60)
				{
					Min = 0;
				}
			}
		}
	}
	
}

void Timer0_Routine() interrupt 1    //中断函数
{
	static unsigned int T0Count1,T0Count2,T0Count3;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值     //1ms进一次中断，一次中断后，重新计数
	T0Count1++;
	if(T0Count1 >= 20)  //每隔20毫秒检测一下【Key_GetState】是否为【0】
	{
		T0Count1 = 0;
		Key_Loop();     //实现了【定时器扫描按键】
	}
	T0Count2++;
	if(T0Count2 >= 2)   //每隔2毫秒检测一下数码管
	{
		T0Count2 = 0;
		Nixie_Loop();
	}
	T0Count3++;
	if(T0Count3 >= 10)  //每隔10毫秒检测一下，10ms【MinSec】要加一
	{
		T0Count3 = 0;
		Sec_Loop();
	}
}