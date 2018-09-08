#ifndef __BUS_I2C_H
#define __BUS_I2C_H

#include <stdint.h>
#include <stdbool.h>

typedef enum I2CDevice {
	I2CINVALID = -1,
	I2CDEV_1 = 0,
	I2CDEV_2,
	I2CDEV_3,
	I2CDEV_COUNT
}I2CDevice;

void I2C_delay(void);
bool I2C_Start(void);
void I2C_Stop(void);
void i2cInit(I2CDevice device);
bool i2cRead(I2CDevice device, uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif	// __BUS_I2C_H
