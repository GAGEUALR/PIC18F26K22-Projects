#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config PWRTEN = ON

#define _XTAL_FREQ 16000000
#define LCDdata PORTC
#define RW PORTBbits.RB2
#define RS PORTBbits.RB1
#define E PORTBbits.RB3

unsigned int result;
//internal clock delay that toggles enable bit between characters

char freqChar[] = "";

void clockE() {

    E = 1;
    __delay_ms(1);
    E = 0;          //get microcontroller ready 
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

void ADC()
{
    result = 0;
    ADCON0bits.GO = 1; //Start conversion
    while(ADCON0bits.DONE); //wait for conversion to finish
    result = ADRESH;
}

void printDataToLCD(char *freqChar){
    
    RS = 0;
    LCDdata = 0x01;
    clockE();
    
    __delay_ms(2);
    
    int i;
    
    RS = 1;
        for(i = 0; (freqChar[i] != '\0'); i++ )
        {
            LCDdata = freqChar[i];
            clockE();
        }
    
    __delay_ms(500);
}


void timerSelect(unsigned int ADCresult)
{
    //100Hz
    if(ADCresult <= 64)
    {
        //CCPR4H:CCPR4L = 10,000 for 100 Hz signal
        CCPR4H = 0x4E;
        CCPR4L = 0x20;
        //Print frequency to LCD
        
        
        
        printDataToLCD("F = 100 HZ");
    }
    //200Hz
    if (ADCresult <= 127 && ADCresult > 64)
    {
        //CCPR4H:CCPR4L = 5,000 for 200 Hz signal
        CCPR4H = 0x27;
        CCPR4L = 0x10;
        //Print frequency to LCD
        
        printDataToLCD("F = 200 HZ");
        
        clockE();
    }
    //1kHz
     if (ADCresult <= 191 && ADCresult > 127)
    {
        //CCPR4H:CCPR4L = 2,000 for 1 kHz signal
        CCPR4H = 0x07;
        CCPR4L = 0xD0;
        //Print frequency to LCD
        
        printDataToLCD("F = 1000 HZ");
    }
    //10kHZ
     if 
            (ADCresult <= 255 && ADCresult > 191)
    {
        //CCPR4H:CCPR4L = 200 for 10 kHz signal
        CCPR4H = 0x00;
        CCPR4L = 0xC8;
        //Print frequency to LCD
   
        
        
        printDataToLCD("F = 10000 HZ");
    }
    
}

void main(void) {
    
    OSCCON = 0x72;
    TRISA = 0xFF;
    TRISB = 0xF0;
    TRISC = 0x00;
    
    PORTB = 0;
    
    LCDINIT();
    
    ADCON2 = 0x2D; //Left Justified Data FOSC/16 12TAD
    ADCON1 = 0x00; 
    ADCON0 = 0x00;
    
    ADCON0bits.ADON = 1;
    ANSELAbits.ANSA0 = 1;
    
    ANSELB = 0;
    ANSELC = 0;
    
    TRISBbits.RB0 = 0;
    PORTBbits.RB0 = 0;
    
    CCPTMRS1 = 0x00;
    CCP4CON = 0x0A;
    
    /*PIE1bits.ADIE = 1; // Enable ADC interrupts
    PIR1bits.ADIF = 0; //ADC Flag to zero
     */
    INTCONbits.PEIE = 1; //Enable Peripheral Interrupts
    INTCONbits.GIE = 1; //Enable Global Interrupts
    IPR1bits.ADIP = 0; //ADC low-priority interrupt
    IPR4bits.CCP4IP = 1; // Make the CCP4 interrupt high priority
    PIR4bits.CCP4IF = 0; //Clear the CCP4 flag
    PIE4bits.CCP4IE = 1; //Enable CCP4 Interrupt
    
    T1CON = 0x02; //1:1 Prescaler FOSC/4 clock
    //Initialize Timer 1 at 0
    TMR1L = 0;
    TMR1H = 0;
    T1CONbits.TMR1ON = 1; //Turn on the timer
    while(1)
    {
        ADC();
        timerSelect(result);
    }
    return;
}

void __interrupt compare(void)
{
    if(PIR4bits.CCP4IF == 1)
    {
        PORTBbits.RB0 = ~PORTBbits.RB0;
        TMR1L = 0;
        TMR1H = 0;
        PIR4bits.CCP4IF = 0;
    }
    /*if(PIR1bits.ADIF == 1)
    {
        result = 0;
        result = ADRESH;
        PIR1bits.ADIF = 0;
    }
     */
}
