#include <REGX52.H>
#include "OneWire.h"
#include "LCD1602.h"

#define DS18B20_SKIP_ROM		0xCC
#define DS18B20_CONVERT		    0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

/**
  * @brief    启动连接在单总线上的 DS18B20 温度传感器进行温度转换操作   
  * @param    无
  * @retval   无
  */
void DS18B20_ConvertT(void)
{
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);   // 【SKIP ROM】 【跳过指令】
	OneWire_SendByte(DS18B20_CONVERT);
	// 对于 DS18B20 来说，当它接收到这个特定的转换命令后，就会启动内部
	// 的温度测量转换过程，将当前环境温度转换为对应的数字量，以便后续可
	// 以通过读取操作获取到温度值。
}

/**
  * @brief   从 DS18B20 温度传感器读取温度数据，并将其转换为以
			 摄氏度为单位的【浮点数形式】返回。 
  * @param   无
  * @retval  T：【读取的温度数据】
  */
float DS18B20_ReadT(void)
{
	unsigned char TLSB,TMSB;   //Byte 0 为温度的低字节，Byte 1 为温度的高字节
	int Temp;
	float T;
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
	TLSB = OneWire_ReceiveByte();    // 先拿低字节
	TMSB = OneWire_ReceiveByte();    // 再拿高字节
	Temp = (TMSB<<8)|TLSB;
	T = Temp/16.0;                    //【Temp】十六位中【低四位】为小数部分
	return T;
}