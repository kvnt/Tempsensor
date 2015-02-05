/* 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Copyright 2015 Robert Kvant
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf)
 *	----------------------------------
 * 	|	AVR  	| 	LCD	 |	
 * 	----------------------------------
 * 	| D0 		:	RS       |
 * 	| D1 		: 	R/W	 |
 * 	| D2 		: 	E	 |
 *	|				 |
 * 	| B0 - B7 	:	D0 - D7  |	
 * 	----------------------------------
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include "ATM1602B.h"

#define ENABLE_E	PORTD = PORTD | 0b00000100
#define DISABLE_E	PORTD = PORTD & 0b11111011


// Initialize display
void initLCD(void){

	// Set as outputs
	DDRD = DDRD | 0b00000111;
	DDRB = DDRB | 0b11111111;

    systemSet();
    
	// Clear display
	clearDisplay();

    entryModeSet();
    
    systemSet();
    

}

void systemSet(void){
    
    // System set
    PORTD = PORTD & 0b11111100;
    PORTB = PORTB & 0b00111100;
    PORTB = PORTB | 0b00111100;
    
    sendEnable();
    readBusyFlag();
    
}

void entryModeSet(void){
    
    // Entry Mode Set
    PORTD = PORTD & 0b11111100;
    PORTB = PORTB & 0b00000110;
    PORTB = PORTB | 0b00000110;
    
    sendEnable();
    readBusyFlag();
    
}

// Clear display
void clearDisplay(void){

	// Clear display
	PORTD = PORTD & 0b11111100;
	PORTB = PORTB & 0b00000001;
	PORTB = PORTB | 0b00000001;
	
	sendEnable();
	readBusyFlag();

}

// Display on
void displayOn(void){

	PORTD = PORTD & 0b11111100;
	PORTB = PORTB & 0b00001100;
	PORTB = PORTB | 0b00001100;
	
	sendEnable();
	readBusyFlag();		
	
}

// Send E signal
void sendEnable(void){	
	
	_delay_us(0.2);

	// Enable E
	ENABLE_E;
	
	_delay_us(0.4);	

	// Disable E
	DISABLE_E;
	
}

// Check busy flag
void readBusyFlag(void){

	// Set pin B7 as input (LOW)
	DDRB = DDRB & 0b01111111;

	// Set RS = 0 och R/W = 1 (Read Busy Flag)
	PORTD = PORTD & 0b11111110;
	PORTD = PORTD | 0b00000010;

	_delay_us(0.2);

	// Enable E
	ENABLE_E;
	
	_delay_us(0.4);	

	// Wait until BF=0
	while ((PINB & 0x80) == 0x80){}

	// Disable E
	DISABLE_E;

	_delay_us(0.2);

	// Set pin B7 as output (HIGH)
	DDRB = DDRB | 0x80;

}

// Write a single character
void writeCharacter(unsigned char character){
    
    // Set RS = 1, R/W = 0
    PORTD = PORTD & 0b11111101;
    PORTD = PORTD | 0b00000001;
    
    // Assign character to port B
    PORTB = character;
    
    sendEnable();
    readBusyFlag();
    
}


void printCharAsBinary(unsigned char bin){
    
    unsigned char mask = 0b10000000;
    
    for (int i = 0; i < 8; i++) {
        
        if(mask & bin){
            writeCharacter('1');
        }
        else{
            writeCharacter('0');
        }
        
        mask >>= 1;
        
    }
    
}

void printIntAsBinary(unsigned int bin){
    
    unsigned int mask = 0b1000000000000000;
    
    for (int i = 0; i < 16; i++) {
        
        if(mask & bin){
            writeCharacter('1');
        }
        else{
            writeCharacter('0');
        }
        
        mask >>= 1;
        
    }
    
}


void position(unsigned char address){
    
    PORTD = PORTD & 0b11111100;
    PORTB &= 0b10000000;
    PORTB |= 0b10000000;
    PORTB |= address;
    sendEnable();
    readBusyFlag();
    
}





