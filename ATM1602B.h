/*
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf) */

#include <stdint.h>

 // Initialize display
void initLCD(void);

// System set
void systemSet(void);

// Entry mode set
void entryModeSet(void);

// Clear display
void clearDisplay(void);

// Display on
void displayOn(void);

// Check busy flag
void readBusyFlag(void);

// Send E signal
void sendEnable(void);

// Write a single character
void writeCharacter(uint8_t character);

// Execute command
void executeCommand(uint8_t command);

// System set
void systemSet(void);

// Entry Mode
void entryModeSet(void);

// Clear display
void clearDisplay(void);

// Display on
void displayOn(void);

// Place characters at specified address
void position(uint8_t address);