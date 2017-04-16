
#include "ADC.h"
int ADC_A;
int ADC_B;

void Init_ADCs(void){
	
	RCC->AHB1ENR |= 0x02; //port B clock enable
	
/*------------------------------------*/
/*----------RIGHT SENSOR--------------*/
/*------------------------------------*/
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //ADC1 clock enabled
	GPIOB->MODER |= (3<<(PB0*2)); // Sets port pin PB0 as analogue inputs
	ADC1->SQR3 |= CH8;
	ADC1->CR2 |= 0x01; //turns on ADC1
	
/*------------------------------------*/
/*-----------LEFT SENSOR--------------*/
/*------------------------------------*/
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN; //ADC2 clock enabled
	GPIOB->MODER |= (3<<(PB1*2)); // Sets port pin PB1 as analogue inputs
	ADC2->SQR3 |= CH9;
	ADC2->CR2 |= 0x01; //turns on ADC2
}
/*------------------------------------*/
/*------Reading Opto signals----------*/
/*------------------------------------*/
unsigned short int Read_Right_ADC (void)
{
	ADC1->CR2|=(1<<30); //start conversion by setting SWSTART bit
	while(!(ADC1->SR & ADC_SR_EOC)); //wait for EOC bit while it is not set
	return ADC1->DR; //return value
}

unsigned short int Read_Left_ADC (void)
{
	ADC2->CR2|=(1<<30); //start conversion by setting SWSTART bit
	while(!(ADC2->SR & ADC_SR_EOC)); //wait for EOC bit while it is not set
	return ADC2->DR; //return value
}

 void ADC_Read_Data()
{
	int analogueL;
	int analogueR;
	 
	analogueL = Read_Left_ADC(); //Assign left sensor value to analogueL
	analogueR = Read_Right_ADC();//Assign right sensor value to analogueR
	
	if (analogueL > 2000){	
		ADC_A = 1;	//High after selected sensor value 
	}
	else{
		ADC_A = 0;	//Low if below selected value
	}
	if (analogueR > 2000){
		ADC_B = 1;		//High aftr selected sensor value
	}
	else {
		ADC_B = 0;	//Low if below selected value
 }
}	
 
/*------------------------------------*/
/*---------ADC TEST ROUTINES----------*/
/*------------------------------------*/

void Left_ADC_Test(){
	
		ADC_Read_Data(); 
		if (ADC_A == 1){ 
			LED_ON(GREEN);		//turn on green LED
			LED_OFF(RED);			//turn off red LED
			LCD_HOME;					//Send LCD to starting position
			LCD_DATA(1 + 0x30,TXT);	//Write '1' to LCD
		}
		else{
			LED_ON(RED);		//turn on red LED
			LED_OFF(GREEN);	//turn on green LED
			LCD_HOME;				//Send LCD to starting position
			LCD_DATA(0 + 0x30,TXT);//Write '0' to LCD
		}
}	
void Right_ADC_Test(){
	
		ADC_Read_Data(); 
		if (ADC_B == 1){ 
			LED_ON(GREEN);		//turn on green LED
			LED_OFF(RED);			//turn off red LED
			LCD_HOME;					//Send LCD to starting position
			LCD_DATA(1 + 0x30,TXT);	//Write '1' to LCD
		}
		else{
			LED_ON(RED);		//turn on red LED
			LED_OFF(GREEN);	//turn on green LED
			LCD_HOME;				//Send LCD to starting position
			LCD_DATA(0 + 0x30,TXT);//Write '0' to LCD
		}
}

