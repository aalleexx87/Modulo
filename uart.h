#ifndef UART_H
#define UART_H

#include "def.h"

void uart_init(uint32_t baud);
int uart_send(uint8_t byte);

#endif // UART_H
