#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Nixie.h"
#include "Buzzer.h"

unsigned char KeyNum;

void main()
{
	Nixie(1,0);
    while(1)
    {
        KeyNum = Key();
		if(KeyNum)  //如果KeyNum不为0
		{
			Buzzer_Time(100);
			Nixie(1,KeyNum);
		}
    }
}