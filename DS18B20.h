/*
 * Code to control DS18B20 temperaturesensor.
 * http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
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

/*  INITIALIZATION PROCEDURE—RESET AND PRESENCE PULSES
 --------------------------------------------------
 'All communication with the DS18B20 begins with an initialization
 sequence that consists of a reset pulse from the master followed
 by a presence pulse from the DS18B20.'
 */

void initSequence(void);



/*  WRITE TIME SLOT
 -----------------------------
 void writeTimeSlot(char bit) takes zero or one as input and writes
 it to the sensor.
 
 */

void writeTimeSlot(uint8_t bit);



/* READ TIME SLOTS
 -------------------------------
 unsigned char readTimeSlot() returns the read bit.
 
 */

uint8_t readTimeSlot(void);


/* Takes a function or rom command as input
 and transmittes it to the sensor */

void issueCommand(uint8_t command);



/* Issues a tempconversion and reads the whole
 scratchpad structure. */

uint16_t readScratchPad(void);



/* Returns the tempdata in scratchpad */

uint16_t getTemperatureRegisterData(void);




/* Takes the whole scratchpad and shifts it into
 the shiftregister. Finally the CRC is shifted
 in and if the result is zero the verification has
 passed and hopefully the data is error-free. */

uint8_t verifyCrc(void);