#include "LCD.h"

unsigned char BF;
unsigned char blank[20] = {"                   "};

// Make Bits 8-15 of PORT E inputs //
void Config_portE_Input(void){
	 LCD_PORT->MODER    &= ~(
//				(3UL << 2*8) |                 
//				(3UL << 2*9) |
//				(3UL << 2*10) |
//				(3UL << 2*11) |                       
				(3UL << 2*12) |                       
				(3UL << 2*13) |
				(3UL << 2*14) |
				(3UL << 2*15) );   /* PE: 8..15 = input (00)             */
}
/*---------------------------------------------------------------------*/
// Make Bits 8-15 of PORT E outputs //
void Config_portE_Output(void){

	/* PE: 8..15 = output (01)              */   
	 LCD_PORT->MODER    |= (
//				(1UL << 2*8) |                 
//				(1UL << 2*9) |
//				(1UL << 2*10) |
//				(1UL << 2*11) |                       
				(1UL << 2*12) |                       
				(1UL << 2*13) |
				(1UL << 2*14) |
				(1UL << 2*15) );   /* PE: 8..15 = output (01)              */   
}
/*---------------------------------------------------------------------*/
// Initialise LCD Pins //
void LCD_Init(void){
		RCC->AHB1ENR  |= ((1UL <<  4) );         // Enable GPIOE clock 
   
	//Setup LCD Control Pins
/* 	Bits 3,4,5 as inputs : clear bits  first   */
	LCD_PORT->MODER    &= ~(
				(3UL << 2*3) |
				(3UL << 2*4) |
				(3UL << 2*5) );
/* 	Bits 3,4,5 as outputs										   */
	LCD_PORT->MODER    |=  (
				(1UL << 2*3) |
				(1UL << 2*4) |
				(1UL << 2*5) );   
// Setup LCD Data Bus PortE[8:15]
	Config_portE_Input();		// Clear all pins first
	Config_portE_Output();	
/* PE: 3,4,5,8..15 is output Push-Pull     */
  	LCD_PORT->OTYPER   &= ~(
  			(1UL << 3) |
				(1UL << 4) |
				(1UL << 5) |   
//				(1UL << 8) |                 
//				(1UL << 9) |
//				(1UL << 10) |
//				(1UL << 11) |                      
				(1UL << 12) |                       
				(1UL << 13) |
				(1UL << 14) |
				(1UL << 15) );   
/* PE: 3,4,5,8..15 is 50MHz Fast Speed     */									 
  	LCD_PORT->OSPEEDR  &= ~(
  			(3UL << 2*3) |
				(3UL << 2*4) |
				(3UL << 2*5) |   
//				(3UL << 2*8) |                 
//				(3UL << 2*9) |
//				(3UL << 2*10) |
//				(3UL << 2*11) |                      
				(3UL << 2*12) |                       
				(3UL << 2*13) |                    
				(3UL << 2*14) |
				(3UL << 2*15)	);   
	LCD_PORT->OSPEEDR  |=  (
				(2UL << 2*3) |
				(2UL << 2*4) |
				(2UL << 2*5) |   
//				(2UL << 2*8) |                 
//				(2UL << 2*9) |
//				(2UL << 2*10) |
//				(2UL << 2*11) |                       
				(2UL << 2*12) | 
				(2UL << 2*13) | 
				(2UL << 2*14) | 
				(2UL << 2*15) ); 
/* PE: 3,4,5,8..15 is Disabled Pull up-down  */										 
  	LCD_PORT->PUPDR    &= ~(
  			(3UL << 2*3) |
 				(3UL << 2*4) |
				(3UL << 2*5) |   
//				(3UL << 2*8) |                 
//				(3UL << 2*9) |
//				(3UL << 2*10) |
//				(3UL << 2*11) |                      
				(3UL << 2*12) |                     
				(3UL << 2*13) |
				(3UL << 2*14) |
				(3UL << 2*15) );   
											 
    	BitClr(LCD_E);          //clear enable
			BitClr(LCD_RW);					// write
    	BitClr(LCD_RS);         // command

			delay_nms(15);
      LCD_DATA(0x30,CMD);
			delay_nms(5);					//Sends 11 to the LCD in a sequence to allow both lines to activate every time
			LCD_DATA(0x30,CMD); 
			delay_nus(200);			
			LCD_DATA(0x30,CMD);

    	delay_nms(3);           //delay for LCD to initialise.
    	LCD_DATA(0x28,CMD);     //set to 4-bit interface, 2 line and 5*8 font
    	LCD_DATA(0x0f,CMD);     //cursor on, cursor position blink
			LCD_DATA(0x10,CMD);
			LCD_CLR;           	//clear display
    	LCD_DATA(0x06,CMD);     //move cursor right after write
			LCD_HOME;		//return home

}
/*---------------------------------------------------------------------*/
/*------------------------------------*/
/*---------LCD busy routine-----------*/
/*------------------------------------*/
void WaitLCDBusy(void){
		int pin_busy = 1;
    Config_portE_Input();         // sets port E to input status
		BitClr(LCD_RS);							  //COMMAND MODE
		BitSet(LCD_RW);						    //READ MODE
	
		while(pin_busy != 0){
				delay_nus(1);
				BitSet(LCD_E);			// SET Enable 
				delay_nus(1);
				pin_busy = GPIOE -> ODR;		//Reads port
				pin_busy &= (1UL << 7);			//Masks to bit 7
				delay_nus(1);								//Delay to allow instructions to go through before clearing enable 
				BitClr(LCD_E); 							//Disable the enable pin
		}
		delay_nus(30);
		Config_portE_Output();
		BitClr(LCD_RW);						    //READ MODE  
	}
