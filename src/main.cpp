#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "Mpu6050.h"
#include "Hx711.h"

#include "SerialPackets.h"

int main(void)
{
    // initialize communication channels
    UsartInit();
    TwiInit();

    // Prepare serial packets 
    uint8_t dataPacket[DATA_PACKET_LEN];
    uint8_t errorPacket[ERROR_PACKET_LEN];

    // set bytes to initial values
    {
        // header & footer
        dataPacket[0] = HEADER_BYTE_1;
        dataPacket[1] = HEADER_BYTE_2;
        dataPacket[DATA_PACKET_LEN - 2] = FOOTER_BYTE_1;
        dataPacket[DATA_PACKET_LEN - 1] = FOOTER_BYTE_2;

        errorPacket[0] = HEADER_BYTE_1;
        errorPacket[1] = HEADER_BYTE_2;
        errorPacket[ERROR_PACKET_LEN - 2] = FOOTER_BYTE_1;
        errorPacket[ERROR_PACKET_LEN - 1] = FOOTER_BYTE_2;

        // length and id (all packets are currently fixed size)
        dataPacket[OFFSET_PACKET_LEN]  = DATA_PACKET_LEN - EXTRA_INFO_LEN;
        dataPacket[OFFSET_PACKET_ID]   = DATA_PACKET_ID;

        errorPacket[OFFSET_PACKET_LEN] = ERROR_PACKET_LEN - EXTRA_INFO_LEN;
        errorPacket[OFFSET_PACKET_ID]  = ERROR_PACKET_ID;

        // set right side force reading to zero - no sensor on the right crankarm yet
        for (int i = 0; i < FORCE_LEN; i++)
        {
            dataPacket[OFFSET_FORCE_RIGHT + i] = 0;
        }
    }

    // init packet points for easy access
    
    Mpu6050 mpu(false);
    Hx711 adcLeft;

    // gain 32 -> use channel A
    adcLeft.Init(3, 2, GAIN_32);

    if (!mpu.Init(GYRO_RANGE_2000, ACC_RANGE_8G))
    {
        // couldn't init the MPU -> indicate problem to user
        // ... by spamming 

        //UsartSend("Couldn't init, please reset\r\n\0");
        errorPacket[OFFSET_ERROR_ID] = ERROR_MPU_INIT;
        while (true)
        {
            UsartSend((char*)errorPacket, ERROR_PACKET_LEN);
        }
    }

    // --------------------------------------------------------------
    // ------------------ Main sensor reading loop ------------------
    // --------------------------------------------------------------

    // start timer 1 (16 bit) for timestamps
    TCNT1 = 0;
    // prescale = 1024 => 1 tick is about 1/8th of a millisecond
    // it will overflow every 8-9 seconds but that doesn't concern us
    // because only the time between packets is important
    TCCR1B = 0b101; 

    // store the reason we exited the main loop
    uint8_t exitError = ERROR_NO_ERROR;

    while (exitError == ERROR_NO_ERROR)
    {
        // timestamp
        uint16_t packetTime = TCNT1;
        dataPacket[OFFSET_TIME]     = packetTime >> 8;
        dataPacket[OFFSET_TIME + 1] = packetTime & 0xFF;

        // data from MPU - gyro & temperature
        if (!mpu.GetGyroscope(dataPacket + OFFSET_GYRO))
        {
            exitError |= ERROR_MPU_READ;
        }
        
        if (!mpu.GetTemperature(dataPacket + OFFSET_TEMPERATURE))
        {
            exitError |= ERROR_MPU_READ;
        }

        // strain gauge data
        // TODO error handling
        adcLeft.Read(dataPacket + OFFSET_FORCE_LEFT);

        UsartSend((char*)dataPacket, DATA_PACKET_LEN);
    }

    UsartSend("Error occured, please reset...\r\n\0");

    while (1);
}