#include <REGX52.H>

void Timer0Init(void)
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0;		//设置定时初值
	TH0 = 0;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0【不】开始计时
}

/**
  * @brief    设置开始计时的时间
  * @param    Value：开始计时时间
  * @retval   无
  */
void Timer_SetCounter(unsigned int Value)
{
	/*
		单片机的定时器模块中，采用的是 16 位的计数方式，
		也就是用两个 8 位的寄存器来共同表示一个 16 位的计数值。
		对于 51 单片机的定时器 0 来说，这两个寄存器就是 TH0（定时
		器 0 的高 8 位寄存器）和 TL0（定时器 0 的低 8 位寄存器）。
	
		将一个完整的 16 位计数值【Value】正确地分配到了定时器 0 的
		高 8 位寄存器 TH0 和低 8 位寄存器 TL0 中，从而使得 定时器 能
		够按照设定的计数值进行准确的定时操作。
	*/
	TH0 = Value/256;
	TL0 = Value%256;      //将Value的16位数据
}

/**
  * @brief    获取定时器的当前计数值
  * @param    无
  * @retval   (TH0<<8)|TL0 ：获取到定时器的当前计数值
  */
unsigned int Timer_GetCounter(void)
{
	return (TH0<<8)|TL0;
}

/**
  * @brief    给1时开始计时，给0时不计时  
  * @param    Flag：计时标志位
  * @retval   无
  */
void TIMER_Run(unsigned char Flag)
{
	TR0 = Flag;
}
