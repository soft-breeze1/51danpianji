#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "Key.h"
#include "AT24C02.h"
#include "Delay.h"
#include "Timer0.h"
#include "Buzzer.h"

float T,Thow;
char TLow,THigh;    //温度范围
unsigned char KeyNum;

void main()
{
	Timer0Init();        //定时器初始化
	
	DS18B20_ConvertT();      //延时个1ms，防止读到默认值【85】
	Delay(1000);
	
	/*上电时，将存储的温度范围读取到LCD上*/
	THigh = AT24C02_ReadByte(0);
	TLow = AT24C02_ReadByte(1);
	if(THigh > 125 || TLow < -55 || THigh <= TLow)       
		//防止AT24C02存储的是【非法数据】
	{
		THigh = 20;    //默认值
		TLow = 15;
	}
	
	LCD_Init();
	LCD_ShowString(1,1,"T:");
	LCD_ShowString(2,1,"TH:");
	LCD_ShowString(2,9,"TL:");
	LCD_ShowSignedNum(2,4,THigh,3);
	LCD_ShowSignedNum(2,12,TLow,3);
    while(1)
    {
		KeyNum = Key();
		/*温度读取与显示*/
        DS18B20_ConvertT();
		T = DS18B20_ReadT();
		if(T<0)
		{
			LCD_ShowChar(1,3,'-');
			Thow = -T;    //【不改变【T的原始值】】
		}
		else
		{
			LCD_ShowChar(1,3,'+');
			Thow = T;
		}
		LCD_ShowNum(1,4,Thow,3);
		LCD_ShowChar(1,7,'.');
		LCD_ShowNum(1,8,(unsigned long)(Thow*100)%100,2);
		
		/*阈值判断及显示*/
		if(KeyNum)         //如果有按键按下，才判断，不一直扫描按键是否为1234
		{
			if(KeyNum == 1)
			{
				THigh++;
				if(THigh>125){THigh = 125;}
			}
			if(KeyNum == 2)
			{
				THigh--;
				if(THigh <= TLow){THigh++;}
			}
			if(KeyNum == 3)
			{
				TLow++;
				if(TLow >= THigh){TLow--;}
			}
			if(KeyNum == 4)
			{
				TLow--;
				if(TLow<-55){TLow = -55;}
			}
			LCD_ShowSignedNum(2,4,THigh,3);
			LCD_ShowSignedNum(2,12,TLow,3);
			
			/*将温度范围存储到AT24C02中*/
			AT24C02_WriteByte(0,THigh);
			Delay(5);
			AT24C02_WriteByte(1,TLow);
			Delay(5);
		}
		if(T > THigh)
		{
			LCD_ShowString(1,13,"OV:H");
			Buzzer_Time(2000);
		}
		else if(T < TLow)
		{
			LCD_ShowString(1,13,"OV:L");
		}
		else
		{
			LCD_ShowString(1,13,"    ");
		}
    }
}

void Timer0_Routine() interrupt 1    //中断函数
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值     //1ms进一次中断，一次中断后，重新计数
	T0Count++;
	if(T0Count >= 20)      //每隔20ms进入一次
	{
		T0Count = 0;
		Key_Loop();
	}
}