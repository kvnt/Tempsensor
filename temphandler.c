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

void startLCD(void){
    
    initLCD();
    executeCommand(SYSTEM_SET_8BITS_2LINES_10DOTS);
    executeCommand(ENTRY_MODE_SET_INC_NOSHIFT);
    executeCommand(DISPLAY_ON_NO_CURSOR_NO_BLINK);
    executeCommand(CLEAR_DISPLAY);
    executeCommand(SYSTEM_SET_8BITS_2LINES_10DOTS);
    
}


void printCelsiusTemperature(void){
    
    while (readScratchPad() != 1) {}
    
    unsigned int temp = getTemperatureRegisterData();
    
    position(0x41);
    
    if (verifyCrc()) {
        
        writeCharacter(0b01010100);
        writeCharacter(0b01100101);
        writeCharacter(0b01101101);
        writeCharacter(0b01110000);
        writeCharacter(0b00111010);
        
        unsigned int signBits = (temp & 0b1111100000000000) >> 11;
        unsigned int integer  = (temp & 0b0000011111110000) >> 4;
        unsigned int fraction = (temp & 0b0000000000001111);
        
        if (signBits){ //Negativt
            
            unsigned int tempBits = (temp & 0b0000011111111111);
            tempBits = ~tempBits;
            tempBits = tempBits + 1;
            integer = (tempBits & 0b0000011111110000) >> 4;
            fraction = (tempBits & 0b0000000000001111);
            writeCharacter(0b00101101);
            
        }
        
        
        unsigned int hundreds  = integer / 100;
        unsigned int tens      = (integer % 100) / 10;
        unsigned int ones      = (integer % 100) % 10;
        
        if (hundreds){
            
            writeCharacter(hundreds | 0b00110000);
            writeCharacter(tens | 0b00110000);
            
        }
        else{
            
            if(tens){
                writeCharacter(tens | 0b00110000);
            }
            
        }
        
        writeCharacter(ones | 0b00110000);
        
        // -----------------------
        
        writeCharacter(0b00101110);
        
        // -----------------------
        
        fraction = fraction * 625;
        unsigned int tenths             = fraction/1000;
        unsigned int centesimals        = (fraction/100) % 10;
        unsigned int millesimals        = (fraction/10) % 10;
        unsigned int tensmillesimals     = (fraction % 100) % 10;
        
        writeCharacter(tenths | 0b00110000);
        writeCharacter(centesimals | 0b00110000);
        writeCharacter(millesimals | 0b00110000);
        writeCharacter(tensmillesimals | 0b00110000);
        
        
        
        writeCharacter(0b11011111);
        writeCharacter(0b01000011);
        
    }
    else{
        
        executeCommand(CLEAR_DISPLAY);
        writeCharacter(0b01000011); //C
        writeCharacter(0b01010010); //R
        writeCharacter(0b01000011); //C
        
    }
    
    
}