/*---------------------------------------------------------------------*/
/*------------------------------------*/
/*-------SET UP IN 4BIT MODE----------*/
/*------------------------------------*/
	void LCD_DATA(unsigned char data,unsigned char type){

    	WaitLCDBusy();             		//TEST LCD FOR BUSY 
    
			LCD_PORT->BSRR = (0xF0000000);
			//LCD_PORT->ODR &= 0x00FF;		// CLEAR DATA BITS
			LCD_PORT->BSRR = (data<<8);		
			//LCD_PORT->ODR |= data<<8;		// SET DATA BITS
			delay_nus(500);        			    //SMALL DELAY
	
    	if(type == CMD)
    	{
    		BitClr(LCD_RS);       		 //COMMAND MODE
    	} 
    	else 
    	{
        	BitSet(LCD_RS);          //CHARACTER/DATA MODE
    	}

    	BitSet(LCD_E);         	     //ENABLE LCD DATA LINE
    	delay_nus(100);            		 //SMALL DELAY
    	BitClr(LCD_E);         			 //DISABLE LCD DATA LINE
				
			//shifts 8 bit data along 4 bits
			LCD_PORT->BSRR = (0xF0000000);
			//LCD_PORT->ODR &= 0x00FF;		// CLEAR DATA BITS           -Code takes the 8-bit inputs and shifts along 4-bits to make into 4-bit mode
			LCD_PORT->BSRR = (data<<12);		
			//LCD_PORT->ODR |= data<<8;		// SET DATA BITS
			delay_nus(500);        			    //SMALL DELAY
			
			BitSet(LCD_E);         	     //ENABLE LCD DATA LINE
    	delay_nus(1000);            //SMALL DELAY
    	BitClr(LCD_E);         			 //DISABLE LCD DATA LINE
}
/*---------------------------------------------------------------------*/
//void Write_Text(char*string, int line, int pos){
//	int string_write;
//	LCD_DATA(line|pos,CMD);
//	for(string_write = 0; string_write<strlen(string); string_write++){
//		LCD_DATA(string[string_write], TXT);
//	}
//}
