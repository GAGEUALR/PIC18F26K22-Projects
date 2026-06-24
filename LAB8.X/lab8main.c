/*
 * File:   lab8main.c
 * Author: gagen
 *
 * Created on November 7, 2023, 12:33 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config FOSC = INTIO67 //internal oscillator
#pragma config WDTEN = OFF //watchdog timer
#pragma config PWRTEN = ON //power

#define _XTAL_FREQ 16000000 //16 MHz frequency

#define LED PORTBbits.RB0


int main(int argc, char** argv) {
    OSCCON = 0x72; //16 MHz operation
    TRISA = 0xFF;
    TRISB = 0xFE;
    TRISC = 0xFF;
    ANSELA = 0x0D;
    ANSELB = 0;
    ANSELC = 0;
    PORTBbits.RB0 = 0;
    
    //Comparator setup
    CM1CON0 = 0x08;
    CM2CON0 = 0x08;
    
    CM1CON0bits.C1ON = 1;
    CM2CON0bits.C2ON = 1;
    
    while(1)
    {
        if (!CM1CON0bits.C1OUT && CM2CON0bits.C2OUT)
            LED = 1;
        else 
            LED = 0;
    }
    
    return (EXIT_SUCCESS);
}
