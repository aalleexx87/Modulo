#ifndef NRF24L01_DEF
#define NRF24L01_DEF

#include "def.h"

/* Memory Map */
#define R_CONFIG      0x00
#define R_EN_AA       0x01
#define R_EN_RXADDR   0x02
#define R_SETUP_AW    0x03
#define R_SETUP_RETR  0x04
#define R_RF_CH       0x05
#define R_RF_SETUP    0x06
#define R_STATUS      0x07
#define R_OBSERVE_TX  0x08
#define R_CD          0x09
#define R_RX_ADDR_P0  0x0A
#define R_RX_ADDR_P1  0x0B
#define R_RX_ADDR_P2  0x0C
#define R_RX_ADDR_P3  0x0D
#define R_RX_ADDR_P4  0x0E
#define R_RX_ADDR_P5  0x0F
#define R_TX_ADDR     0x10
#define R_RX_PW_P0    0x11
#define R_RX_PW_P1    0x12
#define R_RX_PW_P2    0x13
#define R_RX_PW_P3    0x14
#define R_RX_PW_P4    0x15
#define R_RX_PW_P5    0x16
#define R_FIFO_STATUS 0x17
#define R_DYNPD	    0x1C
#define R_FEATURE	    0x1D

/* Bit Mnemonics */
#define R_MASK_RX_DR  6
#define R_MASK_TX_DS  5
#define R_MASK_MAX_RT 4
#define R_EN_CRC      3
#define R_CRCO        2
#define R_PWR_UP      1
#define R_PRIM_RX     0
#define R_ENAA_P5     5
#define R_ENAA_P4     4
#define R_ENAA_P3     3
#define R_ENAA_P2     2
#define R_ENAA_P1     1
#define R_ENAA_P0     0
#define R_ERX_P5      5
#define R_ERX_P4      4
#define R_ERX_P3      3
#define R_ERX_P2      2
#define R_ERX_P1      1
#define R_ERX_P0      0
#define R_AW          0
#define R_ARD         4
#define R_ARC         0
#define R_PLL_LOCK    4
#define R_RF_DR       3
#define R_RF_PWR      6
#define R_RX_DR       6
#define R_TX_DS       5
#define R_MAX_RT      4
#define R_RX_P_NO     1
#define R_TX_FULL     0
#define R_PLOS_CNT    4
#define R_ARC_CNT     0
#define R_TX_REUSE    6
#define R_FIFO_FULL   5
#define R_TX_EMPTY    4
#define R_RX_FULL     1
#define R_RX_EMPTY    0
#define R_DPL_P5	    5
#define R_DPL_P4	    4
#define R_DPL_P3	    3
#define R_DPL_P2	    2
#define R_DPL_P1	    1
#define R_DPL_P0	    0
#define R_EN_DPL	    2
#define R_EN_ACK_PAY  1
#define R_EN_DYN_ACK  0

/* Instruction Mnemonics */
#define R_R_REGISTER    0x00
#define R_W_REGISTER    0x20
#define R_REGISTER_MASK 0x1F
#define R_ACTIVATE      0x50
#define R_R_RX_PL_WID   0x60
#define R_R_RX_PAYLOAD  0x61
#define R_W_TX_PAYLOAD  0xA0
#define R_W_ACK_PAYLOAD 0xA8
#define R_FLUSH_TX      0xE1
#define R_FLUSH_RX      0xE2
#define R_REUSE_TX_PL   0xE3
#define R_NOP           0xFF

/* Non-P omissions */
#define R_LNA_HCURR   0

/* P model memory Map */
#define R_RPD         0x09

/* P model bit Mnemonics */
#define R_RF_DR_LOW   5
#define R_RF_DR_HIGH  3
#define R_RF_PWR_LOW  1
#define R_RF_PWR_HIGH 2

#define R_CLEAR 0
#define R_SET 1
#define R_TOGGLE 2
#define R_REPLACE 3

#define R_SS_LOW _CB(PORTB, _BV(2))
#define R_SS_HIGH _SB(PORTB, _BV(2))

#define R_CE_LOW _CB(PORTB, _BV(1))
#define R_CE_HIGH _SB(PORTB, _BV(1))

void radio_send_cmd(uint8_t cmd);
void radio_change_reg(uint8_t addr, uint8_t mask, uint8_t type);
void radio_init(void);
uint8_t radio_read_reg(uint8_t addr);

#endif // NRF24L01_DEF

