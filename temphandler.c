/* 
  -- Code to handle interconnection between LCD and Temperaturesensor --
 
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

/* Initializes LCD */
void startLCD(void){

    initLCD();
    executeCommand(SYSTEM_SET_8BITS_1LINE_10DOTS);
    executeCommand(ENTRY_MODE_SET_INC_NOSHIFT);
    executeCommand(DISPLAY_ON_NO_CURSOR_NO_BLINK);
    executeCommand(CLEAR_DISPLAY);
    executeCommand(CURSOR_HOME);
    
}


/* Calculates every part of the temperaturedata */
void calculateTemperatureVariables(uint16_t *temp,
                                   uint16_t *hundreds,
                                   uint16_t *tens,
                                   uint16_t *ones,
                                   uint16_t *tenths,
                                   uint16_t *centesimals,
                                   uint16_t *millesimals,
                                   uint16_t *tensmillesimals){
    
    
    /* Store each part of temperature in separate variables */
    uint16_t signBits = (*temp & 0xF800) >> 11,
             integer  = (*temp & 0x7F0) >> 4,
             fraction = (*temp & 0xF) * 625;
    
    
    /* Signbits > 0 => the temperature is negative */
    if (signBits){
        
        /* If negative temperature the bits should be inverted
           and added one. */
        uint16_t tempBits = (~(*temp & 0x7FF)) + 1;
        
        integer = (tempBits & 0x7F0) >> 4;
        fraction = (tempBits & 0xF);
        
    }
    
    *hundreds            =   (integer / 100);
    *tens                =   (integer % 100) / 10;
    *ones                =   (integer % 100) % 10;
    *tenths              =   (fraction/1000);
    *centesimals         =   (fraction/100) % 10;
    *millesimals         =   (fraction/10) % 10;
    *tensmillesimals     =   (fraction % 100) % 10;
    
}



/* Prints the current temperature from the sensor */
void printCelsiusTemperature(void){
    
    /* Wait for sensor to finish ... */
    while (readScratchPad() != 1) {}
    
    /* Position next character at position 0x41 */
    position(0x1);
    
    /* If CRC-test pass print temperature */
    if (verifyCrc()) {
        
        /* Read temperaturedata and create variables
           to hold all parts of temperature. */
        uint16_t temp = getTemperatureRegisterData(),
                        hundreds,tens,ones,tenths,centesimals,
                                    millesimals,tensmillesimals;
        
        uint16_t signBits = (temp & 0xF800) >> 11;
        
        /* Call calculateTemperatureVariables() to calculate 
           every part of the temperaturedata*/
        calculateTemperatureVariables((uint16_t *) &temp,
                                      (uint16_t *) &hundreds,
                                      (uint16_t *) &tens,
                                      (uint16_t *) &ones,
                                      (uint16_t *) &tenths,
                                      (uint16_t *) &centesimals,
                                      (uint16_t *) &millesimals,
                                      (uint16_t *) &tensmillesimals);
        
        
        
        
        // ---------------PRINTING--------------------
        
        uint8_t tempCharacters[] = "Temp:";
        
        for (int i = 0; i < sizeof(tempCharacters) - 1; i++) {
            writeData(tempCharacters[i]);
        }
 
        if(signBits){
            writeData('-');
        }
        
        // ---------------Integer part----------------
        
        if (hundreds){
            writeDigit(hundreds);
            writeDigit(tens);
        }
        else{
            if(tens){
                writeDigit(tens);
            }            
        }
        
        // ---------------End Integer part-------------
        
        writeDigit(ones);
        writeData('.');
        
        // ---------------Fraction part----------------
       
        writeDigit(tenths);
        writeDigit(centesimals);
        writeDigit(millesimals);
        writeDigit(tensmillesimals);
        
        writeData(0xDF);
        writeData('C');
        
        // ---------------End Fraction part-------------
        
    }
    else{
        
        executeCommand(CLEAR_DISPLAY);
        
        /* The CRC is not valid print error message */
        
        uint8_t tempError[] = "CRC-Error";
        
        for (int i = 0; i < sizeof(tempError) - 1; i++) {
            writeData(tempError[i]);
        }
        
    }
    
    
}
