/*
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf)
 *
 * Created by Robert Kvant 2015 http://www.github.com/kvnt
 *
 */

#include <stdint.h>

//Commands
#define SYSTEM_SET_8BITS_2LINES_10DOTS          0x3C
#define SYSTEM_SET_8BITS_1LINE_10DOTS           0x34
#define ENTRY_MODE_SET_INC_NOSHIFT              0x6
#define DISPLAY_ON_NO_CURSOR_NO_BLINK           0xC


 // Initialize display
void initLCD(void);

// Write a single character
void writeCharacter(uint8_t character);

// System set
void systemSet(uint8_t command);

// Entry Mode
void entryModeSet(uint8_t command);

// Clear display
void clearDisplay(void);

// Display on
void displayOn(uint8_t command);

// Place cursor at specified address
void position(uint8_t address);