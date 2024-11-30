#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

// 解码

unsigned int IR_Time;         // 
unsigned char IR_State;       //

unsigned char IR_Data[4];     // 用4个元素的数组来存储32位
unsigned char IR_pData;       // 记录当前存到第几位了

unsigned char IR_DataFlag;    // 记录数据是否接收标志位
unsigned char IR_RepeatFlag;  // 
unsigned char IR_Address;     // 地址码
unsigned char IR_Command;     // 命令码

/**
  * @brief   红外初始化
  * @param   无
  * @retval  无
  */
void IR_Init(void)
{
	Timer0Init();
	Int0_Init();
}

unsigned char IR_GetDataFlag(void)
{
	if(IR_DataFlag)      // 如果IR_DataFlag为1，则清零，返回1
	{
		IR_DataFlag = 0;
		return 1;
	}
	return 0;            // 如果IR_DataFlag为0，返回0
}

unsigned char IR_GetRepeatFlag(void)
{
	if(IR_RepeatFlag)      // 如果IR_DataFlag为1，则清零，返回1
	{
		IR_RepeatFlag = 0;
		return 1;
	}
	return 0; 
}

unsigned char IR_GetAddress(void)
{
	return IR_Address;
}

unsigned char IR_GetCommand(void)
{
	return IR_Command;
}

/**
  * @brief    
  * @param   
  * @retval   
  */
void Int0_Routine(void) interrupt 0
{
	if(IR_State == 0)
	{
		Timer_SetCounter(0);
		TIMER_Run(1);
		IR_State = 1;
	}
	else if(IR_State == 1)    // 【起始信号】
	{
		IR_Time = Timer_GetCounter();   //计数器 加一 为 一微秒(1us)
		Timer_SetCounter(0);   //为了下一次计时
		if(IR_Time > 12442-500 && IR_Time < 12442+500)
		// 起始(start) 时间为 9000(低电平)+4500(高电平) = 13500 【12MHZ】
		// 但是对于 本51单片机 用的是【11.0592MHz】 ，结果为【13500/1.085 = 12442】
		// 有些误差，就上下500的范围。
 		{
			IR_State = 2;
		}
		else if(IR_Time > 10368-500 && IR_Time < 10368+500) 
		// 9000 + 2250 = 【11250】
		// 对于 本51单片机 ， 结果为【11250/1.085 = 10368】
		//【Repeat】
		{
			IR_RepeatFlag = 1;    //【说明这一帧 已经 结束了】【已经收到了】
			TIMER_Run(0);         // 停止计时器
			IR_State = 0;
		}
		else       // 其他情况，解码错误
		{
			IR_State = 1;    //转到 【本状态】【起始状态】
		}
	}
	else if(IR_State == 2)      
		//接收【遥控器红外地址，地址反码，键码，键码反码】
	{
		IR_Time = Timer_GetCounter();   //计数器 加一 为 一微秒(1us)
		Timer_SetCounter(0);   //为了下一次计时
		if(IR_Time > 1032-500 && IR_Time < 1032+500)        // 存 0 位
 		{
			IR_Data[IR_pData/8]&=~(0x01<<(IR_pData%8));      // 对应位置零
			IR_pData++;
		}
		else if(IR_Time > 2073-500 && IR_Time < 2073+500)        // 存 1 位
 		{
			IR_Data[IR_pData/8]|=(0x01<<(IR_pData%8));       // 对应位置一
			IR_pData++;
		}
		else                 // 出错
		{
			IR_pData = 0;    // 清零
			IR_State = 1;    // 变为初始状态
		}
		if(IR_pData >= 32)   //收完32位数据
		{
			IR_pData = 0;    // 清零
			if((IR_Data[0] == ~IR_Data[1]) && (IR_Data[2] == ~IR_Data[3]))
			// 验证接收数据正确
			{
				IR_Address = IR_Data[0];
				IR_Command = IR_Data[2];
				IR_DataFlag = 1;   // 代表数据已经过来了
			}
			TIMER_Run(0);  // 停止定时器
			IR_State = 0;  // 切换为【空闲状态】
		}
	}
}