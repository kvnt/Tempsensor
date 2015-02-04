//
//  DS18B20.h
//  
//
//  Created by robert on 2015-01-27.
//
//

/*  INITIALIZATION PROCEDURE—RESET AND PRESENCE PULSES
 --------------------------------------------------
 'All communication with the DS18B20 begins with an initialization
 sequence that consists of a reset pulse from the master followed
 by a presence pulse from the DS18B20.'
 
 */

void initSequence(void);



/*  WRITE TIME SLOT
 -----------------------------
 'There are two types of write time slots: “Write 1” time slots
 and “Write 0” time slots. The bus master uses a Write 1 time
 slot to write a logic 1 to the DS18B20 and a Write 0 time slot
 to write a logic 0 to the DS18B20. All write time slots must
 be a minimum of 60μs in duration with a minimum of a 1μs recovery
 time between individual write slots. Both types of write time slots
 are initiated by the master pulling the 1-Wire bus low.'
 
 void writeTimeSlot(char bit) takes zero or one as input and writes
 it to the sensor.
 
 */

void writeTimeSlot(unsigned char bit);



/* READ TIME SLOTS
 -------------------------------
 'The DS18B20 can only transmit data to the master when the master
 issues read time slots. Therefore, the master must generate read
 time slots immediately after issuing a Read Scratchpad [BEh] or
 Read Power Supply [B4h] command, so that the DS18B20 can provide
 the requested data.'
 
 unsigned char readTimeSlot() returns the read bit.
 
 */

unsigned char readTimeSlot(void);

void issueCommand(unsigned char command);

unsigned int readScratchPad(void);

unsigned int getTemperatureRegisterData(void);
unsigned int getCrcRegisterData(void);