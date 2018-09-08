#ifndef __ACCGYRO_SPI_MPU9250_H
#define __ACCGYRO_SPI_MPU9250_H

#include <stdbool.h>

#define MPU9250_WHO_AM_I_CONST              (0x71)

/* <MPU9250A-00-v1.6_RegisterMap.pdf> p40 PWR_MGMT_1 [7] H_RESET, Write a 1 to set the reset, the bit will auto clear. */
#define MPU9250_BIT_RESET                   (0x80)

bool mpu9250SpiDetect(void);
bool mpu9250WriteRegister(uint8_t reg, uint8_t data);
bool mpu9250ReadRegister(uint8_t reg, uint8_t length, uint8_t *data);

#endif	// __ACCGYRO_SPI_MPU9250_H
