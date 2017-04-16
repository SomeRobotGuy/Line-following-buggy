#include "delay.h"
                                                                 
#define ms_count 168000       
#define us_count  168   

static __IO uint32_t sysTickCounter;

void SysTick_Init(void) 
{
	while (SysTick_Config(us_count) != 0) 
            {
	} // One SysTick interrupt now equals 1us
 }

// This method needs to be called in the SysTick_Handler
 void TimeTick_Decrement(void) 
{
	if (sysTickCounter != 0x00) 
           {
		sysTickCounter--;
	}
}

void delay_nus(u32 n)         //delay of  n microseconds
{
	sysTickCounter = n;
	while (sysTickCounter != 0) 
            {
	}
}

void delay_1ms(void)         //delay of 1 ms
{
	sysTickCounter = 1000;
	while (sysTickCounter != 0) 
            {
	}
}

 void delay_nms(u32 n)     //delay of n milliseconds
{
	while (n--) 
            {
		delay_1ms();
	}
}
