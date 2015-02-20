/*
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
#include "temphandler.h"
#include "ATM1602B.h"
#include "DS18B20.h"
#include <avr/io.h>

// Initializes LCD
void startLCD(void){
    
    // Initialize LCD
    initLCD();
    executeCommand(SYSTEM_SET_8BITS_1LINE_10DOTS);
    executeCommand(ENTRY_MODE_SET_INC_NOSHIFT);
    executeCommand(DISPLAY_ON_NO_CURSOR_NO_BLINK);
    executeCommand(CLEAR_DISPLAY);
    executeCommand(CURSOR_HOME);
    
}

// Prints the current temperature from the sensor
void printCelsiusTemperature(void){
    
    // Wait for sensor to finish ...
    while (readScratchPad() != 1) {}
    
    // Read temperaturedata
    unsigned int temp = getTemperatureRegisterData();
    
    // Store each part of temperature in separate variables
    unsigned int signBits = (temp & 0xF800) >> 11;
    
    unsigned int integer  = (temp & 0x7F0) >> 4;
    unsigned int hundreds  = integer / 100;
    unsigned int tens      = (integer % 100) / 10;
    unsigned int ones      = (integer % 100) % 10;
    
    
    unsigned int fraction = (temp & 0xF);
    fraction = fraction * 625;
    unsigned int tenths             =  fraction/1000;
    unsigned int centesimals        = (fraction/100) % 10;
    unsigned int millesimals        = (fraction/10) % 10;
    unsigned int tensmillesimals    = (fraction % 100) % 10;
    
    
    // Signbits > 0 => the temperature is negative
    if (signBits){
        
        // If negative temperature the bits should be inverted
        // and added one.
        unsigned int tempBits = (temp & 0x7FF);
        tempBits = ~tempBits;
        tempBits = tempBits + 1;
        
        integer = (tempBits & 0x7F0) >> 4;
        fraction = (tempBits & 0xF);

        writeData('-');
        
    }
    
    // Position next character at position 0x41
    position(0x1);
    
    // If CRC-test pass print temperature
    if (verifyCrc()) {
        
        writeData('T');
        writeData('e');
        writeData('m');
        writeData('p');
        writeData(':');
        
 
        // -----------Integer part------------

        
        if (hundreds){
            writeInteger(hundreds);
            writeInteger(tens);
        }
        else{
            if(tens){
                writeInteger(tens);
            }            
        }
        
        writeInteger(ones);
        
        // .
        writeData(0x2E);
        
        
        // -----------Fraction part------------
       
        writeInteger(tenths);
        writeInteger(centesimals);
        writeInteger(millesimals);
        writeInteger(tensmillesimals);
        
        writeData(0xDF);
        writeData('C');
        
    }
    else{
        
        executeCommand(CLEAR_DISPLAY);
        writeData('C');
        writeData('R');
        writeData('C');
        writeData('-');
        writeData('e');
        writeData('r');
        writeData('r');
        writeData('o');
        writeData('r');
        
    }
    
    
}
