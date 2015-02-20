/*
 *
 * Created by Robert Kvant 2015 http://www.github.com/kvnt
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "temphandler.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

int main(void)
{
    
    // set for 16 MHz clock
    CPU_PRESCALE(0);
    
    startLCD();

    cli(); // disable global interrupts
    
    // enable Timer1 overflow interrupt:
    TIMSK1 = (1 << TOIE1);
    
    // Set CS10 and CS12 bit so timer runs at clock speed/1024:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    
    sei(); // enable global interrupts:

    while(1){

	}

}

ISR(TIMER1_OVF_vect)
{
    printCelsiusTemperature();

}
