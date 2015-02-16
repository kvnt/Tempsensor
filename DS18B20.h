/*
 * Code to control DS18B20 temperaturesensor.
 * http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
 *
 * Created by Robert Kvant 2015 http://www.github.com/kvnt
 *
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