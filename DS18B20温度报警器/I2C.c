#include <REGX52.H>

sbit I2C_SCL = P2^1;  //时钟线
sbit I2C_SDA = P2^0;  //数据线

/**
  * @brief    I2C开始
  * @param    无
  * @retval   无
  */
void I2C_Start(void)
{
	//I2C时序结构：
	// 【【起止条件：SCL高电平期间，SDA从高电平(1)切换到低电平(0)】】
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2C_SDA = 0;
	I2C_SCL = 0;
	/*
		起始信号的产生过程是在 SCL 为高电平时，将 SDA 从高电平拉低。
		在开始产生起始信号之前，确保 SDA 和 SCL 处于一个已知的、稳定的初始
	状态。将 I2C_SDA 置为 1 是为了明确起始操作是从高电平向低电平的转换，这
	样可以使整个起始信号的产生过程更加清晰和可预测。
	*/
}

/**
  * @brief   I2C停止 
  * @param   无
  * @retval  无
  */
void I2C_Stop(void)
{
	// 【【终止条件：SCL高电平期间，SDA从低电平(0)切换到高电平(1)】】
	I2C_SDA = 0;    //SDA从低电平(0)
	I2C_SCL = 1;    //SCL高电平
	I2C_SDA = 1;    //SDA切换到高电平(1)
}

/*
    发送一个字节：SCL低电平(0)期间，主机(单片机)将数据位依次放到SDA线上
（高位在前）,然后拉高SCL(1)，从机(AT24C02)将在SCL高电平期间读取数据位,所
以SCL高电平(1)期间 SDA不允许有数据变化，依次循环上述过程8次，即可发送一个
字节。
*/
/**
  * @brief   I2C发送一个字节 
  * @param   Byte：要发送的字节
  * @retval  无
  */
void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2C_SDA = Byte&(0x80>>i);   //将Byte的最高位赋给I2C_SDA
		I2C_SCL = 1;
		I2C_SCL = 0;
	}
}

/*
    接收一个字节：SCL低电平期间，从机将数据位依次放到SDA线上（高位在前），
然后拉高SCL，主机将在SCL高电平期间读取数据位,所以SCL高电平期间SDA不允许有
数据变化，依次循环上述过程8次，即可接收一个字节（主机在接收之前，需要释放
SDA)
*/
/**
  * @brief   I2C接收一个字节 
  * @param   无
  * @retval  接收到的一个字节数据
  */
unsigned char I2C_ReceiveByte(void)
{
	unsigned char i,Byte = 0x00;
	
	I2C_SDA = 1;   //最好是高电平读取（符合协议）
	
	for(i=0;i<8;i++)
	{
		I2C_SCL = 1;    //每次将 I2C_SCL 置为高电平，此时从设备会在 SDA 线上放置一位数据
		if(I2C_SDA == 1){Byte|=(0x80>>i);}//将Byte的从高到低（第i位）置一}
		//【说明此时读取到的是【1】】【若没有数据则不读取】
		//if(I2C_SDA = 0){什么也不做;}
		I2C_SCL = 0;    //接着将 I2C_SCL 置为低电平，为下一次接收数据位做准备。
	}
	return Byte;
}

/*
	发送应答：在接收完一个字节之后，主机在下一个时钟发送一位数据，
数据0表示应答，数据1表示非应答
*/
/**
  * @brief   I2C发送应答  
  * @param   AckBit：应答位，【0为应答】【1为非应答】
  * @retval  无 
  */
void I2C_SendAck(unsigned char AckBit)
{
	I2C_SDA = AckBit;
	I2C_SCL = 1;
	I2C_SCL = 0;
}

/*
	接收应答：在发送完一个字节之后，主机在下一个时钟接收一位数据，
判断从机是否应答，【数据0表示应答】，【数据1表示非应答】（主机在接收之前，
需要释放SDA)
*/
/**
  * @brief   I2C接收应答位 
  * @param   无
  * @retval  接收到的应答位，【0为应答】【1为非应答】
  */
unsigned char I2C_ReceiveAck(void)
{
	unsigned char AckBit;
	I2C_SDA = 1;  //释放总线，准备接收应答信号。这一步是因为主设备在接收数据
				  //后，应该释放 SDA 线，让从设备可以控制 SDA 线来发送应答信号。
	I2C_SCL = 1;    //拉高时钟线 SCL。在 I²C 通信中，当 SCL 为高电平时，
					//数据在 SDA 线上是稳定的
	AckBit = I2C_SDA;  //读取 SDA 线上的电平状态并保存到 AckBit 变量中
//【如果从设备正确地接收到数据并发送了应答信号，那么在这个时候 SDA 线应该为低电平】
	I2C_SCL = 0;   //拉低时钟线，为下一次通信操作做准备
	return AckBit;  //接收到的应答信号的电平状态
}
