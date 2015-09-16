#ifndef DEF_H
#define DEF_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BAUD_PRESCALE(BAUDRATE) ((( F_CPU / ( BAUDRATE * 16UL))) - 1)

#define _GB(byte, mask) (byte &   mask)
#define _SB(byte, mask) (byte |=  mask)
#define _CB(byte, mask) (byte &= ~mask)
#define _TB(byte, mask) (byte ^=  mask)
#define _CPB(src_byte, src_bit, dest_byte, dest_bit) (_GB(src_byte, src_bit)) ? (_SB(dest_byte, dest_bit)) : (_CB(dest_byte, dest_bit))


#endif
