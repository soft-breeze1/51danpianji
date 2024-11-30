#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Nixie.h"
#include "Motor.h"
#include "IR.h"

unsigned char Command,Speed;

void main()
{
	Motor_Init();
	IR_Init();
    while(1)
    {
		if(IR_GetDataFlag())                       //如果收到数据
		{
			Command = IR_GetCommand();             //获取键码(命令码)
			
			if(Command == IR_0){Speed = 0;}        //按键为1，速度为0
			if(Command == IR_1){Speed = 1;}
			if(Command == IR_2){Speed = 2;}
			if(Command == IR_3){Speed = 3;}
			
			if(Speed == 0){Motor_SetSpeed(0);}     //当速度设置为0，马达电机设置阈值为0 
			if(Speed == 1){Motor_SetSpeed(40);}
			if(Speed == 2){Motor_SetSpeed(70);}
			if(Speed == 3){Motor_SetSpeed(100);}   //Counter < 100
		}
		Nixie(1,Speed);                            //晶体管显示速度等级【0，1，2，3】
    }
}
