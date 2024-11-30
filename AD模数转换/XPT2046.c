#include <REGX52.H>

sbit XPT2046_CS = P3^5;
sbit XPT2046_DCLK = P3^6;
sbit XPT2046_DIN = P3^4;
sbit XPT2046_DOUT = P3^7;

/**
  * @brief   通过 SPI 接口（模拟方式）从 XPT2046 芯片读取模拟数字
			 转换（ADC）的值。XPT2046 是一款常用的触摸屏控制器和 
			 ADC 芯片，这段函数实现了【向XPT2046 芯片】发送【读取
			 命令】并接收转换结果的操作。
  * @param   Command : 向 XPT2046 芯片发送特定的命令 
  * @retval  ADVAlue 被用于存储从 XPT2046 芯片读取到的模拟数字转换（ADC）
			 结果。具体来说，通过一系列的操作，包括向芯片发送读取命令以及
			 接收芯片返回的数据位，并将这些数据位组合成一个完整的 16 位
			 ADVAlue。
  */
unsigned int XPT2046_ReadAD(unsigned char Command)  //转换哪个通道
	// 三个通道【电位器】【热敏电阻】【光敏电阻】
{
	unsigned char i;
	unsigned int ADVAlue = 0;       // 初始化要转换的AD值
	XPT2046_DCLK = 0;				// 初始化DCLK
	XPT2046_CS = 0; 				// 拉低【片选线】
	
	for(i=0;i<8;i++)                // 将 8位 命令 发给 【XPT2046芯片】 
	{
		XPT2046_DIN = Command&(0x80>>i);    
		//将 【Command 的最高位】 放在 【XPT2046_DIN】 上
		XPT2046_DCLK = 1;           //将【最高位】 发出去，
		XPT2046_DCLK = 0;           //再将其拉低，为了下一次发送数据
	}
	
	// 中间会有【BUSY】时间，根据实际测试，不需要加【延时时间】
	
	/*
		通过一个循环从 XPT2046 芯片的 DOUT 引脚读取 16 位数据到 ADVAlue
		变量中，然后根据 Command 与 0x04 的按位与结果，对读取到的数据进
		行移位处理并返回不同的值。
	*/
	
	for(i=0;i<16;i++)               // 连续读 16 位
	{
		XPT2046_DCLK = 1;           //先给一个上升沿和一个下降沿，数据才会过来
		XPT2046_DCLK = 0;
		if(XPT2046_DOUT){ADVAlue|=(0x8000>>i);}
		// 当 【XPT2046_DOUT】 为【高电平】，就将右移后的 0x8000 与 
		// ADVAlue 进行【按位或】操作，从而将【对应位的数据 1】存入ADVAlue中
	}
	XPT2046_CS = 1;                 // 【CS拉高】
	
	if(Command&0x08)                // 取出【0x08】这一位，如果为1，则为8 位精度
	{	
		return ADVAlue>>8;          // 因为选择的是8位的精度，所以读出的
									// 数据中后8面【0填充】，右移8位读出
									// 准确数据
	}
	else                            // 如果为0，则为 【12 位精度】            
	{
		return ADVAlue>>4;          // 右移4位读出准确数据
	}
}