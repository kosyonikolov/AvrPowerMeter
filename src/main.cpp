#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

#define LED_MASK ((1 << 5) - 1)

int main()
{
    DDRB |= LED_MASK;
    PORTB = 0;

    UsartInit();
    int currentDelay = 1000;
    int newDelay = 0;

    const int minDelay = 50;

    int count = 0;
    while (true)
    {
        PORTB = count & LED_MASK;
        count++;
        if (count > LED_MASK)
        {
            count = 0;
        }

        // Check uart for command
        char inChar;
        while (UsartRead(&inChar))
        {
            UsartSend(inChar);
            if (inChar == '\n')
            {
                if (newDelay < minDelay)
                {
                    UsartSend("Requested delay is smaller than min: ");
                    UsartSend(newDelay);
                    UsartNewLine();
                    newDelay = 0;
                }
                else
                {
                    currentDelay = newDelay;
                    newDelay = 0;
                }
            }
            else if (inChar >= '0' && inChar <= '9')
            {
                newDelay *= 10;
                newDelay += inChar - '0';
            }
            else // invalid character
            {
                newDelay = 0;
            }
        }

        _delay_ms(currentDelay);
    }
}