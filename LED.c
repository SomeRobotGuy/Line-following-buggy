
#include "LED.h"

void LED_Init(void){
	RCC->AHB1ENR |= 0x08; //IO Port D clock enable
	GPIOD->MODER |= 0x11000000; //Set LED's 12 & 14 as output PINs
	GPIOD->OTYPER |= 0; //Set output as Push-Pull mode
	GPIOD->OSPEEDR |= 0; //Set output speed 2MHz low speed
	GPIOD->PUPDR |= 0; //Set no pull up and pull down
}

void LED_ON(int LED){
	GPIOD->BSRR |= (1UL<<LED); //Function for seting LEDs
}

void LED_OFF(int LED){
	GPIOD->BSRR |= (1UL<<(LED+16));	//Function for clearing LEDs
}
