#include "Hx711.h"

#include <avr/io.h>
#include <util/delay.h>

void Hx711::Init(const uint32_t clockPin, const uint32_t dataPin, const Hx711Gain gain)
{
    // setup class members
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    
    if (gain == GAIN_32)
    {
        this->gainPulses = 2;
    }
    else if (gain == GAIN_64)
    {
        this->gainPulses = 3;
    }
    else // gain 128
    {
        this->gainPulses = 1;
    }
    
    // config pins
    // clock as output
	DDRD |= (1 << clockPin);
    // data pin is input pullup (tristate)
	DDRD &= ~(1 << dataPin);
	PORTD |= (1 << dataPin);
}

void Hx711::Read(uint8_t * outputBuffer)
{
    // wait for data to be available
    while ((PIND & (1 << dataPin)) != 0);

    // read data, MSB bits first
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            PORTD |= 1 << clockPin;
            outputBuffer[i] <<= 1;
            _delay_us(1);

            outputBuffer[i] |= (PIND & (1 << dataPin)) >> dataPin;
            PORTD &= ~(1 << clockPin);
            _delay_us(1);
        }
    }

    for (int i = 0; i < gainPulses; i++)
    {
        PORTD |= 1 << clockPin;
        _delay_us(1);
        PORTD &= ~(1 << clockPin);
        _delay_us(1);
    }
}