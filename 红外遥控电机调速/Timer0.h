#ifndef __TIMER0_H__
#define __TIMER0_H__

void Timer0Init(void);
void Timer_SetCounter(unsigned int Value);
unsigned int Timer_GetCounter(void);
void TIMER_Run(unsigned char Flag);

#endif