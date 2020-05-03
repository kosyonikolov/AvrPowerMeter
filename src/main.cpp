#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "Mpu6050.h"

Mpu6050 mpu(false);

int ax, ay, az;
int gx, gy, gz;

int temp;

int main(void)
{
    UsartInit();
    TwiInit();

    if (!mpu.Init(GYRO_RANGE_250, ACC_RANGE_8G))
    {
        UsartSend("Couldn't init, please reset\r\n\0");
    }

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

        UsartSend(gx);
        UsartSend('\t');
        UsartSend(gy);
        UsartSend('\t');
        UsartSend(gz);
        UsartSend('\t');

        /*UsartSend(temp);*/

        UsartNewLine();
    }

    UsartSend("Error occured, please reset...\r\n\0");

    while (1)
        ;
}