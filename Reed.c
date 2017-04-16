
#include "Reed.h"	
int pulses;

/*------------------------------------*/
/*---PC8 Left reed, PC9 Right reed----*/
/*------------------------------------*/
void Reed_Init(void){
	RCC->AHB1ENR |= 0x04; //IO Port C clock enable
	GPIOC->MODER &= ~(0x000F0000); //Set pins 8 and 9 as inputs
	GPIOC->OSPEEDR |= 0; //Set output speed 2MHz low speed
	GPIOC->PUPDR &= 0; //Clear the PUPDR
	GPIOC->PUPDR |= 0x00050000; //Set pins 8/9 to pull up 
}
/*------------------------------------*/
/*-------REED SENSOR RETRIEVAL--------*/
/*------------------------------------*/
int Check_Left_Reed(void){
	int Left_Reed;
	Left_Reed = GPIOC->IDR;	//assign variable the value of the PUPDR
	Left_Reed &= 0x0100;		//mask out PC8
	return Left_Reed;				//return value of PC8
}

int Check_Right_Reed(void){
	int Right_Reed;
	Right_Reed = GPIOC->IDR;	//assign variable the value of the PUPDR
	Right_Reed &= 0x0200;			//mask out PC9
	return Right_Reed;				//return value of PC9
}
/*------------------------------------*/
/*---------REED TEST ROUTINES---------*/
/*------------------------------------*/
void Left_Reed_Test(void){
	if (Check_Left_Reed() != 0){
		LED_ON(RED);		//LED on
		delay_nms(100);	//Short delay
		LED_OFF(RED);		//LED off
	}
	else{
		LED_OFF(RED); //LED off
		LED_ON(GREEN);//LED on
		delay_nms(100);//Short delay
		LED_OFF(GREEN);//LED off
	}
}

void Right_Reed_Test(void){
	if (Check_Right_Reed() != 0){
		LED_ON(RED);		//LED on
		delay_nms(100);	//Short delay
		LED_OFF(RED);		//LED off
	}
	else{
		LED_OFF(RED); //LED off
		LED_ON(GREEN);//LED on
		delay_nms(100);//Short delay
		LED_OFF(GREEN);//LED off
	}
}

void Left_Reed_Motor_Test(void){
	int distance = 0;
	int Digit1 = 0;
	int Digit2 = 0;
	int Digit3 = 0;
	int Digit4 = 0;
	long Math1 = 0;
	long Math2 = 0;
	long Math3 = 0;
	long Math4 = 0;
	
	Forwards();	//Motors forwards
	
	if (Check_Left_Reed() == 0){
		pulses += 1;		//Increment pulse count
		LED_ON(RED);		//Red LED on
		delay_nms(100);	//Short delay
		LED_OFF(RED);		//Red LED off
	}
	
	//Write_Text("Distance: ",1,0); //Writes distance to LCD
	
	distance = 1000 + (1256*pulses)/100; //2 * PI * radius = 12.56. TIMES PULSES FOR DISTANCE
	
	Digit1 = distance/1000;       //Mask out the first ADC digit
	//LCD_DATA((Digit1+0x30),TXT);  //write it to the LCD
	Math1 = Digit1*1000;					//Multiply back to a factor of 1000
	
	Math2 = distance - Math1;			//Removes the first ADC digit
	Digit2 = Math2/100;						//Masks the second ADC digit
	LCD_DATA((Digit2+0x30),TXT);	//write the second digit to the LCD
	Math3 = Digit2*100;

	Math4 = Math2 - Math3;			//Removes the first and second ADC digit
	Digit3 = Math4/10;						//Masks the third ADC digit
	LCD_DATA((Digit3+0x30),TXT);	//write the third digit to the LCD
	Math3 = Digit3*10;
	
	Digit4 = Math4 - Math3;			//Removes the fist three digits
	LCD_DATA((Digit4+0x30),TXT);//Prints fourth to LCD
	
	//Write_Text("cm ",1,13); //Writes cm to LCD
		
	delay_nms(250); //short delay
	LCD_HOME;				//Reset terminal home
	}

void Right_Reed_Motor_Test(void){
	int distance;
	int Digit1 = 0;
	int Digit2 = 0;
	int Digit3 = 0;
	int Digit4 = 0;
	long Math1 = 0;
	long Math2 = 0;
	long Math3 = 0;
	long Math4 = 0;
	
	Forwards();	//Motors forwards
	
	if (Check_Right_Reed() == 0){
		pulses += 1;		//Increment pulse count
		LED_ON(RED);		//Red LED on
		delay_nms(100);	//Short delay
		LED_OFF(RED);		//Red LED off
	}
	distance = 1000 + (1256*pulses)/100; //2 * PI * radius = 12.56. TIMES PULSES FOR DISTANCE
	
	//Write_Text("Distance: ",1,0); //Writes distance to LCD
	
	Digit1 = distance/1000;       //Mask out the first ADC digit
//LCD_DATA((Digit1+0x30),TXT);  //write it to the LCD
	Math1 = Digit1*1000;					//Multiply back to a factor of 1000
	
	Math2 = distance - Math1;			//Removes the first ADC digit
	Digit2 = Math2/100;						//Masks the second ADC digit
	LCD_DATA((Digit2+0x30),TXT);	//write the second digit to the LCD
	Math3 = Digit2*100;

	Math4 = Math2 - Math3;			//Removes the first and second ADC digit
	Digit3 = Math4/10;						//Masks the third ADC digit
	LCD_DATA((Digit3+0x30),TXT);	//write the third digit to the LCD
	Math3 = Digit3*10;
	
	Digit4 = Math4 - Math3;			//Removes the fist three digits
	LCD_DATA((Digit4+0x30),TXT);//Prints fourth to LCD
	
	//Write_Text("cm ",1,13); //Writes cm to LCD
	
	delay_nms(250); //short delay
	LCD_HOME;				//Reset terminal home
	}



