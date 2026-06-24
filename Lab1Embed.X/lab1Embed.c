#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define _XTAL_FREQ 16000000
#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2
#define C4 PORTBbits.RB3
#define R1 PORTBbits.RB4
#define R2 PORTBbits.RB5
#define R3 PORTBbits.RB6
#define R4 PORTBbits.RB7

#define SRCLR PORTCbits.RC2
#define SRE PORTCbits.RC1

//Scans for a key press
/*
char keyScan(void)  
{           
            //Checking first column for key press
            C1 = 0; 
            C2 = 1; 
            C3 = 1; 
            C4 = 1;    
            if (!R1)
            {   
                __delay_ms(100);
                while (!R1);
                return '1'; 
            }
            if (!R2) 
            { 
                __delay_ms(100); 
                while (!R2); 
                return '2'; 
            }
            if (!R3) 
            { 
                __delay_ms(100); 
                while (!R3); 
                return '3'; 
            }
            if (!R4) 
            { 
                __delay_ms(100); 
                while (!R4); 
                return 'A'; 
            }
            //Checking second column for key press
            C1 = 1; 
            C2 = 0; 
            C3 = 1; 
            C4 = 1;    
            if (!R1) 
            { 
                __delay_ms(100); 
                while (!R1); 
                return '4'; 
            }
            if (!R2) 
            { 
                __delay_ms(100); 
                while (!R2); 
                return '5'; 
            }
            if (!R3) 
            { 
                __delay_ms(100); 
                while (!R3); 
                return '6'; 
            }
            if (!R4) 
            { 
                __delay_ms(100); 
                while (!R4); 
                return 'B'; 
            }
            //Checking the third column for key press
            C1 = 1; 
            C2 = 1; 
            C3 = 0; 
            C4 = 1;    
            if (!R1) 
            { 
                __delay_ms(100); 
                while (!R1); 
                return '7'; 
            }
            if (!R2) 
            { 
                __delay_ms(100); 
                while (!R3); 
                return '8'; 
            }
            if (!R3) 
            { 
                __delay_ms(100); 
                while (!R3); 
                return '9'; 
            }
            if (!R4) 
            { 
                __delay_ms(100); 
                while (!R4); 
                return 'C'; 
            }
           //Checking the fourth column for key press
            C1 = 1; 
            C2 = 1;
            C3 = 1; 
            C4 = 0;    
            if (!R1) 
            { 
                __delay_ms(100); 
                while (!R1); 
                return '*'; 
            }
            if (!R2) 
            { 
                __delay_ms(100); 
                while (!R2); 
                return '0'; 
            }
            if (!R3) 
            { 
                __delay_ms(100); 
                while (!R3); 
                return '#'; 
            }
            if (!R4) 
            { 
                __delay_ms(100); 
                while (!R4); 
                return 'D'; 
            }
            
    return 'n';                   
}
*/
void pushData(int keyPressed)
{
    //If char is a digit push to buffer
    SSP2BUF = keyPressed;
    while(!PIR3bits.SSP2IF);
    PIR3bits.SSP2IF = 0;
    //Else if char if special do function
}

int keyPressed = 0110;

void main(void) 
{
    OSCCON = 0x72; //set clock to 16 MHz
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    TRISA = 0xCF;
    TRISB = 0xF0;
    TRISC = 0x06;
    
    SSP1CON1 = 0x2A;
    SSP1STAT = 0x40;
    SSP1ADD = 0xCF; //207
    
    SRCLR = 1; //Clear for the Shift Register
    SRE = 1; //Enable for the Shift Register
    
    INTCONbits.GIE = 1; //Enable Global Interrupts
    INTCONbits.PEIE = 1; //Enable Peripheral Interrupts
    INTCONbits.RBIE = 1;//Interrupt Port B On Change
    PIE3bits.SSP2IE = 1;
    
    SSP2BUF = keyPressed;
    while(!PIR3bits.SSP2IF);
    PIR3bits.SSP2IF = 0;
    
    return;
}
/*
void __interrupt keyPress(void)
{
    if(INTCONbits.RBIF)
    {
        pushData(keyScan());
    }
    INTCONbits.RBIF = 0; //Reset interrupt flag
}
 * */