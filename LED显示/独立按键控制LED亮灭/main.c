#include <REGX52.H>

void main()
{
//	P2 = 0xFE;
	P2_0 = 0;        //代表P2引脚的D1亮
	P2_0 = 1;        //代表P2引脚的D1灭
	while(1)
	{
		if(P3_1 == 0 && P3_0 == 0)     //按下按键1 和 按键2 LED灯才亮
		{
			P2_0 = 0;
		}
		else
		{
			P2_0 = 1;
		}
	}
}
