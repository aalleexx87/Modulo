#include "radio.h"
#include "spi.h"

void radio_init(void) {
    _SB(DDRB, (_BV(2) | _BV(1)));
    radio_change_reg(R_CONFIG, R_PWR_UP, R_SET);
    radio_change_reg(R_SETUP_RETR, 0x4F, R_REPLACE);
    radio_change_reg(R_RF_SETUP, 0x00, R_REPLACE);
    radio_change_reg(R_CONFIG, R_EN_CRC, R_SET);
    radio_change_reg(R_STATUS, _BV(R_RX_DR) | _BV(R_TX_DS) | _BV(R_MAX_RT), R_SET);
    radio_change_reg(R_STATUS, _BV(R_RX_DR) | _BV(R_TX_DS) | _BV(R_MAX_RT), R_SET);
    radio_change_reg(R_DYNPD, 0x00, R_REPLACE);
    radio_change_reg(R_RF_CH, 73, R_REPLACE);
    radio_send_cmd(R_FLUSH_RX);
    radio_send_cmd(R_FLUSH_TX);
}

void radio_send_cmd(uint8_t cmd) {
    R_SS_LOW;
    spi_send(cmd);
    R_SS_HIGH;
}

uint8_t radio_read_reg(uint8_t addr) {
    uint8_t data;
    R_SS_LOW;
    spi_send(R_R_REGISTER | addr);
    data = spi_receive();
    R_SS_HIGH;

    return data;
}

void radio_change_reg(uint8_t addr, uint8_t mask, uint8_t type) {
    uint8_t data = radio_read_reg(addr);

    switch (type) {
    case R_SET:
        _SB(data, mask);
        break;
    case R_CLEAR:
        _CB(data, mask);
        break;
    case R_TOGGLE:
        _TB(data, mask);
        break;
    case R_REPLACE:
    default:
        data = mask;
        break;
    }

    R_SS_LOW;
    spi_send(R_W_REGISTER | addr);
    spi_send(data);
    R_SS_HIGH;
}
