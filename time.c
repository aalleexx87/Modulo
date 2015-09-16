#include "time.h"

uint32_t _time = 0;

ISR(TIMER1_COMPA_vect){
    _time += 10;
    OCR1A = (OCR1A + 0x9C)%0xFFFF;
}

void start_timer(void) {
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TIMSK1 |= _BV(OCIE1A);
    OCR1A = 0x9C;
}
