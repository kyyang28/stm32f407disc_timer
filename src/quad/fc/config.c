
#include <string.h>
#include "configMaster.h"
#include "led.h"
#include "ledTimer.h"			// for testing led timer ONLY
#include "motors.h"				// including mixer.h
#include "target.h"
#include "serial.h"
#include "sensor.h"
#include "pwm_output.h"			// including timer.h
#include "feature.h"
//#include "timer.h"
//#include "accgyro.h"
//#include "filter.h"

#define BRUSHED_MOTORS_PWM_RATE 			16000
#define BRUSHLESS_MOTORS_PWM_RATE 			480

/* master config structure with data independent from profiles */
master_t masterConfig;

void ResetSerialPinConfig(serialPinConfig_t *pSerialPinConfig)
{
	for (int port = 0; port < SERIAL_PORT_MAX_INDEX; port++) {
		pSerialPinConfig->ioTagRx[port] = IO_TAG(NONE);
		pSerialPinConfig->ioTagTx[port] = IO_TAG(NONE);
	}
	
	for (int index = 0; index < SERIAL_PORT_COUNT; index++) {
		switch (serialPortIdentifiers[index]) {
			case SERIAL_PORT_USART1:
//				pSerialPinConfig->ioTagRx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART1)] = IO_TAG(UART1_RX_PIN);
//				pSerialPinConfig->ioTagTx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART1)] = IO_TAG(UART1_TX_PIN);
				break;

			case SERIAL_PORT_USART2:
				pSerialPinConfig->ioTagRx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART2)] = IO_TAG(UART2_RX_PIN);
				pSerialPinConfig->ioTagTx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART2)] = IO_TAG(UART2_TX_PIN);
				break;

			case SERIAL_PORT_USART3:
				pSerialPinConfig->ioTagRx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART3)] = IO_TAG(UART3_RX_PIN);
				pSerialPinConfig->ioTagTx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART3)] = IO_TAG(UART3_TX_PIN);
				break;

			case SERIAL_PORT_USART4:
				break;

			case SERIAL_PORT_USART5:
				break;

			case SERIAL_PORT_USART6:
				pSerialPinConfig->ioTagRx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART6)] = IO_TAG(UART6_RX_PIN);
				pSerialPinConfig->ioTagTx[SERIAL_PORT_IDENTIFIER_TO_RESOURCE_INDEX(SERIAL_PORT_USART6)] = IO_TAG(UART6_TX_PIN);
				break;

			case SERIAL_PORT_USART7:
				break;

			case SERIAL_PORT_USART8:
				break;

			case SERIAL_PORT_SOFTSERIAL1:
				break;

			case SERIAL_PORT_SOFTSERIAL2:
				break;
			
			case SERIAL_PORT_USB_VCP:
				break;
			
			case SERIAL_PORT_NONE:
				break;
		}
	}
}

void ResetSerialConfig(serialConfig_t *serialConfig)
{
	memset(serialConfig, 0, sizeof(serialConfig_t));
	serialConfig->serial_update_rate_hz = 100;
	serialConfig->reboot_character		= 'R';
	
	for (int index = 0; index < SERIAL_PORT_COUNT; index++) {
		serialConfig->portConfigs[index].identifier			= serialPortIdentifiers[index];
		serialConfig->portConfigs[index].msp_baudrateIndex	= BAUD_115200;
		serialConfig->portConfigs[index].gps_baudrateIndex	= BAUD_115200;		// gps port for debugging purposes
//		serialConfig->portConfigs[index].gps_baudrateIndex	= BAUD_57600;
		serialConfig->portConfigs[index].blackbox_baudrateIndex	= BAUD_115200;
		serialConfig->portConfigs[index].telemetry_baudrateIndex = BAUD_AUTO;
	}
	
	serialConfig->portConfigs[0].functionMask = FUNCTION_MSP;
}

void ResetLedStatusConfig(LedStatusConfig_t *ledStatusConfig)
{
	for (int i = 0; i < LED_NUMBER; i++) {
		ledStatusConfig->ledTags[i] = IO_TAG_NONE;
	}
	
#ifdef LED3
	ledStatusConfig->ledTags[0] = IO_TAG(LED3);	// LED3 = PD13 ==> DEFIO_TAG__PD13 ==> 4D
#endif

#ifdef LED4
	ledStatusConfig->ledTags[1] = IO_TAG(LED4);	// LED4 = PD12 ==> DEFIO_TAG__PD12 ==> 4C
#endif

#ifdef LED5
	ledStatusConfig->ledTags[2] = IO_TAG(LED5);	// LED5 = PD14 ==> DEFIO_TAG__PD14 ==> 4E
#endif

#ifdef LED6
	ledStatusConfig->ledTags[3] = IO_TAG(LED6);	// LED6 = PD15 ==> DEFIO_TAG__PD15 ==> 4F
#endif
	
	ledStatusConfig->polarity = 0;
}

