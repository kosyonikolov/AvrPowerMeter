#include "twi.h"

uint8_t lastTwiAction;
uint8_t lastTwiStatus;

void TwiInit()
{
    TWSR = 0;
    TWBR = 72; // 100 kHz @ 16 Hz F_CPU

    DDRB &= ~((1 << 5) | (1 << 4));
    PORTC |= (1 << 5) | (1 << 4); // pull-up

    TWCR = 1 << TWEN;
}

bool TwiStart()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    TwiWaitInt();
    lastTwiAction = TWI_ACTION_START;
    lastTwiStatus = TwiStatus();
    return (lastTwiStatus == TW_START) || (lastTwiStatus == TW_REP_START);
}

void TwiStop()
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TwiWrite(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    TwiWaitInt();
    lastTwiAction = TWI_ACTION_WRITE;
    lastTwiStatus = TwiStatus();
}

uint8_t TwiReadAck()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    TwiWaitInt();
    lastTwiAction = TWI_ACTION_READ_ACK;
    lastTwiStatus = TwiStatus();
    return TWDR;
}

uint8_t TwiReadNack()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    TwiWaitInt();
    lastTwiAction = TWI_ACTION_READ_NACK;
    lastTwiStatus = TwiStatus();
    return TWDR;
}