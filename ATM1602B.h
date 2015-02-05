/* 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Copyright 2015 Robert Kvant
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * Code to control ATM1602B - Liquid Crystal Display Module
 * (http://www.hebeiltd.com.cn/lcm.datasheet/ATM1602B.pdf)
 *	----------------------------------
 * 	|	AVR  	| 	LCD	 |	
 * 	----------------------------------
 * 	| D0 		:	RS       |
 * 	| D1 		: 	R/W	 |
 * 	| D2 		: 	E	 |
 *	|				 |
 * 	| B0 - B7 	:	D0 - D7  |	
 * 	----------------------------------
 */


 // Initialize display
void initLCD(void);

void systemSet(void);

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
void writeCharacter(unsigned char character);

void printCharAsBinary(unsigned char bin);
void printIntAsBinary(unsigned int bin);
void position(unsigned char address);