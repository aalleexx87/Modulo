#include "twi.h"
#include "time.h"

uint8_t twi_start(void);
uint8_t twi_sendAddress(uint8_t);
uint8_t twi_sendByte(uint8_t);
uint8_t twi_receiveByte(uint8_t);
uint8_t twi_stop(void);
void twi_lockUp(void);
uint8_t returnStatus;
uint8_t nack;
uint8_t data[MAX_BUFFER_SIZE];
uint8_t bytesAvailable = 0;
uint8_t bufferIndex = 0;
uint8_t totalBytes = 0;
uint16_t timeOutDelay = 0;

void twi_begin(void) {
    cbi(PORTC, 4);
    cbi(PORTC, 5);
    cbi(TWSR, TWPS0);
    cbi(TWSR, TWPS1);
    TWBR = ((F_CPU / 100000) - 16) / 2;
    TWCR = _BV(TWEN) | _BV(TWEA);
}

void twi_end(void) {
    TWCR = 0;
}

void twi_timeOut(uint16_t _timeOut) {
    timeOutDelay = _timeOut;
}

void twi_setSpeed(uint8_t _fast) {
    if(!_fast) {
        TWBR = ((F_CPU / 100000) - 16) / 2;
    } else {
        TWBR = ((F_CPU / 400000) - 16) / 2;
    }
}

void twi_scan() {
    uint16_t tempTime = timeOutDelay;
    twi_timeOut(80);
    uint8_t totalDevicesFound = 0;
    printf("Scanning for devices...please wait");
    for(uint8_t s = 0; s <= 0x7F; s++) {
        returnStatus = 0;
        returnStatus = twi_start();
        if(!returnStatus) {
            returnStatus = twi_sendAddress(SLA_W(s));
        }
        if(returnStatus) {
            if(returnStatus == 1) {
                printf("There is a problem with the bus, could not complete scan");
                timeOutDelay = tempTime;
                return;
            }
        } else {
            printf("Found device at address - ");
            printf(" 0x");
            printf("0x%X", s);
            totalDevicesFound++;
        }
        twi_stop();
    }
    if(!totalDevicesFound){printf("No devices found");}
    timeOutDelay = tempTime;
}

uint8_t twi_available() {
    return(bytesAvailable);
}

uint8_t twi_receive() {
    bufferIndex = totalBytes - bytesAvailable;
    if(!bytesAvailable) {
        bufferIndex = 0;
        return(0);
    }
    bytesAvailable--;
    return(data[bufferIndex]);
}

/*return values for new functions that use the timeOut feature
  will now return at what point in the transmission the timeout
  occurred. Looking at a full communication sequence between a
  master and slave (transmit data and then readback data) there
  a total of 7 points in the sequence where a timeout can occur.
  These are listed below and correspond to the returned value:
  1 - Waiting for successful completion of a Start bit
  2 - Waiting for ACK/NACK while addressing slave in transmit mode (MT)
  3 - Waiting for ACK/NACK while sending data to the slave
  4 - Waiting for successful completion of a Repeated Start
  5 - Waiting for ACK/NACK while addressing slave in receiver mode (MR)
  6 - Waiting for ACK/NACK while receiving data from the slave
  7 - Waiting for successful completion of the Stop bit

  All possible return values:
  0           Function executed with no errors
  1 - 7       Timeout occurred, see above list
  8 - 0xFF    See datasheet for exact meaning */

/////////////////////////////////////////////////////

uint8_t twi_write_adr_ptr(uint8_t address, uint8_t registerAddress) {
    returnStatus = 0;
    returnStatus = twi_start();
    if(returnStatus){return(returnStatus);}
    returnStatus = twi_sendAddress(SLA_W(address));
    if(returnStatus) {
        if(returnStatus == 1){return(2);}
        return(returnStatus);
    }
    returnStatus = twi_sendByte(registerAddress);
    if(returnStatus) {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
    }
    returnStatus = twi_stop();
    if(returnStatus) {
        if(returnStatus == 1){return(7);}
        return(returnStatus);
    }
    return(returnStatus);
}

