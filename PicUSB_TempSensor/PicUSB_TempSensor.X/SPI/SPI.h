/* 
 * File:   SPI.h
 * Author: imp
 *
 * Configure pins as input and output
 * 
 * 
 * Created on September 20, 2015, 8:12 PM
 */



#ifndef SPI_H
#define	SPI_H

#include <xc.h>

// Master SPI mode only

#define   SMPEND        0b10000000           // Input data sample at end of data out             
#define   SMPMID        0b00000000           // Input data sample at middle of data out


//Bus mode
#define   MODE_00       0b00000000              // Setting for SPI bus Mode 0,0
//CKE           0x40                   // SSPSTAT register 
//CKP           0x00                   // SSPCON1 register 

#define   MODE_01       0b00000001              // Setting for SPI bus Mode 0,1
//CKE           0x00                   // SSPSTAT register 
//CKP           0x00                   // SSPCON1 register

#define   MODE_10       0b00000010              // Setting for SPI bus Mode 1,0
//CKE           0x40                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

#define   MODE_11       0b00000011              // Setting for SPI bus Mode 1,1
//CKE           0x00                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

//sync mode

/* SSPCON1 REGISTER */
#define   SSPENB        0b00100000           // Enable serial port and configures SCK, SDO, SDI

#define   SPI_FOSC_4    0b00000000              // SPI Master mode, clock = Fosc/4
#define   SPI_FOSC_16   0b00000001              // SPI Master mode, clock = Fosc/16
#define   SPI_FOSC_64   0b00000010              // SPI Master mode, clock = Fosc/64
#define   SPI_FOSC_TMR2 0b00000011              // SPI Master mode, clock = TMR2 output/2
#define   SLV_SSON      0b00000100              // SPI Slave mode, /SS pin control enabled
#define   SLV_SSOFF     0b00000101              // SPI Slave mode, /SS pin control disabled

//Clock polarity
#define   myCKP           0b0001000


#ifdef	__cplusplus
extern "C" {
#endif
    
    /*tiate a new conversion
process by forcing CS high.
     * Read the 16 output bits on the falling edge of the clock. 
     */
/*  void SPI_Init(){
    SSPSTAT &= 0x3F;               // power on state 
    SSPCON1 = 0x00;                // power on state
    
    
    SSPCON1 |= SPI_FOSC_TMR2; // SPI_FOSC_64; //SPI_FOSC_TMR2;//SPI_FOSC_64;          // select serial mode 
    SSPSTAT |= SMPMID;          // select data input sample phase
    
    
      switch( MODE_11)
  {
    case 0:                       // SPI1 bus mode 0,0
      SSPSTATbits.CKE = 1;       // data transmitted on rising edge
      break;    
    case 2:                       // SPI1 bus mode 1,0
      SSPSTATbits.CKE = 1;       // data transmitted on falling edge
      SSPCON1bits.CKP = 1;       // clock idle state high
      break;
    case 3:                       // SPI1 bus mode 1,1
      SSPCON1bits.CKP = 1;       // clock idle state high
      break;
    default:                      // default SPI1 bus mode 0,1
      break;
  }
    
    SSPCON1 |= SSPENB;
    
    ///IFSbits.SPI2IF = 0;	// clear interrupt flag
	//IEC2bits.SPI2IE = 1;	// enable interrupt
//    SSPCON1 = 0b00100000;  // select mode
//    SSPSTAT = 0b01000000;  // enable the peripheral
}    */

//the enable (SSPENB( at the end has to be called
#define SPI_Init() SSPSTAT =0b00111111; SSPCON1 =0b00010010; SSPCON1 |= SSPENB;
//#define ReadSPI() SSPBUF = 0x01; while(!SSPSTATbits.BF); return SSPBUF;     

unsigned char ReadSPI(){
 
    //PIE1bits.SSPIE = 0;
    
    //PIR1bits.SSPIF = 0;     //Clear interrupt flag
    //Write into the buffer to start the transfer
    //SPI needs dummy a byte for transfer to read
    //since write and read happen syncrone
    SSPBUF = 0x01;// initiate bus cycle

    while(!SSPSTATbits.BF);
    //while(!PIR1bits.SSPIF); //wait until cycle complete
    return SSPBUF;
}

    
// send one byte of data and receive one back at the same time
/*unsigned char writeSPI2( unsigned char i )
{
    SPI2BUF = i;					// write to buffer for TX
    while(!SPI2STATbits.SPIRBF);	// wait for transfer to complete
    return SPI2BUF;    				// read the received value
}//writeSPI2
*/
/************************************************************************
* Function: SPI2INTInit                                                 *
*                                                                       *
* Preconditions: TRIS bit of Slave Chip Select pin (if any used) should *
* be made output. This function is used for initializing the SPI module.*
* It initializes the module according to Application Maestro options.   *
*                                                                       *
* Input: Application Maestro options                                    *
*                                                                       *
* Output: None                                                          *
*                                                                       *
************************************************************************/ 
/*void SPI2INTInit()
{
    IFS2bits.SPI2IF = 0;	// clear interrupt flag
	IEC2bits.SPI2IE = 1;	// enable interrupt
    SPI2CON1 = SPI_MASTER;  // select mode
    SPI2STAT = SPI_ENABLE;  // enable the peripheral
} // SPI2INTInit

void __attribute__((interrupt, no_auto_psv)) _SPI2Interrupt()
{
	// add code here 
	
	IFS2bits.SPI2IF = 0;
	
}    
*/


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

