/*
 * File:   testingmain.c
 * Author: gagen
 *
 * Created on October 23, 2023, 1:09 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma PWRT = ON               //Power-up timer is on
#pragma config LVP = OFF        //Low voltage programming disabled
#pragma config OSC = INTIO2     //Internal oscillator selected
#pragma config BOR = ON         //Turn on brown-out detection
#define _XTAL_FREQ 8000000

#define RS PORTBbits.RB0
#define RW PORTBbits.RB1
#define E PORTABits.RB3
#define LCDdata PORTC

void initialize();
void enable();
void lcdCmd(char cmd);
void lcdChar(char out);
void lcdCmd(char cmd) {
   
    //just outputs the hex input 'cmd' and sets the RS and RW bits
    RS = 0;
    RW = 0;
    PORTB = cmd;
    enable();
    __delay_ms(1);
}
void enable() {

    //toggle low long enough for enable signal
    E = 1;
    __delay_ms(1);
    E = 0;
    __delay_ms(1);
    E = 1;
}
void initialize() {
   
    //unlock
    RS = 0;
    RW = 0;
    LCDdata = 0x30;
    enable();
    __delay_ms(5);
   
    //unlock again
    enable();
    __delay_ms(1);
   
    //unlock a third time
    enable();
    __delay_ms(1);
   
    //function set
    LCDdata = 0x38;
    enable();
    __delay_ms(1);
   
    //turn display off
    LCDdata = 0x08;
    enable();
    __delay_ms(1);
   
    //turn display on
    LCDdata = 0x0C;
    enable();
    __delay_ms(1);

    //entry mode set
    LCDdata = 0x06;
    enable();
    __delay_ms(1);
   
    //clear screen and send cursor home
    LCDdata = 0x01;
    enable();
    __delay_ms(1);
   
   
    //set cursor visible
    LCDdata = 0x0E;
    enable();
}

void lcdChar(char out) {
   
    //sets RS and RW bits and outputs the input char, gives delay to help visualize
    RS = 1;
    RW = 0;
    LCDdata = out;
    enable();
    __delay_ms(100);
}

void lcdStr(char* out) {
    //output characters from string until null character is reached
    while(*out != 0) {
       
        lcdChar(*out);
        out++;
    }
}
int main(int argc, char** argv) {

    OSCCON = 0x70;      //Configure the internal oscillator frequency to 8 MHz
    ADCON1 = 0x7F;      //Set all dual use analog inputs to be digital
   
    TRISA = 0xF0;       //TRISA = 1111 1000
    TRISB = 0x00;       //TRISB = 0000 0000
    TRISC
    initialize();
   

    __delay_ms(1000);
   
    lcdStr("bear");
    //move to line 2
    lcdCmd(0xC0);
    lcdStr("oak");
    //move to line 3
    lcdCmd(0x90);
    lcdStr("Saturn");
    //move to line 4
    lcdCmd(0xD0);
    lcdStr("rose");
   
    //don't let the program end
    for(;;){}
    return (EXIT_SUCCESS);
}