uint8_t twi_write_byte(uint8_t address, uint8_t registerAddress, uint8_t data) {
    returnStatus = 0;
    returnStatus = twi_start();
    if(returnStatus){return(returnStatus);}
    returnStatus = twi_sendAddress(SLA_W(address));
    if(returnStatus) {
        if(returnStatus == 1){return(2);}
        return(returnStatus);
    }
    returnStatus = twi_sendByte(registerAddress);
    if(returnStatus) {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
    }
    returnStatus = twi_sendByte(data);
    if(returnStatus) {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
    }
    returnStatus = twi_stop();
    if(returnStatus) {
        if(returnStatus == 1){return(7);}
        return(returnStatus);
    }
    return(returnStatus);
}

uint8_t twi_write_str(uint8_t address, uint8_t registerAddress, char *data) {
    uint8_t bufferLength = strlen(data);
    returnStatus = 0;
    returnStatus = twi_write_bytes(address, registerAddress, (uint8_t*)data, bufferLength);
    return(returnStatus);
}

uint8_t twi_write_bytes(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t numberBytes) {
    returnStatus = 0;
    returnStatus = twi_start();
    if(returnStatus){return(returnStatus);}
    returnStatus = twi_sendAddress(SLA_W(address));
    if(returnStatus) {
        if(returnStatus == 1){return(2);}
        return(returnStatus);
    }
    returnStatus = twi_sendByte(registerAddress);
    if(returnStatus) {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
    }
    for (uint8_t i = 0; i < numberBytes; i++) {
        returnStatus = twi_sendByte(data[i]);
        if(returnStatus) {
            if(returnStatus == 1){return(3);}
            return(returnStatus);
        }
    }
    returnStatus = twi_stop();
    if(returnStatus) {
        if(returnStatus == 1){return(7);}
        return(returnStatus);
    }
    return(returnStatus);
}

uint8_t twi_read_byte(uint8_t address, uint8_t registerAddress, uint8_t numberBytes) {
    bytesAvailable = 0;
    bufferIndex = 0;
    if(numberBytes == 0){numberBytes++;}
    nack = numberBytes - 1;
    returnStatus = 0;
    returnStatus = twi_start();
    if(returnStatus){return(returnStatus);}
    returnStatus = twi_sendAddress(SLA_W(address));
    if(returnStatus) {
        if(returnStatus == 1){return(2);}
        return(returnStatus);
    }
    returnStatus = twi_sendByte(registerAddress);
    if(returnStatus) {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
    }
    returnStatus = twi_start();
    if(returnStatus) {
        if(returnStatus == 1){return(4);}
        return(returnStatus);
    }
    returnStatus = twi_sendAddress(SLA_R(address));
    if(returnStatus) {
        if(returnStatus == 1){return(5);}
        return(returnStatus);
    }
    for(uint8_t i = 0; i < numberBytes; i++) {
        if( i == nack ) {
            returnStatus = twi_receiveByte(0);
            if(returnStatus == 1){return(6);}
            if(returnStatus != MR_DATA_NACK){return(returnStatus);}
        } else {
            returnStatus = twi_receiveByte(1);
            if(returnStatus == 1){return(6);}
            if(returnStatus != MR_DATA_ACK){return(returnStatus);}
        }
        data[i] = TWDR;
        bytesAvailable = i+1;
        totalBytes = i+1;
    }
    returnStatus = twi_stop();
    if(returnStatus) {
        if(returnStatus == 1){return(7);}
        return(returnStatus);
    }
    return(returnStatus);
}

