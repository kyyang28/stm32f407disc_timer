
#include <string.h>
#include "gyro.h"
#include "exti.h"
#include "target.h"

gyro_t gyro;				// gyro access functions

static const gyroConfig_t *gyroConfig;

static const extiConfig_t *selectMPUIntExtiConfig(void)
{
#if defined(MPU_INT_EXTI)
	static const extiConfig_t mpuIntExtiConfig = { .tag = IO_TAG(MPU_INT_EXTI) };
	return &mpuIntExtiConfig;
#endif
}

bool gyroInit(const gyroConfig_t *gyroConfigToUse)
{
	gyroConfig = gyroConfigToUse;
	memset(&gyro, 0, sizeof(gyro));

//#if defined(USE_GYRO_MPU6050)
//#if defined(USE_GYRO_SPI_MPU9250)
	gyro.dev.mpuIntExtiConfig = selectMPUIntExtiConfig();
	mpuDetect(&gyro.dev);
	mpuReset = gyro.dev.mpuConfiguration.reset;
//#endif
	
	return true;
}
