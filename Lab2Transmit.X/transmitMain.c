/*
 * File:   transmitMain.c
 * Author: gagen
 *
 * Created on January 31, 2024, 4:35 PM
 */
#pragma config FOSC = INTIO67   // Oscillator Selection (Internal oscillator block, port function on RA6 and RA7)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config PWRTEN = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#include <xc.h>
#include <stdlib.h>

#define _XTAL_FREQ 16000000 

#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2
#define C4 PORTBbits.RB3
#define R1 PORTBbits.RB4
#define R2 PORTBbits.RB5
#define R3 PORTBbits.RB6
#define R4 PORTBbits.RB7

unsigned char specialKey = 0b0100011; //nodes bfg set to high for 7-segment "u" case


char keyScan(void)  
{           
    PORTCbits.RC1 = 1;
        
            //Checking first column for key press
            C1 = 0; 
            C2 = 1; 
            C3 = 1; 
            C4 = 1;    
            if (!R1)
            {   
                __delay_ms(1);
                while (!R1){
                    return '1';}
            }
            if (!R2) 
            { 
                __delay_ms(1); 
                while (!R2){
                    return '4'; }
            }
            if (!R3) 
            { 
                __delay_ms(1); 
                while (!R3) {
                    return '7'; }
            }
            if (!R4) 
            { 
                __delay_ms(1); 
                while (!R4){
                     return specialKey; }
            }
            
            //Checking second column for key press
            C1 = 1; 
            C2 = 0; 
            C3 = 1; 
            C4 = 1;    
            if (!R1) 
            { 
                __delay_ms(1); 
                while (!R1){
                return '2'; }
            }
            if (!R2) 
            { 
                __delay_ms(1); 
                while (!R2){
                return '5'; }
            }
            if (!R3) 
            { 
                __delay_ms(1); 
                while (!R3){
                return '8'; }
            }
            if (!R4) 
            { 
                __delay_ms(1); 
                while (!R4){
                return '0'; }
            }
            
            //Checking the third column for key press
            C1 = 1; 
            C2 = 1; 
            C3 = 0; 
            C4 = 1;    
            if (!R1) 
            { 
                __delay_ms(1); 
                while (!R1){
                return '3'; }
            }
            if (!R2) 
            { 
                __delay_ms(1); 
                while (!R2){
                return '6'; }
            }
            if (!R3) 
            { 
                __delay_ms(1); 
                while (!R3) {
                return '9'; }
            }
            if (!R4) 
            { 
                __delay_ms(1); 
                while (!R4) {
                return specialKey;}
            }
            
            C1 = 1;
            C2 = 1;
            C3 = 1;
            C4 = 0;
            if (!R1 || !R2 || !R3 || !R4)
            {
            __delay_ms(10);
            while (!R1 || !R2 || !R3 || !R4){
                return specialKey;} 
            }
            
            
            return 0;
}

void enableInterrupts()
{
   
    INTCONbits.GIE = 1;         //global enable
    INTCONbits.RBIE = 1;        //portBenable
    INTCONbits.RBIF = 0;         //interrupt flag
    INTCON2bits.INTEDG0 = 0;    //falling edge
    INTCON2bits.RBIP = 1;       //priority high
    
    IOCB4 = 1;
    IOCB5 = 1;
    IOCB6 = 1;
    IOCB7 = 1; 
                                                                                            //change
}

void configureTransmitBaud(){
    
    TXSTA1bits.BRGH = 1;    // High speed
    BAUDCON1bits.BRG16 = 0; // 8 bit
    SPBRG = 51;            // 19200 bps
}

void configureTransmitCon(){
    RCSTA1bits.SPEN = 1;  // Enable serial port 
    TXSTA1bits.SYNC = 0; // Asynch Transfer
    TXSTA1bits.TXEN = 1; // Enable transmission
}

void configurePIC()
{
    OSCCON = 0x72; // Set clock to 16 MHz
    TRISC = 0xFC; // Set PORTC as input except for RC2
    ANSELC = 0;
    TRISB = 0xF0;
    PORTB = 0;
    ANSELB = 0;
}

void main(void) {
    
   configurePIC();
   enableInterrupts();
   configureTransmitCon();
   configureTransmitBaud();
   
    while(1)
    {
        
        PORTCbits.RC0 = 0; // Indicate transmission activity
        
        __delay_ms(100);   // Delay between transmissions
        
        PORTCbits.RC0 = 1;
        
        __delay_ms(100);   // Delay between transmissions
       
    }
      
    return;
}

void __interrupt keyPress(void)
{
    if(INTCONbits.RBIF && INTCONbits.RBIE)
    {
       PORTCbits.RC1 = 1;
        
       TXREG1 = keyScan();
      
        __delay_ms(100);
        
        PORTB = 0;
        
       INTCONbits.RBIF = 0; //Reset interrupt flag
        
       PORTCbits.RC1 = 0; // End transmission 
       
    }
    
}