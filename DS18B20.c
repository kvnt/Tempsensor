/*
 * Code to control DS18B20 temperaturesensor.
 * http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
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
#include "DS18B20.h"

// -------------Miscellaneous macros------------------
#define PULL_BUS_LOW    (DDRF = DDRF | 0x1)
#define RELEASE_BUS     (DDRF = DDRF & 0xFE)

#define BUS_IS_HIGH     ((PINF & 0x1) == 0x1)
#define BUS_IS_LOW      ((PINF & 0x1) == 0x0)

#define READ_BUS        (PINF & 0x1)

// ROM-commands
#define SKIP_ROM        issueCommand(0xCC)

// Function-commands
#define CONVERT_T       {issueCommand(0x44); while (readTimeSlot() == 0){}}
#define READ_SCRATCH    issueCommand(0xBE)


/* -------------Scratchpad --------------
 Contains all data in scratchpad-register (DS18B20)
 */

uint8_t scratch[9];


/*  INITIALIZATION PROCEDURE—RESET AND PRESENCE PULSES
 --------------------------------------------------
 'All communication with the DS18B20 begins with an initialization
 sequence that consists of a reset pulse from the master followed
 by a presence pulse from the DS18B20.'
 */

void initSequence(void){
    
    // PORTF0 always zero.
    PORTF &= 0xFE;
    
    // ---- MASTER TX RESET PULSE ---- //
    
    PULL_BUS_LOW;
    
    _delay_us(480);
    
    RELEASE_BUS;
    
    // ---- MASTER RX (SENSOR PRESENCE PULSE) ---- //
    
    _delay_us(15);
    
    while (BUS_IS_HIGH) {}
    
    _delay_us(60);
    
    while (BUS_IS_LOW) {}
    
    _delay_us(405);
    
}



/*  WRITE TIME SLOT
 -----------------------------
 void writeTimeSlot(char bit) takes zero or one as input and writes
 it to the sensor.
 
 */

void writeTimeSlot(uint8_t bit){
    
    switch (bit) {
            
        case 0:
            
            /* Pull bus low for
               60 microseconds */
            
            PULL_BUS_LOW;
            
            _delay_us(60);
            
            RELEASE_BUS;
            
            break;
            
        case 1:
            
            /* Pull bus low for
             10 microseconds */
            
            PULL_BUS_LOW;;
            
            _delay_us(10);
            
            RELEASE_BUS;
            
            _delay_us(50);
            
            break;
            
    }
    
    _delay_us(2);
    
}

/* READ TIME SLOTS
 -------------------------------
 unsigned char readTimeSlot() returns the read bit.
 
 */

uint8_t readTimeSlot(void){
    
    uint8_t readBit = 0;
    
    // --- Master initiates the read time slot --
    
    PULL_BUS_LOW;
    
    _delay_us(2);
    
    RELEASE_BUS;
    
    _delay_us(10);
    
    //Sample bus
    readBit = READ_BUS;
    
    _delay_us(48);
    
    // Wait for sensor to finish
    while (BUS_IS_LOW) {}
    
    _delay_us(2);
    
    return readBit;
    
}


/* Takes a function or rom command as input
   and transmits it to the sensor */

void issueCommand(uint8_t command){
    
    uint8_t mask = 0x1;
    
    for (uint8_t i = 0; i < 8; i++){
        
        writeTimeSlot((mask & command) >> i);
        
        mask <<= 1;
    }
    
}

/* ------ Read Scratchpad ------
   Issues a tempconversion and reads the whole
   scratchpad structure. */

uint16_t readScratchPad(void){
    
    /* Send presence pulse */
    initSequence();
    
    /* Send skip ROM-command */
    SKIP_ROM;
    
    /* Issue a tempconversion */
    CONVERT_T;
    
    /* Send presence pulse */
    initSequence();
    
    /* Send skip ROM-command */
    SKIP_ROM;
    
    /* Issue a read scratch command */
    READ_SCRATCH;
    
    /* Reads all bytes from scratchpad 
       and stores the result in scratch
       array */
    for (uint8_t n = 0; n < 9; ++n)
    {
        
        uint8_t s   = 0;
        scratch[n]  = 0;
        
        for (uint8_t i = 0; i < 8; i++) {
            
            if(readTimeSlot()){
                
                s |= (1 << i);
                
            }
            
        }
        
        scratch[n] = s;
    }
    
    /* Return 1 when finished */
    return 1;
}

/* ------ Verify CRC ------
   Takes the whole scratchpad and shifts it into
   the shiftregister. Finally the CRC is shifted
   in and if the result is zero the verification has
   passed and hopefully the data is error-free. */

uint8_t verifyCrc(void){
    
    register uint8_t shiftregister = 0,
                            output = 0,
                            input  = 0;
    
    /* Iterate through all 9 bytes in scratchpad */
    for (uint8_t n = 0; n < 9; ++n)
    {
        
        /* Store current byte from scratchpad in
           temporary variable input. */
        input = scratch[n];
        
        
        /* Go through every bit in current byte */
        for (uint8_t c = 0; c < 8; c++) {
            
            /* Take the current bit and do XOR with least
               significant bit in shiftregister */
            output = (!(input & (1 << c)) != !(0x1 & shiftregister));
            
            /* Shift the three least significant bits one step to the left */
            shiftregister = (((shiftregister & 0x7) >> 1) |
                                (shiftregister & 0xF8));
            
            /* Do XOR between third bit in shiftregister and output and
               store the result in bit 2. */
            shiftregister = ((!(shiftregister & 0x8) != !output) << 2) |
                                (shiftregister & 0xFB);
            
            /* Do XOR between fourth bit in shiftregister and output and
               store result in bit 3. */
            shiftregister = ((!(shiftregister & 0x10) != !output) << 3) |
                                (shiftregister & 0xF7);
            
            /* Shift the four most significant bits one step to the right. */
            shiftregister = (((shiftregister & 0xF0) >> 1) & 0xF0) |
                                (shiftregister & 0x0F);
            
            /* Store the result of output in most significant bit. */
            shiftregister = (output << 7) | (shiftregister & 0x7F);
            
        }
        
    }
    
    /* If shiftregister now contains all zeroes
       the data should be error-free */
    return shiftregister == 0;
}


/* ------ Return temperature data ------
   Returns the tempdata in scratchpad */

uint16_t getTemperatureRegisterData(void){
    
    /* Take the eight most significant bits
       in tempregister and store them in temp. */
    uint16_t temp = scratch[1];
    temp <<= 8;
    
    /* Add the eight last significant bits */
    temp |= scratch[0];
    
    return temp;
    
}

