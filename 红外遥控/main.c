#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "IR.h"

unsigned char Num;       // 要显示的数字
//unsigned int Time;
unsigned char Address;
unsigned char Command;

void main()
{
	LCD_Init();    //  【LCD初始化】
	LCD_ShowString(1,1,"ADDR  CMD  NUM");  // 【地址码  命令码  数字】
	LCD_ShowString(2,1,"00    00   000");  // 初始值显示
	
	IR_Init();     //  【红外初始化】
	
	//测试代码
//	Timer0Init();
//	Timer_SetCounter(0);
//	TIMER_Run(1);
//	Delay(1);
//	Time = Timer_GetCounter();
	
//	LCD_ShowNum(2,1,Time,5);
	
    while(1)
    {
        if(IR_GetDataFlag() || IR_GetRepeatFlag())   
		//获取到键码 或 一直在按【即连发功能】
		{
			Address = IR_GetAddress();
			Command = IR_GetCommand();
			
			LCD_ShowHexNum(2,1,Address,2);
			LCD_ShowHexNum(2,7,Command,2);
			
			if(Command == IR_VOL_MINUS)
			{
				Num--;
			}
			if(Command == IR_VOL_ADD)
			{
				Num++;
			}
			
			LCD_ShowHexNum(2,12,Num,3);
		}
    }
}
