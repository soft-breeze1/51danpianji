#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

sbit Buzzer=P2^5;

#define SPEED	350     //【定义宏后可以快速修改音乐速度】

#define P    0
#define L1   1
#define L1_  2
#define L2   3
#define L2_  4
#define L3   5
#define L4   6
#define L4_  7
#define L5   8
#define L5_  9
#define L6   10
#define L6_  11
#define L7   12
#define M1   13
#define M1_  14
#define M2   15
#define M2_  16
#define M3   17
#define M4   18
#define M4_  19
#define M5   20
#define M5_  21
#define M6   22
#define M6_  23
#define M7   24
#define H1   25
#define H1_  26
#define H2   27
#define H2_  28
#define H3   29
#define H4   30
#define H4_  31
#define H5   32
#define H5_  33
#define H6   34
#define H6_  35
#define H7   36

unsigned int FreqTable[]={
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
	0,
};    //频率


//《天空之城》
//【main.c(317): error C249: 'DATA': SEGMENT TOO LARGE】
// STC89C52的 RAM 大小为 【512 字节】，ROM(Flash)大小为 【8k】
// 加一个 code 将其存储到 【Flash】中
//【Flash中的内容不可以更改】
unsigned char code Music[] = {
	//1
	P,	4,
	P,	4,
	P,	4,
	M6,	2,
	M7,	2,
	
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	M7,	4+4+4,
	M3,	2,
	M3,	2,
	
	//2
	M6,	4+2,
	M5,	2,
	M6, 4,
	H1,	4,
	
	M5,	4+4+4,
	M3,	4,
	
	M4,	4+2,
	M3,	2,
	M4,	4,
	H1,	4,
	
	//3
	M3,	4+4,
	P,	2,
	H1,	2,
	H1,	2,
	H1,	2,
	
	M7,	4+2,
	M4_,2,
	M4_,4,
	M7,	4,
	
	M7,	8,
	P,	4,
	M6,	2,
	M7,	2,
	
	//4
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	M7,	4+4+4,
	M3,	2,
	M3,	2,
	
	M6,	4+2,
	M5,	2,
	M6, 4,
	H1,	4,
	
	//5
	M5,	4+4+4,
	M2,	2,
	M3,	2,
	
	M4,	4,
	H1,	2,
	M7,	2+2,
	H1,	2+4,
	
	H2,	2,
	H2,	2,
	H3,	2,
	H1,	2+4+4,
	
	//6
	H1,	2,
	M7,	2,
	M6,	2,
	M6,	2,
	M7,	4,
	M5_,4,
	
	
	M6,	4+4+4,
	H1,	2,
	H2,	2,
	
	H3,	4+2,
	H2,	2,
	H3,	4,
	H5,	4,
	
	//7
	H2,	4+4+4,
	M5,	2,
	M5,	2,
	
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	H3,	4+4+4+4,
	
	//8
	M6,	2,
	M7,	2,
	H1,	4,
	M7,	4,
	H2,	2,
	H2,	2,
	
	H1,	4+2,
	M5,	2+4+4,
	
	H4,	4,
	H3,	4,
	H3,	4,
	H1,	4,
	
	//9
	H3,	4+4+4,
	H3,	4,
	
	H6,	4+4,
	H5,	4,
	H5,	4,
	
	H3,	2,
	H2,	2,
	H1,	4+4,
	P,	2,
	H1,	2,
	
	//10
	H2,	4,
	H1,	2,
	H2,	2,
	H2,	4,
	H5,	4,
	
	H3,	4+4+4,
	H3,	4,
	
	H6,	4+4,
	H5,	4+4,
	
	//11
	H3,	2,
	H2,	2,
	H1,	4+4,
	P,	2,
	H1,	2,
	
	H2,	4,
	H1,	2,
	H2,	2+4,
	M7,	4,
	
	M6,	4+4+4,
	P,	4,
	
	0xFF
};

//《小星星》
//第一种方法：【一维数组】
//unsigned char Music[] = {
//	12,4,
//	12,4,
//	19,4,
//	19,4,
//	21,4,
//	21,4,
//	19,4+4,
//	0,4,
//	17,4,
//	17,4,
//	16,4,
//	16,4,
//	14,4,
//	14,4,
//	12,4+4,
//	
//	0xFF
//};

//第二种方法：【二维数组】
//unsigned char Music[][2] = {
//	{12,4},
//	{12,4},
//	{19,4},
//	{19,4},
//	{21,4},
//	{21,4},
//	{19,4+4},
//	{17,4},
//	{17,4},
//	{16,4},
//	{16,4},
//	{14,4},
//	{14,4},
//	{12,4+4},

//	{0xFF}
//};

unsigned char FreqSelect,MusicSelect;  //【频率选择】  【音乐选择】

void main()
{
	Timer0Init();
    while(1)
    {
		//第一种方法：【一维数组】
		if(Music[MusicSelect] != 0xFF)
		{
			FreqSelect = Music[MusicSelect];   //先取到第一个数值，代表频率，通过音乐选择递增来选择第几个频率
			MusicSelect++;                     //加1
			Delay(SPEED/4*Music[MusicSelect]);      //加1后取到的为这个频率持续的时间
			MusicSelect++;                     //再加1，可以取到下一个频率
			TR0=0;     //每演奏一个就把定时器关掉
			Delay(5);  //延迟5ms
			TR0=1;     //延迟完后再将其打开
		}
		else
		{
			TR0=0;      //把定时器关掉
			while(1);
		}
		
		
		//第二种方法：【二维数组】
//		if(Music[MusicSelect][0] != 0xFF)
//		{
//			FreqSelect = Music[MusicSelect][0];
//			Delay(SPEED/4*Music[MusicSelect][1]);
//			MusicSelect++;
//			TR0=0;     //每演奏一个就把定时器关掉
//			Delay(5);  //延迟5ms
//			TR0=1;     //延迟完后再将其打开
//		}
//		else
//		{
//			while(1);
//		}
    }
}

void Timer0_Routine() interrupt 1    //中断函数
{
	if(FreqTable[FreqSelect] != 0)    
	//如果频率为0，即【休止符】，则不执行代码，即【不播放音乐】
	//如果频率不为0，则播放音乐
	{
		TL0 = FreqTable[FreqSelect]%256;
		TH0 = FreqTable[FreqSelect]/256;
		Buzzer = !Buzzer;
	}
}