#include <REGX52.H>

// 在【@11.0592MHz】中的延时函数中【函数跳转】需要【5us】

sbit OneWire_DQ = P3^7;

/**
  * @brief    初始化：主机将总线拉低至少480us，然后释放总线，等待15~60us后，
  * @brief	  【存在的从机】会拉低总线60~240us以响应主机，之后从机将释放总线。
  * @param    无
  * @retval   返回应答位
  */
unsigned char OneWire_Init(void)
{
	unsigned char i;
	unsigned char AckBit;
	
	EA = 0;     // 不允许全局中断 ， 
				// 因为进入中断时间比较长,而下面这些操作都是 us 
				// 【要防止下面的代码运行时进入中断】

	OneWire_DQ = 1;
	OneWire_DQ = 0;
	i = 227;while (--i);       //Delay 500 us
	OneWire_DQ = 1;            //释放总线
	i = 29;while (--i);        //Delay 70 us
	AckBit = OneWire_DQ;       //应答位
	i = 227;while (--i);       //把时序走完，480-70=410，直接Delay 500 us
	
	EA = 1;     //【打开全局中断】
	
	return AckBit;
}

/**
  * @brief   【发送一位】：主机将总线拉低60~120us，然后释放总线，表示发送0;
			 主机将总线拉低1~15us，然后释放总线，表示发送1。从机将在总线
			 拉低30us后（典型值）读取电平，整个时间片应大于60us  
  * @param   Bit:主机发送给从机的一位数据
  * @retval  无
  */
void OneWire_SendBit(unsigned char Bit)
{
	unsigned char i;
	EA = 0;
	OneWire_DQ = 0;   //从机拉低总线后会自动释放总线，所以初始化完总线是1
	i = 4;while (--i);         // Delay 10us
	OneWire_DQ = Bit;     //【如果是高电平就是1，低电平为0】
	i = 22;while (--i);   //Delay 50us   结束       【10+50=61】
	OneWire_DQ = 1;
	EA = 1;
}

/**
  * @brief   【接收一位】：主机将总线拉低1~15us，然后释放总线，并在拉低后
			 15us内读取总线电平（尽量贴近15us的末尾），读取为低电平则
			 为接收0，读取为高电平则为接收1，整个时间片应大于60us。
  * @param   无
  * @retval  主机读取从机一位数据
  */
unsigned char OneWire_ReceiveBit(void)
{
	unsigned char i;
	unsigned char Bit;
	EA = 0;
	OneWire_DQ = 0;        //拉低总线，以防上次总线电平还没恢复为【低电平】
	i = 2;while (--i);     //Delay 5us
	
	OneWire_DQ = 1;
	i = 2;while (--i);     //Delay 5us
	
	Bit = OneWire_DQ;      //主机接收从机的一位数据
	i = 22;while (--i);    //Delay 50us
	EA = 1;
	return Bit;
}

/**
  * @brief    主机发送给从机的一个字节
  * @param    Byte：要发送的字节
  * @retval   无
  */
void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)        //主机发送给从机的一个字节
	{
		OneWire_SendBit(Byte&(0x01<<i));
	}
}

/**
  * @brief    主机接收从机的一个字节
  * @param    无
  * @retval   Byte：要接受的字节
  */
unsigned char OneWire_ReceiveByte(void)
{
	unsigned char i;
	unsigned char Byte = 0x00;
	for(i=0;i<8;i++)        //主机接收从机的一个字节
	{
		if(OneWire_ReceiveBit()){Byte|=(0x01<<i);}
		//判断一下，如果有字节需要接收，就可以依次取出每位的数据
		//OneWire_ReceiveBit()为1，【有数据】，Byte【|】一个【1】
		//OneWire_ReceiveBit()为0，【没有数据】，Byte【|】一个【0】
	}
	return Byte;
}
