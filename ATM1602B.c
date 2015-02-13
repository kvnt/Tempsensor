/*
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf) */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ATM1602B.h"

#define ENABLE_E            PORTD   |= 0x4
#define DISABLE_E           PORTD   &= 0xFB
#define BUSY_FLAG_HIGH      (PINB   & 0x80) == 0x80
#define DB7_INPUT           DDRB    &= 0x7F
#define DB7_OUTPUT          DDRB    |= 0x80
#define RW_ONE              PORTD   |= 0x2
#define RW_ZERO             PORTD   &= 0xFD
#define RS_ONE              PORTD   |= 0x1
#define RS_ZERO             PORTD   &= 0xFE


// Initialize display
void initLCD(void){
    
    // Set as outputs
    DDRD = DDRD | 0b00000111;
    DDRB = DDRB | 0b11111111;
    
    systemSet();
    clearDisplay();
    entryModeSet();
    systemSet();
    
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
    DB7_INPUT;
    
    // Set RS = 0 och R/W = 1 (Read Busy Flag)
    RS_ZERO;
    RW_ONE;
    
    _delay_us(0.2);
    
    // Enable E
    ENABLE_E;
    
    _delay_us(0.4);
    
    // Wait until BF=0
    while (BUSY_FLAG_HIGH){}
    
    // Disable E
    DISABLE_E;
    
    _delay_us(0.2);
    
    // Set pin B7 as output (HIGH)
    DB7_OUTPUT;
    
}

// Write a single character
void writeCharacter(uint8_t character){
    
    // Set RS = 1, R/W = 0
    RW_ZERO;
    RS_ONE;
    
    // Assign character to port B
    PORTB = character;
    
    sendEnable();
    readBusyFlag();
    
}

// Execute command
void executeCommand(uint8_t command){
    
    RS_ZERO;
    RW_ZERO;
    
    PORTB = command;
    
    sendEnable();
    readBusyFlag();
    
}

// System set
void systemSet(void){
    
    executeCommand(0x3C);
    
}

// Entry Mode set
void entryModeSet(void){
    
    executeCommand(0x6);
    
}

// Clear display
void clearDisplay(void){
    
    executeCommand(0x1);
    
}

// Display on
void displayOn(void){
    
    executeCommand(0xC);
    
}

// Place characters at specified address
void position(uint8_t address){
    
    executeCommand(address | 0x80);
    
}





