#include <REGX52.H>
#include "Delay.h"

/*
	    这是一个数组，通常被称为数码管段码表。其中每个元素对应 0-9 十个数字的段码值，
	用于控制数码管显示不同的数字。例如，0x3F 通常对应数字 0 的段码。
*/
unsigned char NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

/**
  * @brief    实现了一个数码管（共阴极数码管）的驱动函数。
  * @brief	  它可以根据传入的参数在【特定的数码管位置】显示【相应的数字】。
  * @param    Location：【传入要显示数字的数码管位置】。Number：【传入要显示的数字】
  * @retval   无
  */
void Nixie(unsigned char Location,Number)
{
	switch(Location)
	{
		case 1:P2_4 = 1;P2_3 = 1;P2_2 = 1;break;   //Y7  第一个位置
		case 2:P2_4 = 1;P2_3 = 1;P2_2 = 0;break;   //Y6  第二个位置 
		case 3:P2_4 = 1;P2_3 = 0;P2_2 = 1;break;   //Y5  第三个位置
		case 4:P2_4 = 1;P2_3 = 0;P2_2 = 0;break;   //Y4  第四个位置
		case 5:P2_4 = 0;P2_3 = 1;P2_2 = 1;break;   //Y3  第五个位置
		case 6:P2_4 = 0;P2_3 = 1;P2_2 = 0;break;   //Y2  第六个位置
		case 7:P2_4 = 0;P2_3 = 0;P2_2 = 1;break;   //Y1  第七个位置
		case 8:P2_4 = 0;P2_3 = 0;P2_2 = 0;break;   //Y0  第八个位置
	}
	P0 = NixieTable[Number];//将对应数字的段码输出到 P0 端口，以控制数码管显示特定的数字
	Delay(1);
	P0 = 0x00; //在显示完数字后，将 P0 端口清零，以避免对其他数码管产生影响
}
