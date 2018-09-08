
#include "accgyro_mpu.h"
#include "accgyro_spi_mpu9250.h"
#include "target.h"
#include "io.h"
#include "bus_spi.h"
#include "system.h"
#include "stdio.h"

static IO_t mpuSpi9250CsPin = IO_NONE;

#define ENABLE_MPU9250			IOLo(mpuSpi9250CsPin);
#define DISABLE_MPU9250			IOHi(mpuSpi9250CsPin);

bool mpu9250WriteRegister(uint8_t reg, uint8_t data)
{
	ENABLE_MPU9250;				// CS low
	delayMicroseconds(1);
	spiTransferByte(MPU9250_SPI_INSTANCE, reg);
	spiTransferByte(MPU9250_SPI_INSTANCE, data);
	DISABLE_MPU9250;			// CS high
	delayMicroseconds(1);
	return true;
}

bool mpu9250ReadRegister(uint8_t reg, uint8_t length, uint8_t *data)
{
	ENABLE_MPU9250;
	spiTransferByte(MPU9250_SPI_INSTANCE, reg | 0x80);		// read transaction
	spiTransfer(MPU9250_SPI_INSTANCE, data, NULL, length);
	DISABLE_MPU9250;
	return true;
}

bool mpu9250SpiDetect(void)
{
	uint8_t in;
	uint8_t attemptsRemaining = 20;
//	float cnt = 0.0;
	
#ifdef MPU9250_CS_PIN
	mpuSpi9250CsPin = IOGetByTag(IO_TAG(MPU9250_CS_PIN));
//	printf("%s, %d\r\n", __FUNCTION__, __LINE__);
#endif
//	ioRec_t *ioRecMPUSpi9250Cs = IO_Rec(mpuSpi9250CsPin);
//	printf("ioRecMPUSpi9250Cs->gpio: 0x%x\r\n", (uint32_t)ioRecMPUSpi9250Cs->gpio);		// gpio = 0x40021000 (GPIOE)
//	printf("ioRecMPUSpi9250Cs->pin: %u\r\n", ioRecMPUSpi9250Cs->pin);					// pin = 16 = 0x10 (1 << 4)
	
	IOInit(mpuSpi9250CsPin, OWNER_MPU_CS, 0);
	IOConfigGPIO(mpuSpi9250CsPin, SPI_IO_CS_CFG);
	
//	printf("ioRecMPUSpi9250Cs->owner: %u\r\n", ioRecMPUSpi9250Cs->owner);				// owner = 11
//	printf("ioRecMPUSpi9250Cs->index: %u\r\n", ioRecMPUSpi9250Cs->index);				// index = 0	
	
	spiSetDivisor(MPU9250_SPI_INSTANCE, SPI_CLOCK_INITIALISATION);		// low speed
	
	mpu9250WriteRegister(MPU_RA_PWR_MGMT_1, MPU9250_BIT_RESET);
	delay(100);
//	
	mpu9250WriteRegister(0x68, 0x07);
	delay(100);
//	
	mpu9250WriteRegister(0x6A, 0x10);
//	delay(100);	
	
//	mpu9250ReadRegister(MPU_RA_PWR_MGMT_1, 1, &in);
//	printf("MPU_RA_PWR_MGMT_1 reg: %u\r\n", in);
	
	do {
		delay(150);
		
		mpu9250ReadRegister(MPU_RA_WHO_AM_I, 1, &in);
		printf("WHO_AM_I reg: 0x%x\r\n", in);
//		cnt += 0.1;
//		printf("cnt: %.4f\r\n", cnt);
		if (in == MPU9250_WHO_AM_I_CONST) {
			break;
		}
		
		if (!attemptsRemaining) {
			return false;
		}
	} while (attemptsRemaining--);
	
	/* MPU 9250 found, set the SPI clock to 21.00000 MHz  */
	spiSetDivisor(MPU9250_SPI_INSTANCE, SPI_CLOCK_FAST);
	
	return true;
}
