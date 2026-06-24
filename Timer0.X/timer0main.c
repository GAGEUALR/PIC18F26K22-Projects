/*
 * File:   timer0main.c
 * Author: gagen
 *
 * Created on October 31, 2023, 12:47 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config PWRTEN = ON

#define _XTAL_FREQ 16000000
#define LCDdata PORTC
#define RW PORTBbits.RB1
#define RS PORTBbits.RB0
#define E PORTBbits.RB2

//internal clock delay that toggles enable bit between characters
void enable() {

    E = 1;
    __delay_ms(1);
    E = 0;          //get microcontroller ready 
    __delay_ms(1);
    E = 1;
}

//calls & runs LCD for character input
void initializeLCD(){
    
    __delay_ms(20);
    RS = 0;
    RW = 0;
    
    
    LCDdata = 0x30;
    enable();
    LCDdata = 0x30;     //unlock
    enable();
    LCDdata = 0x30;
  
    
    __delay_ms(5);
    
    enable();
    LCDdata = 0x3C;     //Function set command
    
    enable();
    LCDdata = 0x08;     //turn display off
    
    enable();   
    LCDdata = 0x0C;     //Turn display on
    
    enable();
    LCDdata = 0x06;     //entry mode set command
    
    enable();
    
    LCDdata = 0x01;     //clear screen and send home
    
    enable();
    //FINISHED INIT
}

void printDataToLCD(unsigned int counter)
{
    unsigned int tensDigit; 
    unsigned int onesDigit;
    char tensChar;
    char onesChar;
    //reset cursor position
    LCDdata = 0x01;
    enable();
    //displays only one digit if the number is less than 10
    if (counter < 10)
    {
        onesDigit = counter % 10;
        onesChar = onesDigit + '0';
        LCDdata = onesChar;
        enable();
    }
    //displays two digits based on the assumption that the switch will not be
    //pressed more than 50 times
    else
    {
        tensDigit = counter / 10;
        onesDigit = counter % 10;
        tensChar = tensDigit + '0';
        onesChar = onesDigit + '0';
        LCDdata = tensChar;
        enable();
        LCDdata = onesChar;
        enable();
    }
}
int main(int argc, char** argv) {
    OSCCON = 0x72; //16 MHz operation
    TRISA = 0xFF; //All inputs, particularly important for RA4
    TRISB = 0x00; //All outputs for use as data lines for LCD
    TRISC = 0x00; //All inputs but for RC0-2 which are for the LCD control lines
    //T0 setup
    T0CON = 0xE8; //1110 1000 T0 ON 8-bit Counter clocks on PGT no pre-scaler
    TMR0L = 0;
    //Set all pins to digital mode
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
     //Initialize LCD
    
    initializeLCD(); //call LCD to run
    
    RS = 1; //signal register high
    RW = 0; //write register low
    unsigned int counter = TMR0L;
    while(1){
        printDataToLCD(counter);
    }
    
    return (EXIT_SUCCESS);
}
