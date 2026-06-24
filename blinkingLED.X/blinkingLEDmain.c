
#include <stdlib.h>
#include <xc.h>

#pragma config FOSC = INTIO67   // Oscillator Selection (Internal oscillator block, port function on RA6 and RA7)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config PWRTEN = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#define _XTAL_FREQ 16000000 


void main(void) {
    
    TRISC = 0x00;

    while (1) {
        int i = 0;
        
        while (i <= 1000){
            
        PORTC = 0xFF;
        
        __delay_ms(100);
        
        
        PORTC = 0x00;
        
        __delay_ms(100);
        
        i ++;
        }}
    
 return;
}
