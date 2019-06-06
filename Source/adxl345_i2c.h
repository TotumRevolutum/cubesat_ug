#ifndef ADXL345_I2C_H_
#define ADXL345_I2C_H_
#include <stdint.h>

#define ADXL345_DEFAULT_ADDRESS 0x1d//Нога SDO на 1
#define ADXL345_ALTERNATIVE_ADDRESS 0x53 //Если ногу SDO притянуть к 0, по умолчанию на GY91 она притянута на GND

/** Who_am_i register */
#define ADXL345_DEVID_REG 0x00
/** Device id */
#define ADXL345_DEVICE 0xe5


#define ADXL345_BW_RATE_REG 0x2C
#define ADXL345_BW_RATE_100 0b1010 //100HZ
#define ADXL345_BW_RATE_50 0b1001 //50HZ
#define ADXL345_BW_RATE_25 0b1000 //25HZ

/** Tap Threshold */
#define ADXL345_THRESH_TAP_REG 0x1D
/** Tap Duration */
#define ADXL345_DUR_REG 0x21
/** Tap Latency */
#define ADXL345_LATENT_REG 0x22
/** Tap Window */
#define ADXL345_WINDOW_REG 0x23
/** interrupt MAP */
#define ADXL345_INT_MAP_REG 0x2F
/** interrupt Enable */
#define ADXL345_INT_ENABLE_REG 0x2E
/** Power-saving features control */
#define ADXL345_POWER_CTL_REG 0x2D
/** Source of Single Tap/Double Tap */
#define ADXL345_TAP_STATUS_REG 0x2B
/** Axis Control for Single Tap/Double Tap */
#define ADXL345_TAP_AXES_REG 0x2A
/** Source of Interrupts */
#define ADXL345_INT_SOURCE_REG 0x30
/** Data Format Control */
#define ADXL345_DATA_FORMAT_REG 0x31

/** SELF Test ON */
#define ADXL345_SELF_TEST_ON 0b10000000
/** SELF Test OFF */
#define ADXL345_SELF_TEST_OFF 0b00000000
/** SELF SPI ON */
#define ADXL345_SPI_ON 0b01000000
/** SELF SPI OFF */
#define ADXL345_SPI_OFF 0b00000000
/** INT_INVERT ON */
#define ADXL345_INT_INVERT_ON 0b00100000
/** INT_INVERT OFF */
#define ADXL345_INT_INVERT_OFF 0b00000000
/** FULL_RES ON */
#define ADXL345_FULL_RES_ON 0b00001000
/** FULL_RES OFF */
#define ADXL345_FULL_RES_OFF 0b00000000
/** JUSTIFY ON */
#define ADXL345_JUSTIFY_ON 0b00000100
/** JUSTIFY OFF */
#define ADXL345_JUSTIFY_OFF 0b00000000

/** Axis Tap Control Z axis ON */
#define ADXL345_TAP_AXES_Z_ON 0b00000001
/** Axis Tap Control Y axis ON */
#define ADXL345_TAP_AXES_Y_ON 0b00000010
/** Axis Tap Control X axis ON */
#define ADXL345_TAP_AXES_X_ON 0b00000100

/** Axis Interrupt Single Tap */
#define ADXL345_INT_SINGLE_TAP 0b01000000
/** Axis Interrupt Double Tap */
#define ADXL345_INT_DOUBLE_TAP 0b00100000

/** RANGE 2G */
#define ADXL345_RANGE_2G 0b00
/** RANGE 4G */
#define ADXL345_RANGE_4G 0b01
/** RANGE 8G */
#define ADXL345_RANGE_8G 0b10
/** RANGE 16G */
#define ADXL345_RANGE_16G 0b11

/** AUTO SLEEP ON */
#define ADXL345_AUTO_SLEEP_ON 0b00010000
/** AUTO SLEEP OFF */
#define ADXL345_AUTO_SLEEP_OFF 0b00000000
/** AUTO MEASURE ON */
#define ADXL345_MEASURE_ON 0b00001000
/** AUTO MEASURE OFF */
#define ADXL345_MEASURE_OFF 0b00000000
/** SLEEP ON */
#define ADXL345_SLEEP_ON 0b00000100
/** SLEEP OFF */
#define ADXL345_SLEEP_OFF 0b00000000

/** WAKEUP 8Hz */
#define ADXL345_WAKEUP_8HZ 0b00
/** WAKEUP 4Hz */
#define ADXL345_WAKEUP_4HZ 0b01
/** WAKEUP 2Hz */
#define ADXL345_WAKEUP_2HZ 0b10
/** WAKEUP 1Hz */
#define ADXL345_WAKEUP_1HZ 0b11


void ADXL345_readXYZ_raw(int *x, int *y, int *z);
void ADXL345_readXYZ(float *x, float *y, float *z);

uint8_t ADXL345_begin(uint8_t devaddr);

#endif /* ADXL345_I2C_H_ */

