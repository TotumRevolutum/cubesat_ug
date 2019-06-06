#ifndef BMP280_I2C_H_
#define BMP280_I2C_H_

#include <stdint.h>

#define BMP280_DEFAULT_ADDRESS 0x77//Нога SDO на 1
#define BMP280_ALTERNATIVE_ADDRESS 0x76 //Если ногу SDO притянуть к 0, по умолчанию на GY91 она притянута на GND

#define BMP280_REGISTER_DIG_T1 0x88
#define BMP280_REGISTER_DIG_T2 0x8A
#define BMP280_REGISTER_DIG_T3 0x8C

#define BMP280_REGISTER_DIG_P1 0x8E
#define BMP280_REGISTER_DIG_P2 0x90
#define BMP280_REGISTER_DIG_P3 0x92
#define BMP280_REGISTER_DIG_P4 0x94
#define BMP280_REGISTER_DIG_P5 0x96
#define BMP280_REGISTER_DIG_P6 0x98
#define BMP280_REGISTER_DIG_P7 0x9A
#define BMP280_REGISTER_DIG_P8 0x9C
#define BMP280_REGISTER_DIG_P9 0x9E

#define BMP280_REGISTER_CHIPID  0xD0
#define BMP280_CHIPID_VALUE  0x58
#define BMP280_REGISTER_VERSION 0xD1
#define BMP280_REGISTER_SOFTRESET 0xE0

#define BMP280_REGISTER_STATUS 0xF3
#define BMP280_REGISTER_CONTROL 0xF4
#define BMP280_REGISTER_CONFIG  0xF5
#define BMP280_REGISTER_PRESSUREDATA 0xF7
#define BMP280_REGISTER_TEMPDATA     0xFA

#define    BMP280_OVERSAMPLING_T1        0b1
#define    BMP280_OVERSAMPLING_T2        0b10
#define    BMP280_OVERSAMPLING_T4        0b11
#define    BMP280_OVERSAMPLING_T8        0b100
#define    BMP280_OVERSAMPLING_T16        0b101

#define    BMP280_OVERSAMPLING_P1        0b1
#define    BMP280_OVERSAMPLING_P2        0b10
#define    BMP280_OVERSAMPLING_P4        0b11
#define    BMP280_OVERSAMPLING_P8        0b100
#define    BMP280_OVERSAMPLING_P16        0b101

#define    BMP280_MODE_SLEEP            0b0
#define    BMP280_MODE_FORCED            0b01
#define    BMP280_MODE_NORMAL            0b11


#define    BMP280_TSB_0_5                0x00      //3.6.3 datasheet
#define    BMP280_TSB_62_5                0b1
#define    BMP280_TSB_125                0b10
#define    BMP280_TSB_250                0b11
#define    BMP280_TSB_500                0b100
#define    BMP280_TSB_1000                0b101
#define    BMP280_TSB_2000                0b110
#define    BMP280_TSB_4000                0b111

#define    BMP280_FILTER_OFF            0b00      //3.3.3 datasheet
#define    BMP280_FILTER_COEFFICIENT2    0b1
#define    BMP280_FILTER_COEFFICIENT4    0b10
#define    BMP280_FILTER_COEFFICIENT8    0b11
#define    BMP280_FILTER_COEFFICIENT16	0b100

#define	BMP280_SPI_OFF	0b00
#define	BMP280_SPI_ON	0b01


#define	BMP280_MEAS			(BMP280_OVERSAMPLING_T2<<5)|(BMP280_OVERSAMPLING_P16<<2)|(BMP280_MODE_FORCED)
#define	BMP280_CONFIG		(BMP280_TSB_250<<5) | (BMP280_FILTER_COEFFICIENT4<<2)|(BMP280_SPI_OFF)

uint8_t BMP280_begin(uint8_t devaddr);
uint8_t BMP280_receiveData(int32_t *temperature, float *pressure,float *alt);
uint8_t BMP280_read8(uint8_t address);

#endif /* BMP280_I2C_H_ */
