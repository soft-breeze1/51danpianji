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
	ES = 1;         /*
					使能串口中断。这里的 “ES” 是串口中断允许位，
					设置为 1 表示允许串口中断。
					*/
	EA = 1;         /*
					总中断允许位，设置为 1 表示允许所有中断。
					只有当总中断允许位 EA 和相应的中断允许位
					（如串口中断允许位 ES等）都为 1 时，
					对应的中断才能被响应。
					*/
}

void UART_SendByte(unsigned char Byte)
{
	SBUF = Byte;                //串口数据缓存寄存器
	while(TI == 0);             //TI为0一直循环，TI为1则跳出循环
	TI = 0;                     //重新置TI为0
}

/* 串口中断函数模板  需要用的时候挪到主函数 
void UART_Routine() interrupt 4
{
    if(RI == 1)
	{
		
		RI = 0;
	}
}
*/