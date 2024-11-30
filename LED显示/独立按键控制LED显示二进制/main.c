#include <REGX52.H>

void Delay1ms(unsigned int xms)		//@12.000MHz
{
	unsigned char i, j;
	
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

void main()
{
	//unsigned char LEDNum = 0;
	while(1)
	{
		if(P3_1 == 0)
		{
			Delay1ms(20);
			while(P3_1 == 0);     //按键按下后空循环，只有松手后才会退出空循环
			Delay1ms(20);
			
			//LEDNum++;
			//刚开始  1111 1111
			//P2 = ~LEDNum;
			P2--;
		}
	}
}