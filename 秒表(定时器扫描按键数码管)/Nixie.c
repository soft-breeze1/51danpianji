#include <REGX52.H>
#include "Delay.h"

/*
	这是一个数组，通常被称为数码管段码表。其中每个元素对应 0-9 十个数字的段码值，
用于控制数码管显示不同的数字。例如，0x3F 通常对应数字 0 的段码。
	【0x00】代表什么也不显示
	【0x40】代表显示一个【-】
*/
unsigned char NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x40};

//显示缓存区，使用缓存区时是从第一位开始，所以第一位为0，而【1~8】位要显示的
//数字初始时为NixieTable[]的第10位，即【什么也不显示】
unsigned char Nixie_Buf[9] = {0,10,10,10,10,10,10,10,10};

/**
  * @brief    传给缓存区数据
  * @param    Location：数据位置
  * @param    Number：数据
  * @retval   无
  */
void Nixie_SetBuf(unsigned char Location,Number)
{
	Nixie_Buf[Location] = Number;
}
	
/**
  * @brief    实现了一个数码管（共阴极数码管）的驱动函数。
  * @brief	  它可以根据传入的参数在【特定的数码管位置】显示【相应的数字】。
  * @param    Location：【传入要显示数字的数码管位置】。Number：【传入要显示的数字】
  * @retval   无
  */
void Nixie_Scan(unsigned char Location,Number)
{
	P0 = 0x00;    //清零
	switch(Location)    //位选
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
	//段选
	P0 = NixieTable[Number];//将对应数字的段码输出到 P0 端口，以控制数码管显示特定的数字
}

void Nixie_Loop(void)     //每隔2毫秒检测一下数码管，
						  //依次在不同的数码管位置显示缓存区中的数据
{
	static unsigned char i=1;  
	Nixie_Scan(i,Nixie_Buf[i]);   //在第i位显示缓存区的第i位数据
	i++;
	if(i>=9){i = 1;}
}
