#include "spi.h"


void spi_init(void) {
    _SB(DDRD, _BV(4));
    _SB(CLK_DDR, CLK_BIT);
    _SB(MOSI_DDR, MOSI_BIT);
    _CB(MISO_DDR, MISO_BIT);
}

void spi_send(uint8_t byte) {
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      _CB(CLK_PORT, CLK_BIT);
      _CPB(byte, bit, MOSI_PORT, MOSI_BIT);
      _SB(CLK_PORT, CLK_BIT);
    }
    _CB(CLK_PORT, CLK_BIT);
}

uint8_t spi_send_duplex(uint8_t byte) {
    uint8_t data = 0;
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      _CB(CLK_PORT, CLK_BIT);
      _CPB(byte, bit, MOSI_PORT, MOSI_BIT);
      _CPB(MISO_PIN, MISO_BIT, data, bit);
      _SB(CLK_PORT, CLK_BIT);
    }
    _CB(CLK_PORT, CLK_BIT);
    return data;
}

uint8_t spi_receive(void) {
    uint8_t data = 0;
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      _CB(CLK_PORT, CLK_BIT);
      _CPB(MISO_PIN, MISO_BIT, data, bit);
      _SB(CLK_PORT, CLK_BIT);
    }
    _CB(CLK_PORT, CLK_BIT);
    return data;
}
