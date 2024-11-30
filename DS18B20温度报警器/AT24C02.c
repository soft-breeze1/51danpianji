#include <REGX52.H>
#include "I2C.h"

// AT24CO2的固定地址为1010，可配置地址本开发板上为000
// 所以SLAVE ADDRESS+W为【0XA0】，SLAVE ADDRESS+R为0xA1
#define AT24C02_ADDRESS		0xA0

/**
  * @brief    AT24C02写入一个字节
  * @param    WordAddress：要写入字节的地址
  * @param    Data：要写入的数据
  * @retval   无
  */
void AT24C02_WriteByte(unsigned char WordAddress,Data)
{
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);   //发送 AT24C02 的设备地址
	I2C_ReceiveAck();  //等待接收从设备（AT24C02）的应答信号，确认设备地址被正确接收
	I2C_SendByte(WordAddress);   //发送要写入数据的存储地址
	I2C_ReceiveAck();            //等待接收应答信号，确认存储地址被正确接收
	I2C_SendByte(Data);          //发送要写入的数据
	I2C_ReceiveAck();            //等待接收应答信号，确认数据被正确接收
	I2C_Stop();                  //产生 I²C 停止信号，结束本次通信
}

/**
  * @brief     AT24C02读取一个字节
  * @param     WordAddress：要读出字节的地址
  * @retval    Data:要读出的数据
  */
unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();               //【前面几步与 向AT24C02从机中写入数据一样】
	
	I2C_Start(); //再次产生起始信号，因为在 I²C 读操作中，需要在发送完存储地址后重新启动通信
	I2C_SendByte(AT24C02_ADDRESS|0x01);   
	//发送 AT24C02 的设备地址并将最低位设置为 1，表示进行读操作。
	//【读操作的设备地址最低位为1，写操作的设备地址最低位为0】
	I2C_ReceiveAck();   //等待接收应答信号，确认读操作请求已被接收
	Data = I2C_ReceiveByte();// 调用接收字节的函数 I2C_ReceiveByte()，从 I²C 总线接收一个
						     // 字节的数据，并将其存储到 Data 变量中
	I2C_SendAck(1);          //发送非应答信号（这里参数为 1 表示发送非应答），表示读取完成
	I2C_Stop();              //产生 I²C 停止信号，结束本次通信
	
	return Data;             //返回从 AT24C02 读取到的字节数据
}