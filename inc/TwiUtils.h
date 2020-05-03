#ifndef TWIUTILS_H_
#define TWIUTILS_H_

#include "twi.h"
#include "usart.h"

bool TwiWriteTo(uint8_t addr, uint8_t* buff, uint8_t len);
bool TwiWriteToReg(uint8_t addr, uint8_t regAddr, uint8_t* buff, uint8_t len);
bool TwiWriteToReg(uint8_t addr, uint8_t regAddr, uint8_t val);
bool TwiSelectReg(uint8_t addr, uint8_t regAddr);
bool TwiReadFrom(uint8_t addr, uint8_t* recvBuff, uint8_t len);
bool TwiReadFromReg(uint8_t addr, uint8_t regAddr, uint8_t* recvBuff, uint8_t len);
void TwiReportState();

#endif /* TWIUTILS_H_ */