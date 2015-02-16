/*
 *
 * Created by Robert Kvant 2015 http://www.github.com/kvnt
 *
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "DS18B20.h"

// -------------Miscellaneous macros------------------
#define PULL_BUS_LOW    (DDRF = DDRF | 0b00000001)
#define RELEASE_BUS     (DDRF = DDRF & 0b11111110)

#define BUS_IS_HIGH     ((PINF & 0b00000001) == 0b00000001)
#define BUS_IS_LOW      ((PINF & 0b00000001) == 0b00000000)

#define READ_BUS        (PINF & 0b00000001)

// ROM-commands
#define SKIP_ROM        issueCommand(0xCC)

// Function-commands
#define CONVERT_T       {issueCommand(0x44); while (readTimeSlot() == 0){}}
#define READ_SCRATCH    issueCommand(0xBE)


/* -------------Scratchpad --------------
 Contains all data in scratchpad-register (DS18B20)
 */

uint8_t scratch[9];


/*  INITIALIZATION PROCEDURE—RESET AND PRESENCE PULSES
 --------------------------------------------------
 'All communication with the DS18B20 begins with an initialization
 sequence that consists of a reset pulse from the master followed 
 by a presence pulse from the DS18B20.'
  */

void initSequence(void){
    
    // PORTF0 always zero.
    PORTF &= 0xFE;    
    
    // ---- MASTER TX RESET PULSE ---- //
    
    PULL_BUS_LOW;
    
    _delay_us(480);
    
    RELEASE_BUS;
   
    // ---- MASTER RX (SENSOR PRESENCE PULSE) ---- //
    
    _delay_us(15);
 
    while (BUS_IS_HIGH) {}
    
    _delay_us(60);

    while (BUS_IS_LOW) {}
    
    _delay_us(405);
    
}



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

void writeTimeSlot(uint8_t bit){
    
    switch (bit) {            
            
        case 0:
            
            PULL_BUS_LOW;
            
            _delay_us(60);
            
            RELEASE_BUS;
            
            break;
            
        case 1:
            
            PULL_BUS_LOW;;
            
            _delay_us(10);
            
            RELEASE_BUS;
            
            _delay_us(50);
            
            break;

    }
    
    _delay_us(2);
    
}

/* READ TIME SLOTS
 -------------------------------
 'The DS18B20 can only transmit data to the master when the master
 issues read time slots. Therefore, the master must generate read
 time slots immediately after issuing a Read Scratchpad [BEh] or
 Read Power Supply [B4h] command, so that the DS18B20 can provide
 the requested data.'
 
 unsigned char readTimeSlot() returns the read bit.
 
 */

uint8_t readTimeSlot(void){
    
    uint8_t readBit = 0;
    
    // --- Master initiates the read time slot --
    
    PULL_BUS_LOW;
    
    _delay_us(2);
    
    RELEASE_BUS;
    
    _delay_us(10);
    
    //Sample bus
    readBit = READ_BUS;
    
    _delay_us(48);
    
    while (BUS_IS_LOW) {}
    
    _delay_us(2);    

    return readBit;
    
}


void issueCommand(uint8_t command){
    
    uint8_t mask = 0x1;
    
    for (uint8_t i = 0; i < 8; i++){
        
        writeTimeSlot((mask & command) >> i);
        
        mask <<= 1;
    }
    
}


uint16_t readScratchPad(void){
    
    initSequence();
    
    SKIP_ROM;
    CONVERT_T;
    
    initSequence();
    
    SKIP_ROM;
    READ_SCRATCH;


    for (uint8_t n = 0; n < 9; ++n)
    {

        uint8_t s = 0;
        scratch[n] = 0;

        for (uint8_t i = 0; i < 8; i++) {
        
            if(readTimeSlot()){

               s |= (1 << i);

            }
        
        }

        scratch[n] = s;
    }

    return 1;
}


uint8_t verifyCrc(void){
    
    register uint8_t shiftregister = 0,
                            output = 0,
                            input  = 0;


    for (uint8_t n = 0; n < 9; ++n)
    {

        input = scratch[n];

        for (uint8_t c = 0; c < 8; c++) {
        
            
            output = (!(input & (1 << c)) != !(0x1 & shiftregister));
            

            shiftregister = (((shiftregister & 0x7) >> 1) | 
                                        (shiftregister & 0xF8));

            shiftregister = ((!(shiftregister & 0x8) != !output) << 2) | 
                                        (shiftregister & 0xFB);

            shiftregister = ((!(shiftregister & 0x10) != !output) << 3) | 
                                        (shiftregister & 0xF7);

            shiftregister = (((shiftregister & 0xF0) >> 1) & 0xF0) | 
                                        (shiftregister & 0x0F); 


            shiftregister = (output << 7) | (shiftregister & 0x7F);
        
        }

    }
    
  
    return shiftregister == 0;
}


uint16_t getTemperatureRegisterData(void){

    uint16_t temp = scratch[1];
    temp <<= 8;
    temp |= scratch[0];
    return temp;
    
}

