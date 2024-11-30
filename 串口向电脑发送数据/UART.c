#include <REGX52.H>

void UART_Init()
{
	SCON = 0x50;
	PCON &= 0x7F;		//波特率不倍速
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFA;		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

void UART_SendByte(unsigned char Byte)
{
	SBUF = Byte;                //串口数据缓存寄存器
	while(TI == 0);             //TI为0一直循环，TI为1则跳出循环
	TI = 0;                     //重新置TI为0
}