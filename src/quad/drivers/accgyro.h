#ifndef __ACCGYRO_H
#define __ACCGYRO_H

#include "exti.h"
#include "axis.h"
#include "sensor.h"
#include "accgyro_mpu.h"

#define GYRO_LPF_256HZ      0
#define GYRO_LPF_188HZ      1
#define GYRO_LPF_98HZ       2
#define GYRO_LPF_42HZ       3
#define GYRO_LPF_20HZ       4
#define GYRO_LPF_10HZ       5
#define GYRO_LPF_5HZ        6
#define GYRO_LPF_NONE       7

typedef enum {
	GYRO_RATE_1_khz,
	GYRO_RATE_8_khz,
	GYRO_RATE_32_khz,
}gyroRateKHz_e;

typedef struct gyroDev_s {
	sensorGyroInitFuncPtr init;								// initialisation function
	sensorGyroReadFuncPtr read;								// read 3 axis data function
	sensorGyroReadDataFuncPtr temperature;					// read temperature if available
	sensorGyroInterruptStatusFuncPtr intStatus;
	sensorGyroUpdateFuncPtr update;
	extiCallbackRec_t exti;
	float scale;
	volatile int16_t gyroADCRaw[XYZ_AXIS_COUNT];
	uint8_t lpf;
	gyroRateKHz_e gyroRateKHz;
	uint8_t mpuDividerDrops;
	volatile bool dataReady;
	sensor_align_e gyroAlign;
	mpuDetectionResult_t mpuDetectionResult;
	const extiConfig_t *mpuIntExtiConfig;
	mpuConfiguration_t mpuConfiguration;
}gyroDev_t;

#endif	// __ACCGYRO_H
