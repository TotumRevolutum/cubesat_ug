#include "adxl345_i2c.h"
#include "i2c_avr.h"
uint8_t ADXL345_dev_addr;

uint8_t ADXL345_searchDevice()
{

  uint8_t device = 0x00;
 
  I2CReadBytes(ADXL345_dev_addr, ADXL345_DEVID_REG, 1, &device);

  if(device == ADXL345_DEVICE){
    return 1;
  } else{
    return 0;
  }
}

void ADXL345_configuration()
{

  uint8_t conf = ADXL345_SELF_TEST_OFF;
  conf |= ADXL345_SPI_OFF;
  conf |= ADXL345_INT_INVERT_OFF;
  conf |= ADXL345_FULL_RES_ON;
  conf |= ADXL345_JUSTIFY_OFF;
  conf |= ADXL345_RANGE_16G;
  //ADXL345_WriteByte(ADXL345_DATA_FORMAT_REG,conf);
  I2CWriteRegister(ADXL345_dev_addr, ADXL345_DATA_FORMAT_REG, conf);
    
}

void ADXL345_powerOn()
{
  uint8_t power = ADXL345_AUTO_SLEEP_OFF;
  power |= ADXL345_MEASURE_ON;
  power |= ADXL345_SLEEP_OFF;
  power |= ADXL345_WAKEUP_8HZ;

  //ADXL345_WriteByte(ADXL345_POWER_CTL_REG,power);
  I2CWriteRegister(ADXL345_dev_addr, ADXL345_POWER_CTL_REG, power);
}

uint8_t ADXL345_begin(uint8_t devaddr) 
{
  ADXL345_dev_addr  = devaddr;
  if (!ADXL345_searchDevice()) return 0;
 
 //ADXL345_WriteByte(ADXL345_BW_RATE_REG,ADXL345_BW_RATE_25);
  I2CWriteRegister(ADXL345_dev_addr, ADXL345_BW_RATE_REG, ADXL345_BW_RATE_25);
  ADXL345_configuration();
  ADXL345_powerOn();

  return 1;
}


void ADXL345_readXYZ_raw(int *x, int *y, int *z)
{
//  uint8_t i=0;
  uint8_t axis_buff[6];

  //ADXL345_ReadBytes(0x32,6,axis_buff);
  I2CReadBytes(ADXL345_dev_addr, 0x32, 6, axis_buff);
  
  *x = (((int)axis_buff[1]) << 8) | axis_buff[0];
  *y = (((int)axis_buff[3]) << 8) | axis_buff[2];
  *z = (((int)axis_buff[5]) << 8) | axis_buff[4];
}

void ADXL345_readXYZ(float *x, float *y, float *z)
{
	int16_t X, Y, Z;
	ADXL345_readXYZ_raw(&X, &Y, &Z);
	*x = X * 0.03828125f;
	*y = Y * 0.03828125f;
	*z = Z * 0.03828125f;
}

