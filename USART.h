#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "stdint.h"

#define TX 6
#define RX 7

void config_USART1 (void);
void USART_SendData(uint16_t Data);
unsigned int USART_Status(void);

#endif
