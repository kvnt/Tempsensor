/*
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf)
 *
 * Created by Robert Kvant 2015 http://www.github.com/kvnt
 *
 */

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

//Commands
#define SYSTEM_SET_8BITS_2LINES_10DOTS          0x3C
#define SYSTEM_SET_8BITS_1LINE_10DOTS           0x34
#define ENTRY_MODE_SET_INC_NOSHIFT              0x6
#define CLEAR_DISPLAY                           0x1
#define DISPLAY_ON_NO_CURSOR_NO_BLINK           0xC
#define SET_DDRAM_ADDRESS                       0x80

// Initialize display
void initLCD(void){
    
    // Set as outputs
    DDRD = DDRD | 0b00000111;
    DDRB = DDRB | 0b11111111;
    
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
void systemSet(uint8_t command){
    
    executeCommand(command);
    
}

// Entry Mode set
void entryModeSet(uint8_t command){
    
    executeCommand(command);
    
}

// Clear display
void clearDisplay(void){
    
    executeCommand(CLEAR_DISPLAY);
    
}

// Display on
void displayOn(uint8_t command){
    
    executeCommand(command);
    
}

// Place cursor at specified address
void position(uint8_t address){
    
    executeCommand(address | SET_DDRAM_ADDRESS);
    
}





