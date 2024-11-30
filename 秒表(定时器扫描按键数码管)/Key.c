#include <REGX52.H>
#include "Delay.h"

unsigned char Key_KeyNumber; //中间的缓存变量，【记录按键弹起的电平变化边沿】

/**
  * @brief   返回当前的 【Key_KeyNumber】
  * @param   无
  * @retval  Temp：当前的按键值
  */
unsigned char Key(void)
{
	unsigned char Temp = 0;  //中间变量
	//先将【Key_KeyNumber】按键值清零，再返回这个值，
	//否则每过一个Key_Loop就会一直返回【Key_KeyNumber】
	Temp = Key_KeyNumber;
	Key_KeyNumber = 0;     //清零
	return Temp;
}

/**
  * @brief    如果按键此时的电平为0，则记录当前按键按下的值
  * @param    无
  * @retval   返回按键值
  */
unsigned char Key_GetState()
{
	unsigned char KeyNumber = 0;
	
	if(P3_1 == 0){KeyNumber = 1;}
	if(P3_0 == 0){KeyNumber = 2;}
	if(P3_2 == 0){KeyNumber = 3;}
	if(P3_3 == 0){KeyNumber = 4;}
	
	return KeyNumber;
}

/**
  * @brief    每过20ms检测按键的上升沿，并记录按键值
  * @param    无
  * @retval   无
  */
void Key_Loop(void)
{
	static unsigned char NowState,LastState;   //静态变量，
	//记录【上一个按键按了什么键】和【现在按键按了什么键】
	LastState = NowState;
	NowState = Key_GetState();   //获取当前按了【什么键】即【KeyNumber的值】
	if(LastState == 1 && NowState == 0)   
	//如果按键上次按了按键1，即【KeyNumber = 1】，上次的按键电平为0，
	//而现在什么也没有按，这次的按键电平为1，【说明按键弹起】
	{
		Key_KeyNumber = 1;   //缓存变量为1，单片机响应【1】
	}
	if(LastState == 2 && NowState == 0)   //按键上次按了按键2
	{
		Key_KeyNumber = 2;
	}
	if(LastState == 3 && NowState == 0)   //按键上次按了按键3
	{
		Key_KeyNumber = 3;
	}
	if(LastState == 4 && NowState == 0)   //按键上次按了按键4
	{
		Key_KeyNumber = 4;
	}
}