/* 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "temphandler.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

volatile char counter;

int main(void)
{
    
    // set for 16 MHz clock
    CPU_PRESCALE(0);
    
    startLCD();    
    
    // initialize Timer1
    cli();             // disable global interrupts
    
    // enable Timer1 overflow interrupt:
    TIMSK1 = (1 << TOIE1);
    
    // Set CS10 and CS12 bit so timer runs at clock speed/1024:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    
    // enable global interrupts:
    sei();

    while(1){

	}

}


ISR(TIMER1_OVF_vect)
{
    /* Timer 0 overflow */
    printCelsiusTemperature();

}
