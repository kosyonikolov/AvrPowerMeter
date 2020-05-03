#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <util/twi.h>
#include "typedefs.h"

#define TwiWaitInt() while (!(TWCR & (1<<TWINT)))
#define TwiStatus() TWSR & 0xF8

#define TWI_ACTION_START 0
#define TWI_ACTION_WRITE 1
#define TWI_ACTION_READ_ACK 2
#define TWI_ACTION_READ_NACK 3

#define W 0
#define R 1

extern uint8_t lastTwiAction;
extern uint8_t lastTwiStatus;

void TwiInit();
bool TwiStart();
void TwiStop();
void TwiWrite(uint8_t data);
uint8_t TwiReadAck();
uint8_t TwiReadNack();

#endif /* TWI_H_ */