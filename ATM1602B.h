/*
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf)
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

/* Display commands */
#define SYSTEM_SET_8BITS_2LINES_10DOTS          0x3C
#define SYSTEM_SET_8BITS_1LINE_10DOTS           0x34
#define SYSTEM_SET_4BITS_2LINES_10DOTS          0x2C
#define SYSTEM_SET_4BITS_1LINE_10DOTS           0x24
#define SYSTEM_SET_8BITS_2LINES_7DOTS           0x38
#define SYSTEM_SET_8BITS_1LINE_7DOTS            0x30
#define SYSTEM_SET_4BITS_2LINES_7DOTS           0x28
#define SYSTEM_SET_4BITS_1LINE_7DOTS            0x20

#define ENTRY_MODE_SET_INC_SHIFT                0x7
#define ENTRY_MODE_SET_INC_NOSHIFT              0x6
#define ENTRY_MODE_SET_DEC_SHIFT                0x5
#define ENTRY_MODE_SET_DEC_NOSHIFT              0x4

#define CLEAR_DISPLAY                           0x1

#define DISPLAY_ON_NO_CURSOR_NO_BLINK           0xC
#define DISPLAY_ON_NO_CURSOR_BLINK              0xD
#define DISPLAY_ON_CURSOR_NO_BLINK              0xE
#define DISPLAY_ON_CURSOR_BLINK                 0xF

#define DISPLAY_OFF                             0x8

#define CURSOR_SHIFT_ONE_RIGHT                  0x1C
#define CURSOR_SHIFT_ONE_LEFT                   0x18

#define SET_DDRAM_ADDRESS                       0x80
#define CURSOR_HOME                             0x2


/* Initialize display */
void initLCD(void);

/* Takes data of type uint8_t as
 input and writes it to the LCD. */
void writeData(uint8_t data);

/* Takes one of the specified command
 macros as input and sends it to the
 LCD. */
void executeCommand(uint8_t command);

/* Takes the address as input and sends it
 to the LCD */
void position(uint8_t address);