#include <REGX52.H>

unsigned char NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

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


void Nixie(unsigned char Location,Number)
{
	switch(Location)
	{
		case 1:P2_4 = 1;P2_3 = 1;P2_2 = 1;break;   //Y7
		case 2:P2_4 = 1;P2_3 = 1;P2_2 = 0;break;   //Y6
		case 3:P2_4 = 1;P2_3 = 0;P2_2 = 1;break;   //Y5
		case 4:P2_4 = 1;P2_3 = 0;P2_2 = 0;break;   //Y4
		case 5:P2_4 = 0;P2_3 = 1;P2_2 = 1;break;   //Y3
		case 6:P2_4 = 0;P2_3 = 1;P2_2 = 0;break;   //Y2
		case 7:P2_4 = 0;P2_3 = 0;P2_2 = 1;break;   //Y1
		case 8:P2_4 = 0;P2_3 = 0;P2_2 = 0;break;   //Y0
	}
	P0 = NixieTable[Number];
	Delay1ms(1);
	P0 = 0x00;   //«Â¡„
}

void main()
{
	while(1)
	{
		Nixie(1,1);
//		Delay1ms(200);
		Nixie(2,2);
//		Delay1ms(200);
		Nixie(3,3);
//		Delay1ms(200);
	}
}
