#include <stdio.h>
#include <stm32f4xx.h>
#include "Delay.c" //contains delay routines
#include "LED.c" //contains LED routines
#include "LCD.c" //contains LCD routines
#include "ADC.c"//contains ADC routines
#include "Motor.c" //contains motor routines
#include "Reed.c"//contains Reed switch routines
#include "USART.c" //contains USART routines

void SysTick_Handler(void)
{
	TimeTick_Decrement(); //Routine to configure delays
}

/*------------------------------------*/
/*-----INITIALISATION ROUTINE---------*/
/*------------------------------------*/
void Init_Routine(void){
	SysTick_Init(); //defined in delay.c
	LED_Init(); //defined in LED.c
	LCD_Init(); //defined in LCD.c
	config_USART1(); //defined in uart.c
	Reed_Init(); // Defined in Reed.c	
	Motor_Init();//Defined in Motor.c
	Init_Timers_PWM();//Defined in Motor.c
	Init_ADCs(); //defined in ADC.c
}
/*-----------------------*/
/*-----MAIN CODE---------*/
/*-----------------------*/
int main (void){
		Init_Routine(); //Initialise ports
		Edit_Duty(100);	//Percentage based duty works for values > 60

	while(1){
		
		//Right_ADC_Test(); //Testing right opto sensor
		//Left_ADC_Test();	//Testing Left opto sensor
		
		PWM_Motor_Test(); 		//PWM Motor Test
		//Motor_Test_L();			//Left motor test without PWM
		//Motor_Test_R();			//Left motor test without PWM
		
		//Left_Reed_Motor_Test();		//Test for measuring distance travelled
		//Right_Reed_Motor_Test();		//Test for measuring distance travelled
		//Left_Reed_Test();					//Test left reed functionality
		//Right_Reed_Test();   			//Test right reed functionaliy
		
		//USART_SendData('A'); //Test USART transmitter
		//USART_input();			 //Test USART Reciever
		//delay_nms(10); 			 //Short delay
	}
}
