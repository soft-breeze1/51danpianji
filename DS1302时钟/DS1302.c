#include <REGX52.H>
#include "Delay.h"

sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

#define DS1302_SECOND		0X80
#define DS1302_MINUTE		0X82
#define DS1302_HOUR			0X84
#define DS1302_DATE			0X86
#define DS1302_MONTH		0X88
#define DS1302_DAY			0X8A
#define DS1302_YEAR			0X8C
#define DS1302_WP			0X8E

unsigned char DS1302_Time[] = {24,11,5,12,59,55,2};

/**
  * @brief    DS1302初始化（两个引脚电平置为0）
  * @param    无
  * @retval   无
  */
void DS1302_Init(void)
{
	DS1302_CE = 0;           //刚开始上电时CE和SCLK默认都是1，要先置为0
	DS1302_SCLK = 0;
}

/**
  * @brief     向DS1302中写入1个数据
  * @param     Command：写入命令
  * @param     Data：要写入的数据
  * @retval    无
  */
void DS1302_WriteByte(unsigned char Command,Data)  //16个上升下降沿
{
	unsigned char i;
	DS1302_CE = 1;
	
	for(i=0;i<8;i++)
	{
		DS1302_IO = Command&(0x01<<i);    //取Command的第(0x01<<i)位
		DS1302_SCLK = 1;      //如果置一和置零的间隔小于使用手册上的时间，就要加上延时函数
		DS1302_SCLK = 0;
	}
	for(i=0;i<8;i++)
	{
		DS1302_IO = Data&(0x01<<i);    //取Command的第(0x01<<i)位
		DS1302_SCLK = 1;      //如果置一和置零的间隔小于使用手册上的时间，就要加上延时函数
		DS1302_SCLK = 0;
	}
	DS1302_CE = 0;    //最后将CE置为0，方便下次写入数据
}

/**
  * @brief   将DS1302中的一个数据读出   
  * @param   Command：读出命令
  * @retval  Data：读出的数据
  */
unsigned char DS1302_ReadByte(unsigned char Command) //15个上升下降沿
{
	unsigned char i,Data = 0x00;   //全局变量不用初始化（默认地址位0x00），定义为局部变量需要初始化地址
	Command |= 0x01;        //【【先将Command或1，也就是加1，为读命令】】
	//【这样就可以用一个宏来操作两个函数】
	DS1302_CE = 1;
	for(i=0;i<8;i++)
	{
		DS1302_IO = Command&(0x01<<i);    //取Command的第(0x01<<i)位
		DS1302_SCLK = 0;   //先将SCLK置零，再将其置一，是因为防止最后一次将SCLK置零时
		DS1302_SCLK = 1;   //将后续的数据读出来，故改变SCLK的顺序
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCLK = 1;      //在前面SCLK已经置一的情况下再次置一没有影响
		DS1302_SCLK = 0;      //将第一位读出到IO输出线上
		if(DS1302_IO)   //如果DS1302_IO = 1，即读出数据
			//如果输出为1就记录，如果输出为0就不变，i+1继续循环
		    //【【因为Data开始时就全为0，只有是1时才需要同步】】
		{
			Data = Data|(0x01<<i);   
			//如果读取到数据位为 1，则将Data的第i位置为 1，即将读取到的数据位存储到Data变量中。
		}
	}
	DS1302_CE = 0;
	DS1302_IO = 0;     //将DS1302_IO置为0，方便下次读数据或者写入数据
	return Data;
}

/**
  * @brief    将写入DS1302时钟模块数据这一步骤模块化，直接调用可以连续写入多个数据
  * @param    无
  * @retval   无
  */
void DS1302_SetTime(void)    //【模块化后就只用调取DS1302_SetTime和DS1302_ReadTime】
{
	DS1302_WriteByte(DS1302_WP,0x00); //关闭芯片写保护
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);  //转为BCD码
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x00); //关闭芯片写保护
	
}

/**
  * @brief    将读出DS1302时钟模块这一步骤模块化，可以连续读出多个数据
  * @param    无
  * @retval   无 
  */
void DS1302_ReadTime(void)
{
	unsigned char Temp;   //暂时存储一个从DS1320时钟模块读出的数据
	Temp = DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0] = Temp/16*10 + Temp%16;    //转为十进制
	Temp = DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1] = Temp/16*10 + Temp%16;
	Temp = DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2] = Temp/16*10 + Temp%16;
	Temp = DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3] = Temp/16*10 + Temp%16;
	Temp = DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4] = Temp/16*10 + Temp%16;
	Temp = DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5] = Temp/16*10 + Temp%16;
	Temp = DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6] = Temp/16*10 + Temp%16;
	
}