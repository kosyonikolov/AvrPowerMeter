#ifndef MPU6050_H_
#define MPU6050_H_

#include "TwiUtils.h"

#define MPU_ADDR 0x68

#define SMPLRT_DIV 0x19
// Sample rate = 8[1 DLFP] KHz / (1+SMPLRT_DIV)
#define CONFIG 0x1A
// Bits 2-0 : DLPF;
#define GYRO_CONFIG 0x1B
// FS_SEL: [4;3]
#define GYRO_RANGE_250 0
#define GYRO_RANGE_500 1
#define GYRO_RANGE_1000 2
#define GYRO_RANGE_2000 3

#define ACCEL_CONFIG 0x1C
// FS_SEL: [4;3]
#define ACC_RANGE_2G 0
#define ACC_RANGE_4G 1
#define ACC_RANGE_8G 2
#define ACC_RANGE_16G 3

#define ACC_XH 0x3B
#define ACC_XL 0x3C
#define ACC_YH 0x3D
#define ACC_YL 0x3E
#define ACC_ZH 0x3F
#define ACC_ZL 0x40

#define TEMPH 0x41
#define TEMPL 0x42

#define GYRO_XH 0x43
#define GYRO_XL 0x44
#define GYRO_YH 0x45
#define GYRO_YL 0x46
#define GYRO_ZH 0x47
#define GYRO_ZL 0x48

#define PWR_MGMT_1 0x6B 
// [RESET] [SLEEP] [CYCLE] [-] [TEMP_DIS] [CLKSEL - 3]
// CLKSEL: 0 - internal; [1;3] - gyro
#define PWR_MGMT_2 0x6C

class Mpu6050
{
	private:
		bool ad0;
		uint8_t addr;

	public:
		Mpu6050(bool ad0);
		bool Init(ubyte gyroRange, ubyte accRange);
		bool GetAccelerometer(ubyte* buffer);
		bool GetAccelerometer(int* ax, int* ay, int* az);
		bool GetGyroscope(ubyte* buffer);
		bool GetGyroscope(int* gx, int* gy, int* gz);
		bool GetTemperature(uint8_t * outBuffer);
};

#endif /* MPU6050_H_ */