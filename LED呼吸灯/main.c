#include <REGX52.H>

sbit LED = P2^0;

void Delay(unsigned int t)
{
	while(t--);
}

void main()
{
	unsigned char Time,i;
    while(1)
    {
		for(Time = 0;Time <= 100;Time++)      //由暗变亮
		{
			for(i = 0;i < 20;i++)   //每一个亮度循环20次，延缓亮度变化速度
			{
				LED = 0;
				Delay(Time);
				LED = 1;
				Delay(100-Time);
			}
		}
		for(Time = 100;Time > 0;Time--)       //由亮变暗
		{
			for(i = 0;i < 20;i++)   //每一个亮度循环20次，延缓亮度变化速度
			{
				LED = 0;
				Delay(Time);
				LED = 1;
				Delay(100-Time);
			}
		}		
    }
}