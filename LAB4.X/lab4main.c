#include <xc.h>
#include <stdlib.h>
#include <stdio.h>


#pragma config FOSC = INTIO67   // oscillator set
#pragma config WDTEN = OFF      // watchdog timer off
#pragma config PWRTEN = ON      // power-up timer active

char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},    //define keypad matrix, to be used to display
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char key = '\0';        // create key char globally, initialize empty



void enable() {

    PORTCbits.RC3 = 1;
    __delay_ms(1);
    PORTCbits.RC3 = 0;          //get microcontroller ready 
    __delay_ms(1);
    PORTCbits.RC3 = 1;
}

void initializeLCD() {
   
    
    PORTCbits.RC1 = 0;
    RW = 0;                 //unlock LCD
    PORTA = 0x30;
    enable();
    
    __delay_ms(15);         //wait for more than 15ms
   
   
    enable();
    __delay_ms(1);          //unlock LCD
   
                      
    enable();
    __delay_ms(1);         //unlock LCD
   
    
    PORTA = 0x38;
    enable();               //set data interface length/#of display lines
    __delay_ms(1);
   

    PORTA = 0x08;
    enable();                //turn display off
    __delay_ms(1);
   

    PORTA = 0x0C;
    enable();               //turn display on 
    __delay_ms(1);

 
    PORTA = 0x06;
    enable();               //increment cursor after each byte written
    __delay_ms(1);
   
    
    PORTA = 0x01;
    enable();           //clear display
    __delay_ms(1);
   
   
    PORTA = 0x0E;   //turn cursor on, no blink
    enable();
    
    PORTA = 0x02;   //send cursor &LCD home
    enable();
    
}


void displayKey(key) {
                                   
    PORTCbits.RC1 = 0;          //RS = 0
    PORTA = 0x02;               // Send cursor to home
    enable();
    
                                
    PORTCbits.RC1 = 1;          //RS = 1
    PORTA = key;                // Send keypad character to LCD
    enable();
}


void interrupt keyPressed(){
    
    if (INTCONbits.RBIF) {                  //if interrupt flag is ready
        
        int i = {};             //create variable to loop with, initialize empty
        
        for (i; i < 4; i++) {       //start loop through keypad
            
            PORTB &= 0xF0;  // clear column bits
            
            PORTB |= (1 << (i + 4));  // Set the current row low and others high
                                      // using bit shifting and OR operator
            if (PORTBbits.RB0 == 0)
                key = keypad[i][0];
            else if (PORTBbits.RB1 == 0)
                key = keypad[i][1];           //looping through rows, defining
            else if (PORTBbits.RB2 == 0)      //key variable if found
                key = keypad[i][2];
            else if (PORTBbits.RB3 == 0)
                key = keypad[i][3];

            if (key != '')  // breaks loop if key is found, keeping value given
                break;
        
        INTCONbits.RBIF = 0;  // Clear the interrupt flag for Port B
    }
    }}

int main() {
    
    OSCCON = 0x72;      // oscillator set to 16MHz

    ANSELA = 0;
    ANSELB = 0;         // Ports set all digital
    ANSELC = 0;

    INTCONbits.INT0E = 1;     // enable interrupts
    INTCON2bits.INTEDG0 = 0;  // on falling edge
    INTCONbits.INT0F = 0;     // interrupt flag set off for now
    INTCONbits.GIE = 1;       // globally enable all interrupts
    INTCONbits.RBIE = 1;      //enable PORTB interrupts
    
    TRISB = 0xF0;  // set interrupt-on-change or row bits to inputs
    PORTB = 0xF0;  // leaving columns (lower nibble) as outputs

    
    initializeLCD();
    
    while (1) {
        
        displayKey(char key);
        
    }

    return 0;
}