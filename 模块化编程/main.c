#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"

void main()
{
	while(1)
	{
		Nixie(1,1);
//		Delay(200);
		Nixie(2,2);
//		Delay(200);
		Nixie(3,3);
//		Delay(200);
	}
}