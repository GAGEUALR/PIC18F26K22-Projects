/*
 * File:   main.c
 * Author: Gage Nieffenegger
 *
 * Created on September 10, 2023, 11:27 AM
 */


#include <xc.h>

void main(void) {
 
    unsigned int i = 0; //create a variable to loop in the for loops below
    
    TRISA = 0xFE;   //sets least significant bit as an output, all else inputs
    
    while (1){  //will never exit this loop
        
        PORTA = 0x01;   //turns LSB output on
        
        for (i=0; i<50000; i++); //wastes time looping through, 
                                 //otherwise LED would flash too quick to see
        
        PORTA = 0x00;   //turns LSB output off
        
        for (i=0; i<50000; i++);  //wastes time before looping again
    }
    
    return;
}
