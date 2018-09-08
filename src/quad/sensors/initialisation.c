
#include "initialisation.h"
#include "gyro.h"

bool sensorsAutodetect(const gyroConfig_t *gyroConfig)
{
	/* gyro must be initialised before accelerometer */
	if (!gyroInit(gyroConfig)) {
		return false;
	}
	
	if (gyroConfig->gyro_align != ALIGN_DEFAULT) {
		gyro.dev.gyroAlign = gyroConfig->gyro_align;
	}
	
//    if (accelerometerConfig->acc_align != ALIGN_DEFAULT) {
//        acc.dev.accAlign = accelerometerConfig->acc_align;
//    }
//    if (compassConfig->mag_align != ALIGN_DEFAULT) {
//        mag.dev.magAlign = compassConfig->mag_align;
//    }
	
	return true;
}
