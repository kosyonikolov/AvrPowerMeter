#ifndef HX_711_H
#define HX_711_H

#include <stdint.h>

enum Hx711Gain
{
    GAIN_32,
    GAIN_64,
    GAIN_128
};

// TODO make it possible to use ports other than port D

class Hx711
{
private:
    uint8_t clockPin;
    uint8_t dataPin;
    uint8_t gainPulses;

public:
    void Init(const uint32_t clockPin, const uint32_t dataPin, const Hx711Gain gain);

    void Read(uint8_t * outputBuffer);
};

#endif