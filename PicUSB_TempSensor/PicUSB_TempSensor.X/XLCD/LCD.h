/* 
 * File:   LCD.h
 * Author: imp
 * 
 *It is necessary to disable the "Link in Perepherial Library" in the"CX8 Linker options"
For some reason this box checked overwrites the settings in the local library
Which means pin changes become useless 
 * 
 * Change the ports of the LCD in the xlcd.h file
 * 
 * Set the clock frequency _XTAL_FREQ
 *
 * Created on September 20, 2015, 2:32 PM
 */

#ifndef LCD_H
#define	LCD_H

#include <xlcd.h> //local version!!
#include <xc.h>



#ifdef	__cplusplus
extern "C" {
#endif


//Since the free compiler seems not to optimize 
//we write this as a macro    
    
#define writeLine1(s) WriteCmdXLCD(0x80); __delay_ms(15); putrsXLCD(s); while(BusyXLCD());       
#define writeLine2(s) WriteCmdXLCD(0xC0); __delay_ms(15); putrsXLCD(s); while(BusyXLCD());     



 


//Extern functions needed by xlcd.h    
    
/*------------------------------------------------------------------------*/
void DelayFor18TCY(void)
{
    __delay_us(18);

}

#define DelayFor18TCY() __delay_us(18);

/*------------------------------------------------------------------------*/
void DelayPORXLCD(void)
{
     __delay_ms(15);

    }
/*------------------------------------------------------------------------*/
void DelayXLCD(void)
{
    __delay_ms(5);

    }
/*------------------------------------------------------------------------*/
    


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

