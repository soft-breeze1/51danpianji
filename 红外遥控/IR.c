#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

/* 
	解码：
		0状态：表示空闲状态
		1状态：当0状态来个【下降沿】时->1状态，寻找重复repeat和start状态，读
			   出从0状态到1状态的时间，【如果1状态判断为 repeat重发 ，
			   将重发标志位置一，并将状态切换为0状态】
		2状态：如果1状态判断为 start，则转为2状态，即开始解码【101010....的
			   32位的bit数据】在验证数据接收正确后，【IR_RepeatFlag为1】将
			   状态切换为0状态。
*/
unsigned int IR_Time;         // 计时
unsigned char IR_State;       // 定义状态【0，1，2】

unsigned char IR_Data[4];     // 用4个元素的数组来存储32位数据，用【long型】会出错
unsigned char IR_pData;       // 指向当前的哪个位置【收到第几位了】如果收到32位就证明结束了

unsigned char IR_DataFlag;    // 在验证数据接收正确后，记录数据是否接收标志位
unsigned char IR_RepeatFlag;  // 重发标志位【当一直在按按键时置1】
unsigned char IR_Address;     // 地址码，防止数据放在IR_Data[4]里，边收数据边发数据
unsigned char IR_Command;     // 命令码，同上

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

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 集成到函数里

/**
  * @brief   获取 DataFlag 标志位
  * @param   无
  * @retval  1：收到数据   0：没收到数据
  */
unsigned char IR_GetDataFlag(void)
{
	if(IR_DataFlag)      // 如果IR_DataFlag为1，则清零，返回1
	{
		IR_DataFlag = 0;
		return 1;
	}
	return 0;            // 如果IR_DataFlag为0，返回0
}

/**
  * @brief    获取重发标志位
  * @param    无
  * @retval   1：重发  0：没重发
  */
unsigned char IR_GetRepeatFlag(void)
{
	if(IR_RepeatFlag)      // 如果IR_DataFlag为1，则清零，返回1
	{
		IR_RepeatFlag = 0;
		return 1;
	}
	return 0; 
}

/**
  * @brief    获取地址码
  * @param    无
  * @retval   IR_Address：地址码
  */
unsigned char IR_GetAddress(void)
{
	return IR_Address;
}

/**
  * @brief    获取命令码
  * @param    无
  * @retval   IR_Command：命令码
  */
unsigned char IR_GetCommand(void)
{
	return IR_Command;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


/**
  * @brief    解码
  * @param    无
  * @retval   无
  */
void Int0_Routine(void) interrupt 0  // 收到【下降沿】进入中断，看【Int0.c】
{
	if(IR_State == 0)    // 0状态收到【下降沿】
	{
		Timer_SetCounter(0);
		TIMER_Run(1);
		IR_State = 1;
	}
	else if(IR_State == 1)    // 【起始信号】
	{
		IR_Time = Timer_GetCounter();   // 读到【间隔时间】，用来判断是【start】
										// 还是【Repeat】
		Timer_SetCounter(0);   // 计数器清零，为了下一次计时
		
		if(IR_Time > 12442-500 && IR_Time < 12442+500)       //【start】
		// 起始(start) 时间为 9000(低电平)+4500(高电平) = 13500 【12MHZ】
		// 但是对于 本51单片机 用的是【11.0592MHz】 ，结果为【13500/1.085 = 12442】
		// 有些【误差】，就上下500的范围。
 		{
			IR_State = 2;
		}
		else if(IR_Time > 10368-500 && IR_Time < 10368+500)  //【Repeat】 
		// 9000 + 2250 = 【11250】
		// 对于 本51单片机 ， 结果为【11250/1.085 = 10368】
		{
			IR_RepeatFlag = 1;    // 按下按键时间长，已经发送按键命令了，还在
								  //按，【说明这一帧 已经 结束了】【已经收到了】
			TIMER_Run(0);         // 停止计时器
			IR_State = 0;         // 状态置为0状态
		}
		else       // 其他情况，解码错误
		{
			IR_State = 1;    //转到 【本状态】【起始状态】
		}
	}
	else if(IR_State == 2)      
		//接收【遥控器红外地址，地址反码，键码，键码反码】
	{
		IR_Time = Timer_GetCounter();
		Timer_SetCounter(0);   //为了下一次计时
		if(IR_Time > 1032-500 && IR_Time < 1032+500)         // 存 0 位
 		{
			IR_Data[IR_pData/8]&=~(0x01<<(IR_pData%8));      // 对应位置零
			/*
				对一个数组 IR_Data 中的某个字节元素进行按位操作，具体是
				将该字节中的某一位清零，例如，如果 IR_pData 是 5，那么
				IR_pData%8 等于 5，0x01 << 5 的结果就是二进制数 00100000
				取反后就是 11011111，由于掩码中要清零的位为 0，所以经过按
				位与操作后，IR_Data[IR_pData/8] 中对应的那一位就会被清零，
				而其他位保持不变。
-----------------------------------------------------------------------------
				对于掩码中为 1 的那些位：不管 IR_Data[IR_pData/8] 中对应位
				原本是 0 还是 1，按位与的结果就是 IR_Data[IR_pData/8] 中对
				应位本身的值，所以这些位会保持不变。
			
				对于掩码中为 0 的那一位（也就是要清零的位）：无论 IR_Data
				[IR_pData/8] 中对应位原本是什么值，按位与的结果都会使该位
				变为 0，从而实现了将特定的一位清零的目的。
			*/
			IR_pData++;
		}
		else if(IR_Time > 2073-500 && IR_Time < 2073+500)    // 存 1 位
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