uint8_t twi_read_bytes(uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t *dataBuffer) {
    bytesAvailable = 0;
    bufferIndex = 0;
    if(numberBytes == 0){numberBytes++;}
    nack = numberBytes - 1;
    returnStatus = 0;
    returnStatus = twi_start();
    if(returnStatus){return(returnStatus);}
    returnStatus = twi_sendAddress(SLA_W(address));
    if(returnStatus) {
        if(returnStatus == 1){return(2);}
        return(returnStatus);
    }
    returnStatus = twi_sendByte(registerAddress);
    if(returnStatus) {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
    }
    returnStatus = twi_start();
    if(returnStatus) {
        if(returnStatus == 1){return(4);}
        return(returnStatus);
    }
    returnStatus = twi_sendAddress(SLA_R(address));
    if(returnStatus) {
        if(returnStatus == 1){return(5);}
        return(returnStatus);
    }
    for(uint8_t i = 0; i < numberBytes; i++) {
        if( i == nack ) {
            returnStatus = twi_receiveByte(0);
            if(returnStatus == 1){return(6);}
            if(returnStatus != MR_DATA_NACK){return(returnStatus);}
        } else {
            returnStatus = twi_receiveByte(1);
            if(returnStatus == 1){return(6);}
            if(returnStatus != MR_DATA_ACK){return(returnStatus);}
        }
        dataBuffer[i] = TWDR;
        bytesAvailable = i+1;
        totalBytes = i+1;
    }
    returnStatus = twi_stop();
    if(returnStatus) {
        if(returnStatus == 1){return(7);}
        return(returnStatus);
    }
    return(returnStatus);
}

/////////////// Private Methods ////////////////////////////////////////

uint8_t twi_start() {
    unsigned long startingTime = _time;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT))) {
        if(!timeOutDelay){continue;}
        if((_time - startingTime) >= timeOutDelay) {
            twi_lockUp();
            return(1);
        }

    }
    if ((TWI_STATUS == START) || (TWI_STATUS == REPEATED_START)) {
        return(0);
    }
    if (TWI_STATUS == LOST_ARBTRTN) {
        uint8_t bufferedStatus = TWI_STATUS;
        twi_lockUp();
        return(bufferedStatus);
    }
    return(TWI_STATUS);
}

uint8_t twi_sendAddress(uint8_t i2cAddress) {
    TWDR = i2cAddress;
    unsigned long startingTime = _time;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT))) {
        if(!timeOutDelay){continue;}
        if((_time - startingTime) >= timeOutDelay) {
            twi_lockUp();
            return(1);
        }

    }
    if ((TWI_STATUS == MT_SLA_ACK) || (TWI_STATUS == MR_SLA_ACK)) {
        return(0);
    }
    uint8_t bufferedStatus = TWI_STATUS;
    if ((TWI_STATUS == MT_SLA_NACK) || (TWI_STATUS == MR_SLA_NACK)) {
        twi_stop();
        return(bufferedStatus);
    } else {
        twi_lockUp();
        return(bufferedStatus);
    }
}

uint8_t twi_sendByte(uint8_t i2cData) {
    TWDR = i2cData;
    unsigned long startingTime = _time;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT))) {
        if(!timeOutDelay){continue;}
        if((_time - startingTime) >= timeOutDelay) {
            twi_lockUp();
            return(1);
        }

    }
    if (TWI_STATUS == MT_DATA_ACK) {
        return(0);
    }
    uint8_t bufferedStatus = TWI_STATUS;
    if (TWI_STATUS == MT_DATA_NACK) {
        twi_stop();
        return(bufferedStatus);
    } else {
        twi_lockUp();
        return(bufferedStatus);
    }
}

uint8_t twi_receiveByte(uint8_t ack) {
    unsigned long startingTime = _time;
    if(ack) {
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

    } else {
        TWCR = (1<<TWINT) | (1<<TWEN);
    }
    while (!(TWCR & (1<<TWINT))) {
        if(!timeOutDelay){continue;}
        if((_time - startingTime) >= timeOutDelay) {
            twi_lockUp();
            return(1);
        }
    }
    if (TWI_STATUS == LOST_ARBTRTN) {
        uint8_t bufferedStatus = TWI_STATUS;
        twi_lockUp();
        return(bufferedStatus);
    }
    return(TWI_STATUS);
}

uint8_t twi_stop(void) {
    unsigned long startingTime = _time;
    TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
    while ((TWCR & (1<<TWSTO))) {
        if(!timeOutDelay){continue;}
        if((_time - startingTime) >= timeOutDelay) {
            twi_lockUp();
            return(1);
        }

    }
    return(0);
}

void twi_lockUp(void) {
    TWCR = 0; //releases SDA and SCL lines to high impedance
    TWCR = _BV(TWEN) | _BV(TWEA); //reinitialize TWI
}
