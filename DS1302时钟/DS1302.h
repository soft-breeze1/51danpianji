#ifndef __DS1302_H__
#define __DS1302_H__

extern unsigned char DS1302_Time[];
//“extern” 关键字用于声明一个在其他文件中定义的变量或函数
//数组和函数可以不加，但变量必须加

void DS1302_Init(void);
void DS1302_WriteByte(unsigned char Command,Data);
unsigned char DS1302_ReadByte(unsigned char Command);
void DS1302_SetTime(void);
void DS1302_ReadTime(void);

#endif