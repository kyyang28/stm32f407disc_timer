#ifndef __CONFIGMASTER_H
#define __CONFIGMASTER_H

#include "led.h"
#include "ledTimer.h"
#include "serial.h"
#include "rx_pwm.h"
#include "platform.h"			// including target.h
#include "motors.h"				// including mixer.h
//#include "gyro.h"

typedef struct master_s {
	uint8_t version;
	uint16_t size;
	uint8_t magic_be;			// magic number, should be 0xBE
	
	uint32_t enabledFeatures;
	
	serialPinConfig_t serialPinConfig;
	serialConfig_t	serialConfig;
//	gyroConfig_t gyroConfig;
	LedStatusConfig_t ledStatusConfig;
	LedTimerConfig_t ledTimerConfig;

	/* motor related stuff */
	motorConfig_t motorConfig;
	
#ifdef USE_PWM
	pwmConfig_t pwmConfig;
#endif	
}master_t;

extern master_t masterConfig;

#define LedStatusConfig(x)					(&masterConfig.ledStatusConfig)
#define LedTimerConfig(x)					(&masterConfig.ledTimerConfig)			// for testing timer led ONLY
#define SerialPinConfig(x) 					(&masterConfig.serialPinConfig)
#define SerialConfig(x)						(&masterConfig.serialConfig)
#define MotorConfig(x)						(&masterConfig.motorConfig)
#define PwmConfig(x)						(&masterConfig.pwmConfig)
//#define GyroConfig(x)						(&masterConfig.gyroConfig)

#endif	// __CONFIGMASTER_H
