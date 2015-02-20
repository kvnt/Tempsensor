/*
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf)
 *
 The MIT License (MIT)
 
 Copyright (c) 2015 Robert Kvant http://www.github.com/kvnt
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ATM1602B.h"

#define DATAPORT            PORTB
#define ENABLE_E            PORTD   |= 0x4
#define DISABLE_E           PORTD   &= 0xFB
#define BUSY_FLAG_HIGH      (PINB   &  0x80) == 0x80
#define BUSY_FLAG_LOW       (PINB   &  0x80) == 0x00
#define DB7_INPUT           DDRB    &= 0x7F
#define DB7_OUTPUT          DDRB    |= 0x80
#define RW_ONE              PORTD   |= 0x2
#define RW_ZERO             PORTD   &= 0xFD
#define RS_ONE              PORTD   |= 0x1
#define RS_ZERO             PORTD   &= 0xFE


/* Display commands */
#define SYSTEM_SET_8BITS_2LINES_10DOTS          0x3C
#define SYSTEM_SET_8BITS_1LINE_10DOTS           0x34
#define SYSTEM_SET_4BITS_2LINES_10DOTS          0x2C
#define SYSTEM_SET_4BITS_1LINE_10DOTS           0x24
#define SYSTEM_SET_8BITS_2LINES_7DOTS           0x38
#define SYSTEM_SET_8BITS_1LINE_7DOTS            0x30
#define SYSTEM_SET_4BITS_2LINES_7DOTS           0x28
#define SYSTEM_SET_4BITS_1LINE_7DOTS            0x20

#define ENTRY_MODE_SET_INC_SHIFT                0x7
#define ENTRY_MODE_SET_INC_NOSHIFT              0x6
#define ENTRY_MODE_SET_DEC_SHIFT                0x5
#define ENTRY_MODE_SET_DEC_NOSHIFT              0x4

#define CLEAR_DISPLAY                           0x1

#define DISPLAY_ON_NO_CURSOR_NO_BLINK           0xC
#define DISPLAY_ON_NO_CURSOR_BLINK              0xD
#define DISPLAY_ON_CURSOR_NO_BLINK              0xE
#define DISPLAY_ON_CURSOR_BLINK                 0xF

#define CURSOR_SHIFT_ONE_RIGHT                  0x1C
#define CURSOR_SHIFT_ONE_LEFT                   0x18

#define SET_DDRAM_ADDRESS                       0x80
#define CURSOR_HOME                             0x2

#define WRITE_DATA                              { RS_ONE; RW_ZERO; }
#define READ_DATA                               { RS_ONE; RW_ONE;  }



/* Initialize display */
void initLCD(void){
    
    /* Set all pins as outputs */
    DDRD |= 0x7;
    DDRB |= 0xFF;
    
}

// Send E signal
void sendEnable(void){
    
    /* Wait 200 nanoseconds */
    _delay_us(0.2);
    
    /* Enable E */
    ENABLE_E;
    
    /* Wait 400 nanoseconds */
    _delay_us(0.4);
    
    /* Disable E */
    DISABLE_E;
    
}

/* Reads the current status of the
   busy-flag. */
void readBusyFlag(void){
    
    /* Set pin B7 as input (LOW) */
    DB7_INPUT;
    
    /* Set RS = 0, R/W = 1 */
    RS_ZERO; RW_ONE;
    
    /* Wait 200 nanoseconds */
    _delay_us(0.2);
    
    /* Enable E */
    ENABLE_E;
    
    /* Wait 400 nanoseconds */
    _delay_us(0.4);
    
    /*  Wait until LCD is ready
        (busy-flag == 0)*/
    while (BUSY_FLAG_HIGH){}
    
    /* Disable E */
    DISABLE_E;
    
    /* Wait 200 nanoseconds */
    _delay_us(0.2);
    
    /* Set pin B7 as output (HIGH) */
    DB7_OUTPUT;
    
}

/* Takes data of type uint8_t as 
   input and writes it to the LCD. */
void writeData(uint8_t data){
    
    /* Set RS = 1, R/W = 0 */
    WRITE_DATA;
    
    /* Assign character to dataport */
    DATAPORT = data;
    
    /* Send enable and wait until
     LCD has finished */
    sendEnable();
    readBusyFlag();
    
}

/* Takes one of the specified command
   macros as input and sends it to the
   LCD. */
void executeCommand(uint8_t command){
    
    /* Set both pin RS and RW to 0 */
    RS_ZERO; RW_ZERO;
    
    /* Assign command to the dataport */
    DATAPORT = command;
    
    /* Send enable and wait until 
       LCD has finished */
    sendEnable();
    readBusyFlag();
    
}


/* Takes the address as input and sends it
   to the LCD */
void position(uint8_t address){
    
    /* Concatenate the adress with 
       'Set DD RAM Address' command and 
       send it to the executeCommand function. */
    executeCommand(address | SET_DDRAM_ADDRESS);
    
}





