
#include "stm32f4xx_i2c.h"
#include "bus_i2c.h"
#include "utils.h"
#include "io.h"

#include "stdio.h"

// Software I2C driver, using same pins as hardware I2C, with hw i2c module disabled.
// Can be configured for I2C2 pinout (SCL: PB10, SDA: PB11) or I2C1 pinout (SCL: PB6, SDA: PB7)

static IO_t scl;
static IO_t sda;
static volatile uint16_t i2cErrorCount = 0;

#define SCL_H					IOHi(scl)
#define SCL_L					IOLo(scl)

#define SDA_H					IOHi(sda)
#define SDA_L					IOLo(sda)

#define SCL_read				IORead(scl)
#define SDA_read				IORead(sda)

void I2C_delay(void)
//static void I2C_delay(void)
{
	volatile int i = 7;
	while (i) {
		i--;
	}
}

bool I2C_Start(void)
//static bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if (!SDA_read) {
		return false;
	}
	SDA_L;
	I2C_delay();
	if (SDA_read) {
		return false;
	}
	SDA_L;
	I2C_delay();
	return true;
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

static void I2C_Ack(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

static void I2C_NoAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

static bool I2C_WaitAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	if (SDA_read) {
		SCL_L;
		return false;
	}
	SCL_L;
	return true;
}

static void I2C_SendByte(uint8_t byte)
{
	uint8_t i = 8;
	while (i--) {
		SCL_L;
		I2C_delay();
		if (byte & 0x80) {
			SDA_H;
		}else {
			SDA_L;
		}
		byte <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
	}
	SCL_L;
}

static uint8_t I2C_ReceiveByte(void)
{
	uint8_t i = 8;
	uint8_t byte = 0;
	
	SDA_H;
	while (i--) {
		byte <<= 1;
		SCL_L;
		I2C_delay();
		SCL_H;
		I2C_delay();
		if (SDA_read) {
			byte |= 0x01;
		}
	}
	SCL_L;
	return byte;
}

void i2cInit(I2CDevice device)
{
	UNUSED(device);
	
	scl = IOGetByTag(IO_TAG(SOFT_I2C_SCL));
	sda = IOGetByTag(IO_TAG(SOFT_I2C_SDA));
	
	IOConfigGPIO(scl, IOCFG_OUT_OD);
	IOConfigGPIO(sda, IOCFG_OUT_OD);
}

bool i2cRead(I2CDevice device, uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	UNUSED(device);
	
	if (!I2C_Start()) {
		return false;
	}
	I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
	if (!I2C_WaitAck()) {
		I2C_Stop();
		i2cErrorCount++;
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(addr << 1 | I2C_Direction_Receiver);
	I2C_WaitAck();
	while (len) {
		*buf = I2C_ReceiveByte();
		if (len == 1) {
			I2C_NoAck();
		}else {
			I2C_Ack();
		}
		buf++;
		len--;
	}
	I2C_Stop();
	return true;
}

uint16_t i2cGetErrorCounter(void)
{
	return i2cErrorCount;
}