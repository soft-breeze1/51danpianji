#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"

/*
	重要思维方式【MODE 模式】
	通过一个按键(if,else和switch语句)改变两种模式(两个函数的实现)
*/

/*
BCD码转十进制：DEC = BCD/16*10 + BCD%16; (2位BCD) 只能转2位的BCD
十进制转BCD码：BCD = DEC/10*16 + DEC%10; (2位BCD)
*/

unsigned char KeyNum,MODE,TimerSetSelect,TimeSetFlashFlag;
//【KeyNum 输入数字】 【MODE 模式】 
//【TimerSetSelect 年月日时分秒修改位】 
//【TimeSetFlashFlag 闪烁标志位】(在全局变量中声明是默认为0)

/**
  * @brief    从DS1302时钟模块中读取数据到LCD1602上【显示年月日时分秒】
  * @param    无
  * @retval   无
  */
void TimerShow(void)
{
	DS1302_ReadTime();
	LCD_ShowNum(1,1,DS1302_Time[0],2);
	LCD_ShowNum(1,4,DS1302_Time[1],2);
	LCD_ShowNum(1,7,DS1302_Time[2],2);
	LCD_ShowNum(2,1,DS1302_Time[3],2);
	LCD_ShowNum(2,4,DS1302_Time[4],2);
	LCD_ShowNum(2,7,DS1302_Time[5],2);
}

/**
  * @brief    转换到【TimerSet】模式，即【时间设置】模式，
  * @brief	  输入【2】进入到修改【年，月，日，时，分，秒】
  * @brief    输入【3】将对应的时间加1   
  * @brief    输入【4】将对应的时间减1
  * @param    无
  * @retval   无
  */
