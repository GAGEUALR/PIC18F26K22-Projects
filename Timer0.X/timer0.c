

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config PWRTEN = ON

#define _XTAL_FREQ 16000000
#define LCDdata PORTB 
#define E PORTCbits.RC2
#define RW PORTCbits.RC1
#define RS PORTCbits.RC0

//internal clock delay that toggles enable bit between characters
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

void printDataToLCD(unsigned int counter)
{
    unsigned int tensDigit; 
    unsigned int onesDigit;
    char tensChar;
    char onesChar;
    //reset cursor position
    LCDdata = 0x01;
    clockE();
    //displays only one digit if the number is less than 10
    if (counter < 10)
    {
        onesDigit = counter % 10;
        onesChar = onesDigit + '0';
        LCDdata = onesChar;
        clockE();
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
        clockE();
        LCDdata = onesChar;
        clockE();
    }
}
int main(int argc, char** argv) {
    OSCCON = 0x72; //16 MHz operation
    TRISA = 0xFF; //All inputs, particularly important for RA4
    TRISB = 0x00; //All outputs for use as data lines for LCD
    TRISC = 0xF8; //All inputs but for RC0-2 which are for the LCD control lines
    //T0 setup
    T0CON = 0xE8; //1110 1000 T0 ON 8-bit Counter clocks on PGT no pre-scaler
    TMR0L = 0;
    //Set all pins to digital mode
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
     //Initialize LCD
    LCDINIT(); //call LCD to run
    RS = 1; //signal register high
    RW = 0; //write register low

    while(1)
    {
        printDataToLCD(TMR0L);
    }
    return (EXIT_SUCCESS);
}
