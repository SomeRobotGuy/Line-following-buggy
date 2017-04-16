

#include "usart.h"

void config_USART1 (void)
{
	unsigned int temp;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	// enable the clock to USART1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 	// enable the clock to GPIOB

	CLEAR_BIT(GPIOB->MODER,3UL<<(TX*2));	//
	SET_BIT(GPIOB->MODER,2UL<<(TX*2));		// Set TX bit to Alternate function
	CLEAR_BIT(GPIOB->MODER,3UL<<(RX*2)); 	//
	SET_BIT(GPIOB->MODER,2UL<<(RX*2)); 		//set RX bit to Alternate function
	
	CLEAR_BIT(GPIOB->OTYPER, 1UL<<TX);		// set to push-pull
	CLEAR_BIT(GPIOB->OTYPER, 1UL<<RX);		// set to push-pull
	CLEAR_BIT(GPIOB->OSPEEDR, 3UL<<(TX*2));// 2MHz speed
	CLEAR_BIT(GPIOB->OSPEEDR, 3UL<<(RX*2));// 2MHz speed
  CLEAR_BIT(GPIOB->PUPDR, 3UL<<(TX*2));	// no pullup or pulldown
  CLEAR_BIT(GPIOB->PUPDR, 3UL<<(RX*2));	// no pullup or pulldown

	GPIOB->AFR[0] |= 0x07<<(TX*4);				// Set Alternate Function Register for pin6
	GPIOB->AFR[0] |= 0x07<<(RX*4);				// Set Alternate Function Register for pin7
	
	CLEAR_BIT(USART1->CR2, (1<<12|1<<13));// Clear STOP bits = 1 STOP BIT
	CLEAR_BIT(USART1->CR1, 1<<15);				// 16 bit oversampling
	CLEAR_BIT(USART1->CR1, 1<<12);				// 1 start bit 8 Data bits
	CLEAR_BIT(USART1->CR1, 1<<10);				// Disable PARITY
	CLEAR_BIT(USART1->CR1, 1<<9);					// EVEN Parity
	SET_BIT(USART1->CR1, 1<<3); 					// Transmitter enable
	SET_BIT(USART1->CR1, 1<<2);						// Reciever enable
	
/*	  if ((USARTx == USART1) || (USARTx == USART6))
  {apbclock = RCC_ClocksStatus.PCLK2_Frequency;}
  else
  {apbclock = RCC_ClocksStatus.PCLK1_Frequency;}
*/
	//BRR  Bits 15:4 (12 bit)Mantissa, 3:0 (4 bit)Fraction
 /* Oversampling mode is 16 Samples */
  temp = (((168000000/4)<<5) / (16 * 9600));//twice BRR needed for LSB accuracy 0.5%
  USART1->BRR = (temp>>1) + (temp&0x1);
	SET_BIT(USART1->CR1, 1<<13);							//USART Enable
}
//----------------------------------------------------------------------------
void USART_SendData(uint16_t Data)
{
  USART1->DR = (Data & (uint16_t)0x01FF);
}
//----------------------------------------------------------------------------
unsigned int USART_Status(void)
{
unsigned int status;
  status = (USART1->SR & (1<<6));
  
  return status;
}
//----------------------------------------------------------------------------
/* Use no semihosting */
#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{  
	int handle;
};
FILE __stdout;

void _sys_exit(int x)
{
	while(1);
}
#endif

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData((uint8_t) ch);

  /* Loop until the end of transmission */
  while ((USART1->SR & (1<<6)) == 0)
	{}

  return ch;
}

/*------------------------------------*/
/*--------RXNE BUSY ROUTINE-----------*/
/*------------------------------------*/
void USART_input(void){
	int input;
	int READ_USART1;
	READ_USART1 = USART1->SR;	//Variable to access register indirectly
	READ_USART1 &= 0x20;			//Masks RXNE bit
	if (READ_USART1 != 0){	//RXNE = 1 Recieved data is ready to be read
		input = USART1->DR;		//Read recieved data
		delay_nms(100);
		LCD_DATA(input,TXT);	//Displays recieved data onto the LCD
		LED_ON(RED);				//RED LED on
		LED_OFF(GREEN);  //GREEN LED off
	}		
	else{
		LED_ON(GREEN);				//GREEN LED on
		LED_OFF(RED);	//RED LED off
	}

		READ_USART1 = 0;					//Reset variable so no overlap of data
}
