#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"

sbit Buzzer=P2^5;    //定义蜂鸣器IO口(位声明)

//蜂鸣器端口：
/**
  * @brief   蜂鸣器私有延时函数，延时500微妙 
  * @param   无
  * @retval  无
  */
void Buzzer_Delay500us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 247;
	while (--i);
}


/**
  * @brief   用于控制蜂鸣器发声特定的时长
  * @param   ms:【要让蜂鸣器发出响声的时长】
  * @retval  无
  */
void Buzzer_Time(unsigned int ms)//输入【1000】即【1秒响一千次】即【1000HZ】
{
	unsigned int i;
	for(i=0;i<ms*2;i++)
	{
		Buzzer = !Buzzer;    //逻辑取反
		Buzzer_Delay500us(); //每两次反转为一次【响，不响】【2000次反转为1000次响】
	}
}
