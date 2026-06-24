/* 
 * File:   
 * Author: 
 * Comments:
 */


 
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

/*
 The code in the main file is used to flash an LED, and to do this first I 
 created a variable i. After, I set the I/O pins to the required configuration,
 turning the LSB to an output and leaving the rest as inputs to reduce the 
 chances of a malfunction. Then, I created an infinite loop. (the watchdog timer
 resets the code if it runs too long) In the loop, I turn the output pin to high,
 and after create a long for loop to let the LED shine bright long enough to see.
 When the loop is finished, I turn the LSB output to low, and use the same loop
 so that the LED is off long enough to see. The program goes back to the top of 
 the infinite loop to repeat the flashing process.
 */