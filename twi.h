#ifndef TWI_H
#define TWI_H

#include "def.h"

#define START           0x08
#define REPEATED_START  0x10
#define MT_SLA_ACK	0x18
#define MT_SLA_NACK	0x20
#define MT_DATA_ACK     0x28
#define MT_DATA_NACK    0x30
#define MR_SLA_ACK	0x40
#define MR_SLA_NACK	0x48
#define MR_DATA_ACK     0x50
#define MR_DATA_NACK    0x58
#define LOST_ARBTRTN    0x38
#define TWI_STATUS      (TWSR & 0xF8)
#define SLA_W(address)  (address << 1)
#define SLA_R(address)  ((address << 1) + 0x01)
#define cbi(sfr, bit)   (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)   (_SFR_BYTE(sfr) |= _BV(bit))

#define MAX_BUFFER_SIZE 32

void twi_begin(void);
void twi_end(void);
void twi_timeOut(uint16_t);
void twi_setSpeed(uint8_t);
void twi_pullup(uint8_t);
void twi_scan(void);

uint8_t twi_available(void);
uint8_t twi_receive(void);
uint8_t twi_write_adr_ptr(uint8_t address, uint8_t registerAddress);
uint8_t twi_write_byte(uint8_t address, uint8_t registerAddress, uint8_t data);
uint8_t twi_write_str(uint8_t address, uint8_t registerAddress, char *data);
uint8_t twi_write_bytes(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t numberBytes);
uint8_t twi_read_byte(uint8_t address, uint8_t registerAddress, uint8_t numberBytes);
uint8_t twi_read_bytes(uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t *dataBuffer);

#endif // TWI_H
