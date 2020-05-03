#include "TwiUtils.h"

bool TwiWriteTo(uint8_t addr, uint8_t *buff, uint8_t len)
{
    if (!TwiStart())
        return false;

    TwiWrite(addr << 1); // ADDR + W
    if (lastTwiStatus != TW_MT_SLA_ACK)
        return false;

    for (int i = 0; i < len; i++)
    {
        TwiWrite(buff[i]);
        if (lastTwiStatus != TW_MT_DATA_ACK)
            return false;
    }

    TwiStop();
    return true;
}

bool TwiWriteToReg(uint8_t addr, uint8_t regAddr, uint8_t *buff, uint8_t len)
{
    if (!TwiStart())
        return false;

    TwiWrite(addr << 1); // ADDR + W
    if (lastTwiStatus != TW_MT_SLA_ACK)
        return false;

    TwiWrite(regAddr);
    if (lastTwiStatus != TW_MT_DATA_ACK)
        return false;

    for (int i = 0; i < len; i++)
    {
        TwiWrite(buff[i]);
        if (lastTwiStatus != TW_MT_DATA_ACK)
            return false;
    }

    TwiStop();
    return true;
}

bool TwiWriteToReg(uint8_t addr, uint8_t regAddr, uint8_t val)
{
    if (!TwiStart())
        return false;

    TwiWrite(addr << 1); // ADDR + W
    if (lastTwiStatus != TW_MT_SLA_ACK)
        return false;

    TwiWrite(regAddr);
    if (lastTwiStatus != TW_MT_DATA_ACK)
        return false;

    TwiWrite(val);
    if (lastTwiStatus != TW_MT_DATA_ACK)
        return false;

    TwiStop();
    return true;
}

bool TwiSelectReg(uint8_t addr, uint8_t regAddr)
{
    if (!TwiStart())
        return false;

    TwiWrite(addr << 1); // ADDR + W
    if (lastTwiStatus != TW_MT_SLA_ACK)
        return false;

    TwiWrite(regAddr);
    if (lastTwiStatus != TW_MT_DATA_ACK)
        return false;

    TwiStop();
    return true;
}

bool TwiReadFrom(uint8_t addr, uint8_t *recvBuff, uint8_t len)
{
    if (!TwiStart())
        return false;

    TwiWrite((addr << 1) | R);
    if (lastTwiStatus != TW_MR_SLA_ACK)
        return false;

    int lim = len - 1;
    for (int i = 0; i < lim; i++)
    {
        recvBuff[i] = TwiReadAck();
        // TODO:
    }
    recvBuff[len - 1] = TwiReadNack();

    TwiStop();
    return true;
}

bool TwiReadFromReg(uint8_t addr, uint8_t regAddr, uint8_t *recvBuff, uint8_t len)
{
    if (!TwiSelectReg(addr, regAddr))
        return false;

    return TwiReadFrom(addr, recvBuff, len);
}

void TwiReportState()
{
    UsartSend(lastTwiAction);
    UsartSend(" \0");
    UsartSend(lastTwiStatus);
}