/* 
 * File:   main.c
 * Author: imp
 *
 * Created on September 26, 2015, 2:18 PM
 */


#define _XTAL_FREQ (48000000ULL) 

#include <xc.h>
#include <pic18f13k50.h>
#include "system.h"
#include "system_config.h"

#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"
//#include <string.h>
//#include <stdio.h>
#include <stdlib.h>

//LCD
#include <LCD.h>
//SPI
#include <SPI.h>

//Tools
//#include "tools.h"

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/

/*
 * UART in and output buffers
 */
static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];
//static char stringBuffer[16];



/*
 Initialize the UART over USB
 */
/*
void InitializeUSB_UART()
{
    CDCInitEP();

    
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;
    
}
*/
#define InitializeUSB_UART() CDCInitEP(); line_coding.bCharFormat = 0;line_coding.bDataBits = 8;line_coding.bParityType = 0;line_coding.dwDTERate = 9600;

//Function return the data as a const char
//the function argument is a pointer
//which can be used to read back the length
const char* ReadUSBUART(int *numBytesRead){
    //uint8_t numBytesRead;
    *numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));
  
    readBuffer[*numBytesRead]='\0';//Add this as rthe end of the string
    return (const char*)readBuffer;
}

//writes the string to the UART
//before writing check if( USBUSARTIsTxTrfReady() == true)
//one might put here a timeout
bool WriteUSBUART(const char* string){
    
    bool unsend=true;
    while(unsend){
        if( USBUSARTIsTxTrfReady() != true){
            CDCTxService();
        }
        else{
            putrsUSBUSART(string);
            unsend=false;
            CDCTxService();
        }
    }
    return true;
}
/*
bool WriteBinaryData(uint8_t *data, uint16_t length){
    
    if(length==0) return false;
    
    bool unsend=true;
    while(unsend){
        if( USBUSARTIsTxTrfReady() != true){
            CDCTxService();
            return false;
        }
        else{
            uint16_t i;
            for(i=0;i<length;i++){
                writeBuffer[i%CDC_DATA_IN_EP_SIZE]=data[i];
                if(i%CDC_DATA_IN_EP_SIZE==0 && i!=0){
                    putUSBUSART(writeBuffer,CDC_DATA_IN_EP_SIZE);
                }
            }
            //There is some data left in the buffer which needs to be written
            if(i%CDC_DATA_IN_EP_SIZE!=0 ){
                putUSBUSART(writeBuffer,i%CDC_DATA_IN_EP_SIZE-1);//fixme check if -1 is correct
            }
            CDCTxService();
            unsend=false;
        }
    }
    
    return true;
    
}
*/





MAIN_RETURN main(void)
{
    /*
     * MAX6675 :
     * From dataseheet clock has to be <4Mhz
     * conversion time >0.22s
     */
    
    
    
    //Use the internal crystal:
    //OSCCON = 0b01010010;//4MHz internal crystal
    //Timer 2 can be used as the SPI clock
    T2CON=0b00000101;

   
    //SPI configuration
    TRISBbits.TRISB4 = 1;//Input SDI
    ANSELHbits.ANS10=0;//disable analog buffer
    TRISBbits.TRISB6 =0; //OUTPUT SDK
    TRISBbits.TRISB7=0; // OUTPUT CS
    LATBbits.LATB7=1; //active low so disabled
    
    //LCD configuration
    TRISC = 0x0; //TODO select specific pins! maybe in the LCD file??
    LATC = 0x0;
    
    //initialize the SPI
    SPI_Init(); 
    

    
    OpenXLCD(FOUR_BIT & LINES_5X7);
    //while(BusyXLCD());
    
    //writeLine1("Here1:");

 //   while(BusyXLCD());
    
    
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);

    USBDeviceInit();
    USBDeviceAttach();
    

    

    
    
    while(1)
    {

//        LATCbits.LATC0!=LATCbits.LATC0;
        
      //  continue;
        
        SYSTEM_Tasks();

        #if defined(USB_POLLING)
            // Interrupt or polling method.  If using polling, must call
            // this function periodically.  This function will take care
            // of processing and responding to SETUP transactions
            // (such as during the enumeration process when you first
            // plug in).  USB hosts require that USB devices should accept
            // and process SETUP packets in a timely fashion.  Therefore,
            // when using polling, this function should be called
            // regularly (such as once every 1.8ms or faster** [see
            // inline code comments in usb_device.c for explanation when
            // "or faster" applies])  In most cases, the USBDeviceTasks()
            // function does not take very long to execute (ex: <100
            // instruction cycles) before it returns.
            USBDeviceTasks();
        #endif

            

        /* If the USB device isn't configured yet, we can't really do anything
         * else since we don't have a host to talk to.  So jump back to the
         * top of the while loop. */
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }

        /* If we are currently suspended, then we need to see if we need to
         * issue a remote wakeup.  In either case, we shouldn't process any
         * keyboard commands since we aren't currently communicating to the host
         * thus just continue back to the start of the while loop. */
        if( USBIsDeviceSuspended()== true )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }


//---------------------------------------------------- 
//Application code            
        

           
            
             
    
    //Wait 300 ms to give the temperature sensor time to collect the data        
    for(uint8_t i=0;i<100;i++){
        __delay_ms(10);
    }
            

    
    //Activate the temperature sensor        
    LATBbits.LATB7=0;//set low and collect a temperature
    __delay_ms(1);
    
    //the readback is 16 bit long 2x 8 bits
    int a=0;
            a=ReadSPI();//read the first bit
    

    a<<= 8;//shift the bit 8 places
    a |= ReadSPI(); //read the second bit, we get a 16 bit integer


 //   if (a & 0x4) {
    //if (a ==0) {    
        // no thermocouple attached!
        //while(BusyXLCD());
        //writeLine2("Error:");
        //while(BusyXLCD());
 //   }
 //   else{
        //the we need to get bit 15-4
        //shift 3 bits  
        //a >>= 3;

        //to get Celsius we need to divide by 4
        //this can be done by a bit shift
        //divide by 4
        //a>>=2;
    
    a>>=5;


        char b[5];// = "0000\0";    
        itoa(b,a,10);



        //while(BusyXLCD());
        writeLine2(b);
        WriteUSBUART(b);
        

        
  //  }


    //turn the temperature sensor SPI off
    //this is important to rigger the data capture
    LATBbits.LATB7=1;
    
    //Turn off the cursor
//    WriteCmdXLCD(BLINK_OFF & CURSOR_OFF);
         
        
 
            
            
            
//----------------------------------------------------
    }//end while
}//end main


/*******************************************************************
 * Function:        bool USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, uint16_t size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  uint16_t size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            //APP_LEDUpdateUSBStatus();
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            //APP_LEDUpdateUSBStatus();

            //Call the hardware platform specific handler for suspend events for
            //possible further action (like optionally going reconfiguring the application
            //for lower power states and going to sleep during the suspend event).  This
            //would normally be done in USB compliant bus powered applications, although
            //no further processing is needed for purely self powered applications that
            //don't consume power from the host.
            SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND);
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            //APP_LEDUpdateUSBStatus();

            //Call the hardware platform specific resume from suspend handler (ex: to
            //restore I/O pins to higher power states if they were changed during the 
            //preceding SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND) call at the start
            //of the suspend condition.
            SYSTEM_Initialize(SYSTEM_STATE_USB_RESUME);
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the 
             * demo code. */
            //APP_DeviceCDCBasicDemoInitialize();
            InitializeUSB_UART();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckCDCRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}

/*******************************************************************************
 End of File
*/