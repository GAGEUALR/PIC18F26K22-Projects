#pragma config FOSC = INTIO67   // Oscillator Selection (Internal oscillator block, port function on RA6 and RA7)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config PWRTEN = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


#include <xc.h>
#include <stdlib.h>
#define _XTAL_FREQ 16000000 

void configurePIC()
{
    TRISA = 0xEE; //set PORTA as input,RA2 as DACOUT            //change
    TRISB = 0xFF; //set PORTB as input
    TRISC = 0xFF; //set PORTC as input
    
    PORTA = 0x00;
    PORTB = 0x00; //initialize ports as 0, no output yet
    PORTC = 0x00;
    
    ANSELC = 0;
    ANSELB = 0; //all digital
    ANSELA = 0;
    
    OSCCON = 0x72; //16MHZ                      -- important
    
    INTCONbits.PEIE = 1; //enable peripheral interrupts
    INTCONbits.GIE = 1; //enable global interrupts
    
    VREFCON1bits.DACEN = 1;     //enable DAC
    VREFCON1bits.DACOE = 1;
    
}

void configureReceive()
{
       
    SPBRG2 = 51; // Set SPBRG value for 19200 baud rate
    TXSTA2bits.BRGH2 = 1; // High speed mode
    BAUDCON2bits.BRG16 = 0; // 8-bit mode
    
    RCSTA2bits.SPEN = 1; // Enables EUSART
    TXSTA2bits.SYNC = 0; // Asynchronous operation
    RCSTA2bits.CREN = 1; // Enables receiver circuit
    
    PIE3bits.RC2IE = 1; // Enable EUSART receive interrupt
}

void configureTimer0() 
{
    T0CON = 0x84; //16-bit mode, prescaler 1:32, TMR0ON = 1
}

unsigned int calculateDelay(float frequency)
{
    // Calculate the period of the waveform (in seconds)
    float T = 1.0 / frequency;

    // Calculate the time per DAC step
    float timePerVoltStep = T / 32.0;

    // 4 is number of cycles for Timer0 overflow,
    //1 cycle per prescaler count
    //multiply by 32.0, the prescaler
    float timePerTimer0Step = (4.0 / _XTAL_FREQ) * 32.0;

    // Calculate the total time for Timer0 to complete all steps
    unsigned int totalTimer0Time = (unsigned int)(timePerVoltStep / timePerTimer0Step);

    // Calculate the delay time for Timer0
    unsigned int delayTime = (65536 - totalTimer0Time);

    return delayTime;
}

void generateWaveform(unsigned int frequency) {
    
    unsigned int timerDelay = calculateDelay((float)frequency);
    
    int dacValue = 0x00;

    while (!PIR3bits.RC2IF)
    {
        VREFCON2 = dacValue; //set DAC voltage. 
        
        TMR0 = 0; //clear Timer0
        
        T0CONbits.TMR0ON = 1; //turn on Timer0
        
        while (!TMR0IF); //wait until Timer0 overflows
        TMR0IF = 0; //clear Timer0 overflow flag
        
        T0CONbits.TMR0ON = 0; //turn off Timer0
        
        dacValue++; //increment DAC value

        if (dacValue > 31)
        {
            dacValue = 0; //reset DAC value
        }
    }
}

char receiveBuffer[6] = {'\0'};
unsigned char receiveIndex = 0;
unsigned int frequency;

void main(void) {
    configurePIC();
    configureReceive();
    configureTimer0();

    PORTAbits.RA0 = 0;  
   
    while (1) {
        __delay_ms(1000);  // Delay to allow all data to arrive and to simulate processing time
        
        if (receiveIndex == 5) {  // Check if 5 digits have been received
            frequency = atoi(receiveBuffer);  // Convert the buffer to an integer
            PORTAbits.RA0 = 1;  // Signal that a command has been processed
            
            for (int i = 0; i < sizeof(receiveBuffer); i++) {
                receiveBuffer[i] = '\0';  // Clear the buffer after processing
            }
            receiveIndex = 0;  // Reset receive buffer index

            PORTAbits.RA0 = 0;  // Turn off signal
            __delay_ms(10);  // Small delay to visually distinguish the signal
        }
    }
}

void __interrupt() isr(void) {
    if (PIR3bits.RC2IF) {
        char receivedByte = RCREG2;  // Read the received byte from the UART data register
        PIR3bits.RC2IF = 0;  // Clear the UART receive interrupt flag
        
        // Prevent buffer overflow by checking space for the null terminator too
        if (receiveIndex < sizeof(receiveBuffer) - 2) {
            receiveBuffer[receiveIndex++] = receivedByte;
            receiveBuffer[receiveIndex] = '\0';  // Null-terminate after each char just in case
        }
    }
}