void TimerSet(void)
{
	if(KeyNum == 2)
	{
		TimerSetSelect++;
//		if(TimerSetSelect>5)TimerSetSelect=0; // 【6>5时越界清零】
		//0,1,2,3,4,5代表修改【年，月，日，时，分，秒】
		TimerSetSelect%=6;  //越界清零
	}
	if(KeyNum == 3)
	{
		DS1302_Time[TimerSetSelect]++;
		if(DS1302_Time[0]>99){DS1302_Time[0]=0;}   //年
		if(DS1302_Time[1]>12){DS1302_Time[1]=1;}   //月
		if(DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || 
			DS1302_Time[1] == 7 || DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || 
			DS1302_Time[1] == 12)
		{
			if(DS1302_Time[2] > 31){DS1302_Time[2] = 1;}
		}
		else if(DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || 
			DS1302_Time[1] == 11)
		{
			if(DS1302_Time[2] > 30){DS1302_Time[2] = 1;}
		}
		else if(DS1302_Time[1] == 2)
		{
			if((DS1302_Time[0]%4==0 && DS1302_Time[0]%100!=0) || (DS1302_Time[0])%400==0)
			{
				if(DS1302_Time[2] > 29){DS1302_Time[2]=1;}   //闰年
			}
			else
			{
				if(DS1302_Time[2] > 28){DS1302_Time[2]=1;}   //平年
			}
		}                                          //日
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}   //时
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}   //分
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}   //秒
	}
	if(KeyNum == 4)
	{
		DS1302_Time[TimerSetSelect]--;
		//将DS1302_Time[]中的数据改为无符号数后，范围为【- 128 到 127】
		//判断越界就可以是 DS1302_Time[0]<0 了
		//若是【unsigned char】类型，则判断年份的越界条件为>100也可以（0变为255，255>100）
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}   //年
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}   //月
		if(DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || 
			DS1302_Time[1] == 7 || DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || 
			DS1302_Time[1] == 12)
		{
			if(DS1302_Time[2] < 1){DS1302_Time[2] = 31;}
		}
		else if(DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || 
			DS1302_Time[1] == 11)
		{
			if(DS1302_Time[2] < 1){DS1302_Time[2] = 30;}
		}
		else if(DS1302_Time[1] == 2)
		{
			if((DS1302_Time[0]%4==0 && DS1302_Time[0]%100!=0) || (DS1302_Time[0])%400==0)
			{
				if(DS1302_Time[2] < 1){DS1302_Time[2]=29;}   //闰年
			}
			else
			{
				if(DS1302_Time[2] < 1){DS1302_Time[2]=28;}   //平年
			}
		}                                          //日
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}   //时
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}   //分
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}   //秒
	}
	/*
		当进入到【TimerSet】模式时,按照【TimerSetSelect】的时间选择修改位和【闪烁标志位】
		每500ms的频率闪烁（闪烁标志位每500ms变为1）
	*/
	/*
			虽然在这段代码中没有直接调用Timer0_Routine()函数，但是中断的触发是由硬件自动进行的，
		并不需要在代码中显式地调用中断服务函数。
			在单片机系统中，当满足中断条件时（比如定时器 0 计数溢出），硬件会自动跳转到相应的
		中断服务函数（这里就是Timer0_Routine()）去执行。
			在这段代码中，可能在其他地方进行了定时器 0 的初始化配置，使得定时器 0 开始工作并在满足
		条件时触发中断。而TimerSet()函数只是根据中断服务函数中设置的标志位（如TimeSetFlashFlag）来
		进行相应的操作，并不直接控制中断的触发。
	*/
	if(TimerSetSelect == 0 && TimeSetFlashFlag == 1){LCD_ShowString(1,1,"  ");}
	else{LCD_ShowNum(1,1,DS1302_Time[0],2);}
	if(TimerSetSelect == 1 && TimeSetFlashFlag == 1){LCD_ShowString(1,4,"  ");}
	else{LCD_ShowNum(1,4,DS1302_Time[1],2);}
	if(TimerSetSelect == 2 && TimeSetFlashFlag == 1){LCD_ShowString(1,7,"  ");}
	else{LCD_ShowNum(1,7,DS1302_Time[2],2);}
	if(TimerSetSelect == 3 && TimeSetFlashFlag == 1){LCD_ShowString(2,1,"  ");}
	else{LCD_ShowNum(2,1,DS1302_Time[3],2);}
	if(TimerSetSelect == 4 && TimeSetFlashFlag == 1){LCD_ShowString(2,4,"  ");}
	else{LCD_ShowNum(2,4,DS1302_Time[4],2);}
	if(TimerSetSelect == 5 && TimeSetFlashFlag == 1){LCD_ShowString(2,7,"  ");}
	else{LCD_ShowNum(2,7,DS1302_Time[5],2);}
	
	LCD_ShowNum(2,10,TimerSetSelect,2);     //在第2行第10列的位置显示时间选择位【012345】
//	LCD_ShowNum(2,13,TimeSetFlashFlag,2);   //在第2行第13列的位置显示闪烁标志位0101的闪烁
}

void main()
{
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1,1,"  -  -  ");
	LCD_ShowString(2,1,"  -  -  ");
	
	DS1302_SetTime();
	
    while(1)
    {
		KeyNum = Key();
		if(KeyNum == 1)
		{
			if(MODE==0){MODE=1;TimerSetSelect=0;} 
			//实现在每次调为模式1时可以重新从【年】开始修改
			else if(MODE==1){MODE=0;DS1302_SetTime();}
			/*
			通过在【MODE=0】模式下,让【DS1302时钟模块】持续工作,而在【MODE==1】模式下让
			【DS1302时钟模块】不再工作,实现了调时间后可以在调完的时间基础上变动
			*/
		}
		switch(MODE)
		{
			case 0:TimerShow();break;
			case 1:TimerSet();break;
		}
    }
}

/**
  * @brief   进入到【中断1】,实现500ms闪烁一次，以【0，1，0，1】的方式闪烁
  * @param   无
  * @retval  无
  */
void Timer0_Routine() interrupt 1    //中断函数
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值     //1ms进一次中断，一次中断后，重新计数
	T0Count++;
	if(T0Count >= 500)
	{
		T0Count = 0;
		//！是 逻辑取反   ~是按位取反
		TimeSetFlashFlag  = !TimeSetFlashFlag;
	}
}