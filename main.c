#include "def.h"
#include "uart.h"
#include "time.h"

#include "twi.h"
#include "m6050_defs.h"

#include "spi.h"
#include "radio.h"


int main(void)
{
    uart_init(9600);

    spi_init();
    _delay_ms(100);
    radio_init();

    radio_change_reg(R_CONFIG, R_PRIM_RX, R_CLEAR);

    radio_change_reg(R_RF_SETUP, _BV(7), R_SET);
    _delay_ms(2000);
    radio_change_reg(R_RF_SETUP, _BV(7), R_CLEAR);

    twi_setSpeed(0);
    twi_begin();

    twi_write_byte(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_GYRO_CONFIG, 0);
    //twi_write_byte(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_2 << 3);
    twi_write_byte(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_PWR_MGMT_1, _BV(3));


    uint8_t acc[6];
    int8_t gyro[6];

    while (1) {
        uint8_t err = twi_read_bytes(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_XOUT_H, 6, acc);


        printf("\r\n");
    }

}
