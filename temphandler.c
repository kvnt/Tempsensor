//
//  temphandler.c
//  
//
//  Created by robert on 2015-02-04.
//
//

#include "temphandler.h"
#include "ATM1602B.h"
#include "DS18B20.h"
#include <avr/io.h>

void startLCD(void){
    
    initLCD();
    displayOn();
    
}

void printCelsiusTemperature(void){
    
    while (readScratchPad() != 1) {}
    
    unsigned int temp = getTemperatureRegisterData();
    
    position(0x00);
    
    printIntAsBinary(temp);
    
    position(0x41);
    
    writeCharacter(0b01010100);
    writeCharacter(0b01100101);
    writeCharacter(0b01101101);
    writeCharacter(0b01110000);
    writeCharacter(0b00111010);
    
    unsigned int signBits = (temp & 0b1111100000000000) >> 11;
    unsigned int integer  = (temp & 0b0000011111110000) >> 4;
    unsigned int fraction = (temp & 0b0000000000001111);
    
    if (signBits){ //Negativt
        
    }
    else{ //Positivt
        
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
        
    }
    
    writeCharacter(0b11011111);
    writeCharacter(0b01000011);
    
}
