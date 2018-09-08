#ifndef __ACCGYRO_MPU_H
#define __ACCGYRO_MPU_H

#include <stdint.h>
#include <stdbool.h>

#define MPU_RA_GYRO_XOUT_H      0x43
#define MPU_RA_PWR_MGMT_1       0x6B
#define MPU_RA_WHO_AM_I         0x75

typedef enum {
	MPU_NONE,
	MPU_60x0,
	MPU_9250_SPI
}detectedMPUSensor_e;

typedef enum {
	MPU_HALF_RESOLUTION,
	MPU_FULL_RESOLUTION
}mpu6050Resolution_e;

typedef struct mpuDetectionResult_s {
	detectedMPUSensor_e sensor;
	mpu6050Resolution_e resolution;
}mpuDetectionResult_t;

typedef bool (*mpuReadRegisterFunc)(uint8_t reg, uint8_t length, uint8_t *data);
typedef bool (*mpuWriteRegisterFunc)(uint8_t reg, uint8_t data);
typedef void (*mpuResetFuncPtr)(void);

extern mpuResetFuncPtr mpuReset;

typedef struct mpuConfiguration_s {
	mpuReadRegisterFunc read;
	mpuWriteRegisterFunc write;
	mpuReadRegisterFunc slowRead;
	mpuWriteRegisterFunc verifyWrite;
	mpuResetFuncPtr reset;
	uint8_t gyroReadXRegister;			// Y and Z must registers follow this, 2 words each
}mpuConfiguration_t;

struct gyroDev_s;
mpuDetectionResult_t *mpuDetect(struct gyroDev_s *gyro);

#endif	// __ACCGYRO_MPU_H
