#include "uart.h"

int put_char(char byte, FILE *stream) {
  while ( _GB(UCSR0A , _BV(UDRE0)) == 0 );
  UDR0 = byte;
  return 0;
}

int uart_send(uint8_t byte) {
  while ( _GB(UCSR0A , _BV(UDRE0)) == 0 );
  UDR0 = byte;
  return 0;
}

FILE uart_str = FDEV_SETUP_STREAM(put_char, NULL, _FDEV_SETUP_RW);

void uart_init(uint32_t baud) {
  uint16_t baudrate = (uint16_t)BAUD_PRESCALE(baud);
  UBRR0H = (uint8_t)(baudrate >> 8);
  UBRR0L = (uint8_t) baudrate;
  UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);
  UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
  stdout = &uart_str;
}

