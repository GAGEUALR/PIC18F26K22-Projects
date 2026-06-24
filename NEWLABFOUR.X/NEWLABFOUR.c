#include <xc.h>
#include <stdlib.h>
#include <stdio.h>


#pragma config FOSC = INTIO67   // oscillator set
#pragma config WDTEN = OFF      // watchdog timer off
#pragma config PWRTEN = ON      // power-up timer active
#define _XTAL_FREQ 16000000
#pragma config LVP = OFF        //Low voltage programming OFF


#define RS PORTBbits.RB0
#define RW PORTBbits.RB1
#define LCDdata PORTC

char key = '1';        // create key char globally, initialize empty



void enable() {

    PORTBbits.RB2 = 1;
    __delay_ms(1);
    PORTBbits.RB2 = 0;          //get microcontroller ready 
    __delay_ms(1);
    PORTBbits.RB2 = 1;
}

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
    
    enable();
    LCDdata = 0x01;     //clear screen and send home
    
    enable();
    //FINISHED INIT
}


void displayKey(char key) {
                                   
                                
    RS = 1;                     //RS = 1
    PORTC = key;                // Send keypad character to LCD
    enable();
}

int main(int argc, char** argv){
    
    OSCCON = 0x72;     //configure oscillator 
    
    TRISC = 0x00;
    TRISB = 0x00;       //<7:4> must be inputs for keypad
    TRISA = 0x00;
    
    ANSELC = 0;
    ANSELB = 0;         //set all ports digital
    ANSELA = 0;
    
    initializeLCD();
    
    __delay_ms(100);        //delay to let LCD sit
    

    while(1){
        displayKey(key);
    }
    
    return (EXIT_SUCCESS);
    
}



