/*
 *
 * Created by Robert Kvant 2015 http://www.github.com/kvnt
 *
 */

#include <stdint.h>
#include "temphandler.h"
#include "ATM1602B.h"
#include "DS18B20.h"
#include <avr/io.h>

/* Starts up LCD */
void startLCD(void){
    
    // Initialize LCD
    initLCD();
    executeCommand(SYSTEM_SET_8BITS_2LINES_10DOTS);
    executeCommand(ENTRY_MODE_SET_INC_NOSHIFT);
    executeCommand(DISPLAY_ON_NO_CURSOR_NO_BLINK);
    executeCommand(CLEAR_DISPLAY);
    executeCommand(SYSTEM_SET_8BITS_2LINES_10DOTS);
    
}

/* Print current temperature from sensor */
void printCelsiusTemperature(void){
    
    // Wait for sensor to finish ...
    while (readScratchPad() != 1) {}
    
    // Read temperaturedata
    unsigned int temp = getTemperatureRegisterData();
    
    // Position next characters at position 0x41
    position(0x41);
    
    // If CRC-test pass print temperature
    if (verifyCrc()) {
        
        writeCharacter('T');
        writeCharacter('e');
        writeCharacter('m');
        writeCharacter('p');
        writeCharacter(':');
        
        // Store each part of temperature in separate variables
        unsigned int signBits = (temp & 0xF800) >> 11;
        unsigned int integer  = (temp & 0x7F0) >> 4;
        unsigned int fraction = (temp & 0xF);
        
        // Signbits > 0 the temperature is negative
        if (signBits){
            
            // If negative temperature the bits should be inverted
            // and added one.
            unsigned int tempBits = (temp & 0x7FF);
            tempBits = ~tempBits;
            tempBits = tempBits + 1;
            
            integer = (tempBits & 0x7F0) >> 4;
            fraction = (tempBits & 0xF);
            
            // -
            writeCharacter('-');
            
        }
        
        // -----------Integer part------------
        unsigned int hundreds  = integer / 100;
        unsigned int tens      = (integer % 100) / 10;
        unsigned int ones      = (integer % 100) % 10;
        
        if (hundreds){
            
            writeCharacter(hundreds | 0x30);
            writeCharacter(tens | 0x30);
            
        }
        else{
            
            if(tens){
                writeCharacter(tens | 0x30);
            }
            
        }
        
        writeCharacter(ones | 0x30);
        
        // -----------------------
        
        // .
        writeCharacter(0b00101110);
        
        // -----------Fraction part------------
        
        fraction = fraction * 625;
        unsigned int tenths             =  fraction/1000;
        unsigned int centesimals        = (fraction/100) % 10;
        unsigned int millesimals        = (fraction/10) % 10;
        unsigned int tensmillesimals    = (fraction % 100) % 10;
        
        writeCharacter(tenths | 0x30);
        writeCharacter(centesimals | 0x30);
        writeCharacter(millesimals | 0x30);
        writeCharacter(tensmillesimals | 0x30);
        
        writeCharacter(0xDF);
        writeCharacter('C');
        
    }
    else{
        
        executeCommand(CLEAR_DISPLAY);
        writeCharacter('C');
        writeCharacter('R');
        writeCharacter('C');
        writeCharacter('-');
        writeCharacter('e');
        writeCharacter('r');
        writeCharacter('r');
        writeCharacter('o');
        writeCharacter('r');
        
    }
    
    
}
