/*------------------------------------------------*/
/*-------------Header File for Motor.c------------*/
/*------------------------------------------------*/

#ifndef __MOTOR_H
#define __MOTOR_H

#define Enable_L	5
#define Enable_R	15

#define Motor_L1 5
#define Motor_L2 6

#define Motor_R1 4
#define Motor_R2 7

#define Set_Motor(pin) 		GPIOC->BSRR |= (1UL<<(pin))
#define Clear_Motor(pin) 	GPIOC->BSRR |= (1UL<<(pin + 16))
#define Set_Enable(pin) 	GPIOB->BSRR |= (1UL<<(pin))
#define Clear_Enable(pin) GPIOB->BSRR |= (1UL<<(pin + 16))

void Motor_Init(void);
void Init_Timers_PWM(void);

void Forwards(void);
void Forwards_L(void);
void Forwards_R(void);

void Backwards(void);
void Backwards_L(void);
void Backwards_R(void);

void Halt(void);

void Edit_Duty(int Duty);


#endif
