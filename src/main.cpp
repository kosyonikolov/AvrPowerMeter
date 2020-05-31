#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "Mpu6050.h"
#include "Hx711.h"

int main(void)
{
    UsartInit();
    TwiInit();

    Mpu6050 mpu(false);
    Hx711 adcLeft;

    // gain 32 -> use channel A
    adcLeft.Init(3, 2, GAIN_32);

    //dummyCount();

    if (!mpu.Init(GYRO_RANGE_2000, ACC_RANGE_8G))
    {
        UsartSend("Couldn't init, please reset\r\n\0");
    }

    uint8_t adcBytes[3];
    int ax, ay, az;
    int gx, gy, gz;

    int temp;

    while (true)
    {
        if (!mpu.GetGyroscope(&gx, &gy, &gz))
            break;
        if (!mpu.GetAccelerometer(&ax, &ay, &az))
            break;
        if (!mpu.GetTemperature(&temp))
            break;
        /*
		UsartSend(ax); UsartSend('\t');
		UsartSend(ay); UsartSend('\t');
		UsartSend(az); UsartSend('\t');		
		*/

        adcLeft.Read(adcBytes);

        int32_t reading = (uint32_t(adcBytes[0]) << 16) |
                          (uint32_t(adcBytes[1]) << 8) |
                          adcBytes[2];
        if (adcBytes[0] >= 0x80)
        {
            reading |= 0xFFL << 24; // negative number
        }
        
        UsartSend(gx);
        UsartSend('\t');
        UsartSend(gy);
        UsartSend('\t');
        UsartSend(gz);
        UsartSend('\t');
        UsartSend(temp);
        UsartSend('\t');
        UsartSend(reading);

        UsartNewLine();
    }

    UsartSend("Error occured, please reset...\r\n\0");

    while (1);
}