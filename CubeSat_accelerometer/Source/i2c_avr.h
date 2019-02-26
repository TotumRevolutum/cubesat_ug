#ifndef I2CAVR_H_
#define I2CAVR_H_

#define TRUE 1
#define FALSE 0
#include <stdint.h>


void I2CInit(uint16_t CPU_F,  uint16_t SCL_F);

void I2CClose();

void I2CStart();
void I2CStop();

uint8_t I2CWriteByte(uint8_t data);
uint8_t I2CReadByte(uint8_t *data,uint8_t ack);

void I2CWriteRegister(uint8_t addr, uint8_t reg, uint8_t value);
void I2CReadBytes(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t *buffer);
//int32_t I2CgetRegister(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t unsign);

#endif /* I2CAVR_H_ */
