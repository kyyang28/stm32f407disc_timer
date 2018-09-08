
#include <stdint.h>
#include <stdbool.h>
#include "accgyro.h"		// accgyro_mpu.h is included
#include "accgyro_spi_mpu9250.h"
#include "bus_i2c.h"
#include "system.h"
#include "target.h"
#include "utils.h"

#ifndef MPU_I2C_INSTANCE
#define MPU_I2C_INSTANCE 			I2C_DEVICE				// I2C_DEVICE = I2CDEV_2 = 1 using I2C2 (SCL: PB10, SDA: PB11)
#endif

#define MPU_ADDRESS             	0x68

/* Debugging purposes */
#include "stdio.h"

mpuResetFuncPtr mpuReset;

static bool mpuReadRegisterI2C(uint8_t reg, uint8_t length, uint8_t *data);

#ifdef USE_SPI
static bool detectSPISensorsAndUpdateDetectionResult(gyroDev_t *gyro)
{
#ifdef USE_GYRO_SPI_MPU9250
//	printf("%s, %d\r\n", __FUNCTION__, __LINE__);
	if (mpu9250SpiDetect()) {
//		printf("%s, %d\r\n", __FUNCTION__, __LINE__);
		gyro->mpuDetectionResult.sensor = MPU_9250_SPI;
		gyro->mpuConfiguration.gyroReadXRegister = MPU_RA_GYRO_XOUT_H;
//		gyro->mpuConfiguration.read = mpu9250ReadRegister;
//		gyro->mpuConfiguration.slowRead = mpu9250SlowReadRegister;
//		gyro->mpuConfiguration.verifyWrite = verifympu9250WriteRegister;
//		gyro->mpuConfiguration.write = mpu9250WriteRegister;
//		gyro->mpuConfiguration.reset = mpu9250ResetGyro;
		return true;
	}
#endif
	
//	printf("%s, %d\r\n", __FUNCTION__, __LINE__);
	
	UNUSED(gyro);
	return false;
}
#endif

mpuDetectionResult_t *mpuDetect(gyroDev_t *gyro)
{
	bool ack;
	uint8_t sig;
	
	/* Chapter 3.1 Table 1, Gyroscope Startup Time: TYP 35 ms from <MPU9250REV1.0_ProductSpecification.pdf> */
	delay(35);

	ack = mpuReadRegisterI2C(MPU_RA_WHO_AM_I, 1, &sig);
	if (ack) {
		printf("MPU6050 is detected!\r\n");
	}else {
		printf("Failed to detect MPU6050!\r\n");
	}
	
//#ifdef USE_SPI
////	printf("%s, %d\r\n", __FUNCTION__, __LINE__);
//	bool detectedSpiSensor = detectSPISensorsAndUpdateDetectionResult(gyro);
////	printf("%s, %d\r\n", __FUNCTION__, __LINE__);
//	UNUSED(detectedSpiSensor);
//#endif
	
	return &gyro->mpuDetectionResult;
}

static bool mpuReadRegisterI2C(uint8_t reg, uint8_t length, uint8_t *data)
{
	bool ack = i2cRead(MPU_I2C_INSTANCE, MPU_ADDRESS, reg, length, data);
	return ack;
}
