#ifndef __LCD_H
#define __LCD_H

#define LCD_PORT GPIOE    //LCD DATA PORT : 8 data line bit 8-15
#define LCD_OUT  GPIOE->ODR
#define LCD_IN	 GPIOE->IDR

#define LCD_RS   3        //LCD Command/Data Control on Pin 3
#define LCD_RW	 4				//LCD Read/Write Select on Pin 4
#define LCD_E    5        //LCD Enable Line on Pin 5

#define CMD 		 0
#define TXT 		 1
#define CLEAR		 1
#define HOME		 2
#define READ 		 1
#define WRITE 	 0
#define LEFT 		 0
#define RIGHT 	 1

#define LINE1    0x80        // Start address of first line
#define LINE2    0xC0        // Start address of second line

#define BitClr(var) 	(LCD_PORT->BSRR |= (1<<(var+16)))
#define BitSet(var) 	(LCD_PORT->BSRR |= (1<<var))
#define LCD_CLR				(LCD_DATA(CLEAR,CMD))
#define LCD_HOME			(LCD_DATA(HOME,CMD))

void WaitLCDBusy(void);
void LCD_Init(void);
void LCD_DATA(unsigned char nyb,unsigned char type);


#endif
