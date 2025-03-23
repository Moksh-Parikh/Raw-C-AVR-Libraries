/* Moksh Parikh, 22.03.2025 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "HCSR04_lib.h"

#define TRIGGER PD2
#define ECHO PD3
#define ECHO_PIN PIND

// speed of sound in centimetres per microsecond
float speedOfSound = 0.0343;

volatile long double distance;
volatile int time;


void pulseTriggerPin() {
    // turn off interrupts to ensure the sonic burst is triggered
    cli();
    // ensure the pin is low
    PORTD &= ~(1 << TRIGGER);
    _delay_us(2);
    
    // set the pin high for ten microseconds
    PORTD |= (1 << TRIGGER);
    _delay_us(10);
    PORTD &= ~(1 << TRIGGER);
    
    sei();
}


void startTimer1() {
    // set timer value to zero
    TCNT1 = 0;

    // sets prescaler to clk/8, 2 million oscillations per second
    // 500 nanosecond resoulution
    TCCR1B |= (1 << CS11);
}


void stopTimer1() {
    // sets the timer to no clock source, off
    TCCR1B = 0;
}


// initialise Pin Change Interrupt on pin 3 (PCINT19)
void initPCISR(void) {
    // set pin change mask to section two, PCINT16 to 23,
    // the D Bank
    PCICR |= (1 << PCIE2);

    // trigger when pin 19 is changed
    PCMSK2 |= (1 << PCINT19);

    sei();
}


ISR(PCINT2_vect) {
    // the sensor returns a high pulse that starts when the 
    // sound is released, and ends when it returns
    if (bit_is_set(ECHO_PIN, ECHO)) {
        // start timer
        startTimer1();
    }
    else {
        // stop timer and calculate distance
        
        stopTimer1();
        
        // clock ticks once evry 500 nanoseconds
        // divide this by two to get the time in microseconds
        time = TCNT1 / 2;

        distance = (float)(time) * (speedOfSound);
        
        // sound makes a round trip, divide by two to get distance of first "leg"
        distance /= (float)2;
    }
    time = 0;
}



int main(void) {
    // set the trigger pin to output
    DDRD |= (1 << TRIGGER);

    // initialise pin change interrupts
    initPCISR();

    while (1) {
        pulseTriggerPin();
    }

    return 0;
}
