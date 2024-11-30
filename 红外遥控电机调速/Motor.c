#include <REGX52.H>
#include "Timer1.h"

sbit Motor = P1^0;

// Counter：用于对定时器中断发生的次数进行计数。   
// Compare： 阈值
unsigned char Counter,Compare;

/**
  * @brief    电机初始化
  * @param    无
  * @retval   无
  */
void Motor_Init(void)
{
	Timer1_Init();    // 定时器1初始化
}

/**
  * @brief    设置速度阈值  
  * @param    无
  * @retval   无
  */
void Motor_SetSpeed(unsigned char Speed)
{
	Compare = Speed;
}

void Timer1_Routine() interrupt 3    //中断函数  【100us】
{
	TL1 = 0xA4;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	Counter++;
	Counter %= 100;
	if(Counter < Compare)    //Counter 小于 阈值
	{
		Motor = 1;
	}
	else
	{
		Motor = 0;
	}
}