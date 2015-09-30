/* 
 * File:   lcd_config.h
 * Author: imp
 *
 * Created on September 13, 2015, 9:47 PM
 * 
 * //http://ww1.microchip.com/downloads/en/DeviceDoc/xlcdc.readme.pdf
 * //http://pic-projekte.de/wordpress/?p=908
 * 
 */

#ifndef LCD_CONFIG_H
#define	LCD_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <xlcd.h>
#include <delays.h>
      
#include <xc.h>
    
/* Interface type 8-bit or 4-bit
 * For 8-bit operation uncomment the #define BIT8
 */
/* #define BIT8 */

/* When in 4-bit interface define if the data is in the upper
 * or lower nibble.  For lower nibble, comment the #define UPPER
 */
// #define UPPER

/* DATA_PORT defines the port to which the LCD data lines are connected */
 #define DATA_PORT      		PORTC
 #define TRIS_DATA_PORT 		TRISC

/* CTRL_PORT defines the port where the control lines are connected.
 * These are just samples, change to match your application.
 */
 #define RW_PIN   LATCbits.LATC6   		/* PORT for RW */
 #define TRIS_RW  TRISCbits.TRISC6    	/* TRIS for RW */

 #define RS_PIN   LATCbits.LATC5   		/* PORT for RS */
 #define TRIS_RS  TRISCbits.TRISC5    	/* TRIS for RS */

 #define E_PIN    LATCbits.LATC4  		/* PORT for E  */
 #define TRIS_E   TRISCbits.TRISC4    	/* TRIS for E  */

void DelayFor18TCY(void){
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
 Delay1TCY();
}
void DelayPORXLCD(void){
    __delay_ms(15);//Delay1KTCYx(75); // Delay of 15ms 
}
void DelayXLCD(void){
    __delay_ms(5);//Delay1KTCYx(25); // Delay of 5ms 
}   

void OpenLCDDisplay(){
    /*while( BusyXLCD() );
    OpenXLCD(FOUR_BIT & LINES_5X7);
    putrsXLCD("Hallo World");*/
    
/*    while( BusyXLCD() );
    WriteCmdXLCD( FOUR_BIT & LINES_5X7 );
    WriteCmdXLCD( BLINK_ON );
    WriteCmdXLCD( SHIFT_DISP_LEFT );
 */
 unsigned char config=0xFF,i=0,addr=0,data=0;
ADCON1 = 0xFF;
char XLCD_Disp1[] = "MIC";
//char XLCD_Disp2[] = " PIC_UC       ";

	config = FOUR_BIT  & LINES_5X7;
//********  Configure LCD for four line communication and 5X7 line display *********************************
	OpenXLCD(config);		
		while( BusyXLCD() );		//wait untill LCD controller is busy

//*********** Set the starting address in the LCD RAM for display. This determines the location of display ********	
	SetDDRamAddr(0x80);
		while( BusyXLCD() );		//wait untill LCD controller is busy
	putsXLCD(XLCD_Disp1);			//Display string of text
		while( BusyXLCD() );		//wait untill LCD controller is busy

//********** Set the address in second line for display ****************************************************		
	SetDDRamAddr(0xC1);
		while( BusyXLCD() );		//wait untill LCD controller is busy
	putsXLCD(XLCD_Disp1);					//Display string of text
		while( BusyXLCD() );		//wait untill LCD controller is busy
	putcXLCD(0x5F);					//dsiplay some chrecter
	 	while( BusyXLCD() );		//wait untill LCD controller is busy
		 
	SetDDRamAddr(0x20);
		while( BusyXLCD() );		//wait untill LCD controller is busy   
    
}
    
    




#ifdef	__cplusplus
}
#endif

#endif	/* LCD_CONFIG_H */

