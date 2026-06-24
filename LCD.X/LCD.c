/*
 * File:   LED.c
 * Author: gagen
 *
 * Created on November 13, 2023, 11:25 PM
 */


#include <xc.h>
#include <stdlib.h>

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config PWRTEN = ON

#define E PORTBbits.RB3
#define RW PORTBbits.RB2
#define RS PORTBbits.RB1
#define LCDdata PORTC
#define _XTAL_FREQ 16000000

int signalValue;

char lcdOUT[] = "freq = 1KHZ";


void clockE(){
    E = 1;
    __delay_ms(1);
    E = 0;
    __delay_ms(1);
    E = 1;
}

//calls & runs LCD for character input
void LCDINIT(){
    
    __delay_ms(20);
    
    RS = 0;
    RW = 0;
    
    clockE();
    LCDdata = 0x30;
    clockE();
    __delay_ms(5);
    clockE();
    __delay_ms(1);
    clockE();
    __delay_ms(1);
    LCDdata = 0x38;
    clockE();
    __delay_ms(1);
    LCDdata = 0x08;
    clockE();
    __delay_ms(1);
    LCDdata = 0x0C;
    clockE();
    __delay_ms(1);
    LCDdata = 0x06;
    clockE();
    __delay_ms(1);
    LCDdata = 0x01;
    clockE();
    //FINISHED INIT
}


void printDataToLCD(){
    
    RS = 0;
    LCDdata = 0x01;
    clockE();
    
    __delay_ms(2);
    
    int i;
    
    RS = 1;
        for(i = 0; (lcdOUT[i] != '\0'); i++ )
        {
            LCDdata = lcdOUT[i];
            clockE();
      }
}
void main(void) {
    
    TRISB = 0xF0;
    TRISC = 0x00;
    ANSELB = 0;
    ANSELC = 0;
    
    LCDINIT();
 
    printDataToLCD();
        
    while(1)
    {
        __delay_ms(10000);
    };
    
    return;
}
