#include <REGX52.H>

void Delay1ms(unsigned int xms)		//@12.000MHz
{
	unsigned char i, j;

	while(xms)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
		xms--;
	}
	
}


void main()
{
	while(1)
	{
		if(P3_1 == 0)
		{
			Delay1ms(20);
			while(P3_1 == 0);
			Delay1ms(20);   //过滤抖动
			
			P2_0 = ~P2_0;      //状态取反，按位取反
		}
	}
}