void ResetLedTimerConfig(LedTimerConfig_t *ledTimerConfig)
{
	int ledTimerIndex = 0;
	for (int i = 0; i < USABLE_TIMER_CHANNEL_COUNT && ledTimerIndex < LED_NUMBER; i++) {
		if (timerHardware[i].usageFlags & TIM_USE_LED) {
			ledTimerConfig->ioTags[ledTimerIndex] = timerHardware[i].tag;
			ledTimerIndex++;
		}
	}
}

void ResetMotorConfig(motorConfig_t *motorConfig)
{
	motorConfig->minthrottle = 1070;
	motorConfig->motorPwmRate = BRUSHLESS_MOTORS_PWM_RATE;		// BRUSHLESS_MOTORS_PWM_RATE = 480
	motorConfig->motorPwmProtocol = PWM_TYPE_STANDARD;
	motorConfig->maxthrottle = 2000;
	motorConfig->mincommand = 1000;
	motorConfig->useUnsyncedPwm = true;
	
	int motorIndex = 0;
	for (int i = 0; i < USABLE_TIMER_CHANNEL_COUNT && motorIndex < MAX_SUPPORTED_MOTORS; i++) {
		if (timerHardware[i].usageFlags & TIM_USE_MOTOR) {
			motorConfig->ioTags[motorIndex] = timerHardware[i].tag;
			motorIndex++;
		}
	}
}

#if defined(USE_PWM)
void ResetPwmConfig(pwmConfig_t *pwmConfig)
{
	int inputIndex = 0;
	for (int i = 0; i < USABLE_TIMER_CHANNEL_COUNT && inputIndex < PWM_INPUT_PORT_COUNT; i++) {
		if (timerHardware[i].usageFlags & TIM_USE_PWM) {
			pwmConfig->ioTags[inputIndex] = timerHardware[i].tag;
			inputIndex++;
		}
	}
}
#endif

void targetConfiguration(master_t *config)
{
	/* USART2 */
	int index = findSerialPortIndexByIdentifier(SERIAL_PORT_USART2);
	config->serialConfig.portConfigs[index].functionMask = FUNCTION_GPS;
	
	/* USART1 */
	index = findSerialPortIndexByIdentifier(SERIAL_PORT_USART1);
	config->serialConfig.portConfigs[index].functionMask = FUNCTION_TELEMETRY_FRSKY;
	
	/* USART6 */
	index = findSerialPortIndexByIdentifier(SERIAL_PORT_USART6);
	config->serialConfig.portConfigs[index].functionMask = FUNCTION_RX_SERIAL;
	
	/* USART3 */
	index = findSerialPortIndexByIdentifier(SERIAL_PORT_USART3);
	config->serialConfig.portConfigs[index].functionMask = FUNCTION_BLACKBOX;
}

void CreateDefaultConfig(master_t *config)
{
	/* Clear all configuration */
	memset(config, 0, sizeof(master_t));
	
	/* Feature configuration */
	uint32_t *featuresPtr = &config->enabledFeatures;
	intFeatureClearAll(featuresPtr);
	intFeatureSet(DEFAULT_RX_FEATURE, featuresPtr);
	
	/* Global settings */
//	config->gyroConfig.gyro_lpf = GYRO_LPF_256HZ;
//	config->gyroConfig.gyro_sync_denom = 4;
//	config->pidConfig.pid_process_denom = 2;
//	config->gyroConfig.gyro_soft_lpf_type = FILTER_PT1;
//	config->gyroConfig.gyro_soft_lpf_hz = 90;
//	config->gyroConfig.gyro_soft_notch_hz_1 = 400;
//	config->gyroConfig.gyro_soft_notch_cutoff_1 = 300;
//	config->gyroConfig.gyro_soft_notch_hz_2 = 200;
//	config->gyroConfig.gyro_soft_notch_cutoff_2 = 100;
//	
//	config->gyroConfig.gyro_align = ALIGN_DEFAULT;
//	config->gyroConfig.gyroMovementCalibrationThreshold = 48;
	
	ResetSerialPinConfig(&config->serialPinConfig);
	ResetSerialConfig(&config->serialConfig);
	ResetLedStatusConfig(&config->ledStatusConfig);
	ResetLedTimerConfig(&config->ledTimerConfig);
	ResetMotorConfig(&config->motorConfig);

#ifdef USE_PWM
	ResetPwmConfig(&config->pwmConfig);
	config->pwmConfig.inputFilteringMode = INPUT_FILTERING_DISABLED;
#endif

	targetConfiguration(config);
}

static void ResetConfig(void)
{
	CreateDefaultConfig(&masterConfig);
}

void ResetEEPROM(void)
{
	ResetConfig();
}

void CheckEEPROMContainsValidData(void)
{
#if 0
	if (isEEPROMContentValid()) {
		return;
	}
#endif
	
	ResetEEPROM();
}
