
#include "Motor.h"

void Motor_Init(void){
	
	//initiate motor control pins
	RCC->AHB1ENR |= 0x04; //IO Port C clock enable
	GPIOC->MODER |= 0x00005500; // set ports 4,5,6,7, as output pins
	GPIOC->OTYPER |= 0; //Set output as Push-Pull mode
	GPIOC->OSPEEDR |= 0; //Set output speed 2MHz low speed
	GPIOC->PUPDR |= 0; //Set no pull up and pull down
	
	//initiate enable pins
	RCC->AHB1ENR |= 0x02; //IO Port B clock enable
	GPIOB->MODER |= 0x40000400; //sets ports 5 and 15 as output pins 
	GPIOB->OTYPER |= 0; //Set output as Push-Pull mode
	GPIOB->OSPEEDR |= 0; //Set output speed 2MHz low speed
	GPIOB->PUPDR |= 0; //Set no pull up and pull down
}

void Init_Timers_PWM(void)   
{
/*------------------------------------*/
/*------------Timer 3 PB5-------------*/
/*------------------------------------*/
RCC->AHB1ENR|=((1UL<<1)); //GPIOB clock enable	
GPIOB->MODER    &= ~((3UL << 2*5));    /* PB.5  is output    */
GPIOB->MODER    |=  ((2UL << 2*5));
GPIOB->AFR[0]&= ~0xf00000; //AF is timer3	
GPIOB->AFR[0]|=0x200000; //AF is timer3	
RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 		// Enable TIM3 clock	
TIM3->CCMR1|=0x6000;  // PWM mode 1: 
TIM3->CCER|=(1UL<<4);   // Capture compare 1 output enabled
TIM3->ARR=10000;     // auto reload value : PWM PERIOD
TIM3->CCR2=0;    // load capture/compare register : HI PULSE WIDTH
TIM3->PSC=0;        // 1:1 prescale
TIM3->CNT=0;        // zero counter
TIM3->BDTR|=0xc000; // main & automatic output enabled
TIM3->CR1|=0x01;    // Counter enabled		

/*------------------------------------*/
/*-----------Timer 12 PB15------------*/
/*------------------------------------*/
RCC->AHB1ENR|=((1UL<<1)); //GPIOB clock enable
	
GPIOB->MODER    &= ~((3UL << 2*15));    /* PB.15  is output    */
GPIOB->MODER    |=  ((2UL << 2*15)  ); 
	
GPIOB->AFR[1]&= ~0xf0000000; //AF is timer1	
GPIOB->AFR[1]|= 0x90000000; //AF is timer1
RCC->APB1ENR|=RCC_APB1ENR_TIM12EN; //timer1 clock enable
	
TIM12->CCMR1|=0x6000; // PWM mode 1: 
TIM12->CCER|=0x10; // Capture compare 1 output enabled
TIM12->ARR=10000; // auto reload value : PWM PERIOD
TIM12->CCR2=0; // load capture/compare register : HI PULSE WIDTH
TIM12->PSC=0; // 1:1 prescale
TIM12->CNT=0; // zero counter
TIM12->BDTR|=0xc000; // main & automatic output enabled
TIM12->CR1|=0x01; // Counter enabled
}

/*------------------------------------*/
/*-------Motor control routines-------*/
/*------------------------------------*/
void Forwards(void){
	Set_Enable(Enable_L); 	//Enable high
	Set_Motor(Motor_L1);		//PC5 high
	Clear_Motor(Motor_L2); 	//PC6 low
 
	Set_Enable(Enable_R);		//Enable high
	Set_Motor(Motor_R1);		//PC4 high
	Clear_Motor(Motor_R2);	//PC7 low
}

void Forwards_L(void){
	Set_Enable(Enable_L);		//Enable high
	Set_Motor(Motor_L1);		//PC5 high
	Clear_Motor(Motor_L2);	//PC6 low
}

void Forwards_R(void){
	Set_Enable(Enable_R);		//Enable high
	Set_Motor(Motor_R1);		//PC4 high
	Clear_Motor(Motor_R2);	//PC7 low
}

void Backwards(void){
	Set_Enable(Enable_L);		//Enable high
	Clear_Motor(Motor_L1);	//PC5 low
	Set_Motor(Motor_L2);		//PC6 high
	
	Set_Enable(Enable_R);		//Enable high
	Clear_Motor(Motor_R1);	//PC4 low
	Set_Motor(Motor_R2);		//PC7 high
}
void Backwards_L(void){
	Set_Enable(Enable_L);		//Enable high
	Clear_Motor(Motor_L1);	//PC5 low
	Set_Motor(Motor_L2);		//PC6 high
}
void Backwards_R(void){
	Set_Enable(Enable_R);		//Enable high
	Clear_Motor(Motor_R1);	//PC4 low
	Set_Motor(Motor_R2);		//PC7 high
}

void Halt(void){
	Clear_Enable(Enable_L); //Enable low
	Clear_Motor(Motor_L1);	//PC5 low
	Clear_Motor(Motor_L2);	//PC6 low
	
	Clear_Enable(Enable_R);	//Enable low
	Clear_Motor(Motor_R1);	//PC4 low
	Clear_Motor(Motor_R2);	//PC7 low
}

void Edit_Duty(int Duty){
	TIM3->CCR2 = Duty*100;	// Scaling duty to enable percentage input
	TIM12->CCR2 = Duty*100;	// Scaling duty to enable percentage input
}

/*------------------------------------*/
/*---------Motor test routines--------*/
/*------------------------------------*/

void PWM_Motor_Test(void){
	Forwards();
	delay_nms(1000);
	Halt();
	Backwards();
	delay_nms(1000);
	Halt();
	Forwards_L();
	delay_nms(1000);
	Halt();
	Backwards_L();
	delay_nms(1000);
	Halt();
	Forwards_R();
	delay_nms(1000);
	Halt();
	Backwards_R();
	delay_nms(1000);
	Halt();
}

void motor_Test_L(void){
	GPIOB->BSRR |= (1UL<<15);
	GPIOC->BSRR |= (1UL<<4);
	GPIOC->BSRR |= (1UL<<(7+16));
}


void motor_Test_R(void){
	GPIOB->BSRR |= (1UL<<5);
	GPIOC->BSRR |= (1UL<<5);
	GPIOC->BSRR |= (1UL<<(6+16));
}





