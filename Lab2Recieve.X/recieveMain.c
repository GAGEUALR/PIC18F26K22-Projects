/*
 * File:   receiveMain.c
 * Author: gagen
 *
 * Created on January 31, 2024, 4:34 PM
 */
#pragma config FOSC = INTIO67   // Oscillator Selection (Internal oscillator block, port function on RA6 and RA7)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config PWRTEN = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#include <xc.h>
#include <stdlib.h>

#define _XTAL_FREQ 16000000 


void configurePIC()
{
    TRISA = 0x00; // Set PORTA as output
    TRISB = 0xF8; // Set PORTB as input
    TRISC = 0xFA; // Set PORTC as input except for RC2
    
    PORTA = 0x00; // Initialize PORTA
    ANSELC = 0;
    
    OSCCON = 0x72; // Set clock to 16 MHz
}

void configureReceiveBaud(){
    
    TXSTA1bits.BRGH = 1; // High speed
    BAUDCON1bits.BRG16 = 0; // 8 bit
    SPBRG = 51;           // 19200 bps
}

void configureReceiveCon()
{
    TRISCbits.RC6 = 1;      //input
    TRISCbits.RC7 = 1;      //input
    RCSTA1bits.SPEN = 1; // Enables EUSART
    TXSTA1bits.SYNC = 0; // Asynchronous operation
    RCSTA1bits.CREN = 1; // Enables receiver circuit
}

void configureInterrupts()
{
    INTCONbits.GIE = 1; // Enable Global Interrupts
    INTCONbits.PEIE = 1; // Enable Peripheral Interrupts
    PIE1bits.RC1IE = 1; // Enable reception interrupts
}

unsigned char receivedData;
unsigned char specialKey = 0b0100011; //nodes bfg set to high for 7-segment "u" case

int U = 0xF8;
int OFF = 0xFF;

void __interrupt() ISR(void)
{
        if (PIR1bits.RC1IF && PIE1bits.RC1IE) // Check if UART receive interrupt flag is set & interrupt is enabled
            {
                receivedData = RCREG1; // Read received data
        
               
                PORTB = OFF;
                PORTA = OFF;
                
                if (receivedData == 0x2A)
                {
                    PORTA = OFF;
                    PORTB = U;
                }
                if (receivedData == specialKey)
                    {
                    PORTA = OFF;
                    PORTB = U;
                    }
                
                if (receivedData == 0)
                    {
                    PORTB = OFF ;
                    PORTA = OFF;
                    }
                
                if (receivedData != 0 && receivedData != specialKey)
                  { 
                    PORTA = receivedData;
                    PORTB = OFF;
                  }
              
              
                
                
                PIR1bits.RC1IF = 0; // Clear receive interrupt flag
             }
}



int main(void) {
    
    configurePIC();
    configureReceiveBaud();
    configureReceiveCon();
    configureInterrupts();
    
    while (1)
    {
        PORTCbits.RC2 = 1; // Indicate activity on PORTC RC2
    }
    return 0;
}





