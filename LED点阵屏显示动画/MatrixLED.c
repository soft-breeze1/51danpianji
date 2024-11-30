/*
sfr (special function register)：特殊功能寄存器声明
	例：sfr PO= 0x80;
	声明PO口寄存器，物理地址为0x80
sbit (special bit)：特殊位声明
	例：sbit PO_1= 0x81； 或 sbit p0_1=PO^1;
	声明PO寄存器的第1位
可位寻址/不可位寻址：在单片机系统中，操作任意寄存器或者某一位的数据时，
必须给出其物理地址，又因为一个寄存器里有8位,所以位的数量是寄存器数量的8倍，
单片机无法对所有位进行编码，故每8个寄存器中，只有一个是可以位寻址的。
对不可位寻址的寄存器，若要只操作其中一位而不影响其它位时，可用“&=”、“|=”、
“八=”的方法进行位操作
*/
#include <REGX52.H>
#include "Delay.h"

sbit RCK = P3^5;     //SRCK    位声明   sbit P3_5 = 0xB5;  
sbit SCK = P3^6;     //SRCLK   上升沿 
sbit SER = P3^4;     //SER

//设置一个宏，将控制LED灯 列 亮的P0口定义为 【Matrix_LED_PORT】
#define Matrix_LED_PORT    P0   

/**
  * @brief    74HC595写入一个字节
  * @param    要写入的字节
  * @retval   无
  */
void _74HC595_WriteByte(unsigned char Byte)   //给8个引脚赋值
{
	//Byte 赋值给 SER
//	SER = Byte&0x80;  //取出最高位，第八位
	
	//小规则   多位数据（例如8位）传给一位（例如SER）则【非0即1】
	//例如：SER = 0x00; 则SER为0；SER = 0x27（随便任何一个数）; SER为1
	//Byte&0x80 最高位为1 则为1000 0000, SER = 1
    //Byte&0x80 最高位为0 则为0000 0000, SER = 0
	
//	SCK = 1;   //给高电平，就是一个上升沿 ，此时SER就会移进去
//	SCK = 0;   //将高电平置为低电平，为下一次移数据做准备
	
//	SER = Byte&0x40;  //移第二位
//	SCK = 1;
//	SCK = 0;
	
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		SER = Byte&(0x80>>i);   //右移i位
		SCK = 1;   //上升沿
		SCK = 0;
	}
	RCK = 1;    //把8位数据送到io口上
	RCK = 0;    //置为0，为下一次移数据做准备
}

/**
  * @brief  点阵屏初始化  
  * @param   无
  * @retval  无
  */
void MatrixLED_Init()
{
	SCK = 0;    //单片机初始化后所有io口都是高电平，需要先将SCK置为低电平
	RCK = 0;
}

/**
  * @brief    LED点阵屏显示一列数据
  * @param    Column 要选择的列，范围：0~7，0在最左边
  * @param    Data 要选择列显示的数据，高位在上，【1亮0不亮】
  * @retval   无
{
}
  */
void MatrixLED_ShowColumn(unsigned char Column,Data)
{
	_74HC595_WriteByte(Data);
	
	//选列，第一种方法
	Matrix_LED_PORT = ~(0x80>>Column); //如果传入的是(0,0x80)，
	                      //0x80, 1000 0000取反后为0111 1111，故【1亮0不亮】
	
	//第二种方法
//	if(Column == 0){P0 = ~0x80;}  //0111 1111 选作第一列
//	if(Column == 1){P0 = ~0x40;}
//	if(Column == 2){P0 = ~0x20;}
//	if(Column == 3){P0 = ~0x10;}
//	if(Column == 4){P0 = ~0x08;}
//	if(Column == 5){P0 = ~0x04;}
//	if(Column == 6){P0 = ~0x02;}
//	if(Column == 7){P0 = ~0x01;}
	
	//为了防止【段选，位选，段选，位选......】带来的【残影】现象
	//所以就可以加个延时函数
	//段选，位选，延时，位清零，段选，位选，延时，位清零......
	Delay(1);
	Matrix_LED_PORT = 0xFF;    //位清零
}