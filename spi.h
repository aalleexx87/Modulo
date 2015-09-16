#ifndef SPI_H
#define SPI_H

#include "def.h"

#define CLK_DDR DDRB
#define CLK_PORT PORTB
#define CLK_BIT _BV(5)

#define MISO_DDR DDRB
#define MISO_PIN PINB
#define MISO_BIT _BV(4)

#define MOSI_DDR DDRB
#define MOSI_PORT PORTB
#define MOSI_BIT _BV(3)

void spi_init(void);
void spi_send(uint8_t cData);
uint8_t spi_receive(void);

#endif // SPI_H
