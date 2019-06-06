#include "bmp280_i2c.h"
#include "i2c_avr.h"
#include <util/delay.h>


typedef int32_t BMP280_S32_t;
typedef uint32_t BMP280_U32_t;

uint8_t BMP280_dev_addr;


//float BMP280_LP; //	Расчётное давление на уровне моря в мм.рт.ст.
int32_t t_fine;

uint16_t dig_T1;
int16_t  dig_T2;
int16_t  dig_T3;

uint16_t dig_P1;
int16_t  dig_P2;
int16_t  dig_P3;
int16_t  dig_P4;
int16_t  dig_P5;
int16_t  dig_P6;
int16_t  dig_P7;
int16_t  dig_P8;
int16_t  dig_P9;

uint8_t BMP280_ErrData = 0;   //	Флаг ошибки чтения данных

void BMP280_write8(uint8_t address, uint8_t data)
{
	I2CWriteRegister(BMP280_dev_addr, address, data);
}

uint8_t BMP280_read8(uint8_t address)
{
	uint8_t BMP280_buffer;
	I2CReadBytes(BMP280_dev_addr, address, 1, &BMP280_buffer);
   return BMP280_buffer;
}

uint16_t BMP280_read16(uint8_t address)
{
	uint8_t BMP280_buffer[2];
  I2CReadBytes(BMP280_dev_addr, address, 2, BMP280_buffer);
  return ((uint16_t) ((BMP280_buffer[0]<<8)|BMP280_buffer[1]));    
}

uint16_t BMP280_read16_LE(uint8_t reg) {
  uint16_t temp = BMP280_read16(reg);
  return (temp >> 8) | (temp << 8);

}

int16_t BMP280_readS16_LE(uint8_t reg)
{
  return (int16_t)BMP280_read16_LE(reg);
}


uint32_t BMP280_read24(uint8_t address)
{
  uint32_t ret_value;
  uint8_t BMP280_buffer[3];
  I2CReadBytes(BMP280_dev_addr, address, 3, BMP280_buffer);
  //ret_value = (uint32_t) ((BMP280_buffer[0]<<16)|(BMP280_buffer[1]<<8)|BMP280_buffer[2]);
  ret_value = BMP280_buffer[0];
  ret_value <<=8;
  ret_value |= BMP280_buffer[1];
  ret_value <<=8;
  ret_value |= BMP280_buffer[2];
  return ret_value;
}

void BMP280_readCoefficients()
{
    dig_T1 = BMP280_read16_LE(BMP280_REGISTER_DIG_T1);
    dig_T2 = BMP280_readS16_LE(BMP280_REGISTER_DIG_T2);
    dig_T3 = BMP280_readS16_LE(BMP280_REGISTER_DIG_T3);

    dig_P1 = BMP280_read16_LE(BMP280_REGISTER_DIG_P1);
    dig_P2 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P2);
    dig_P3 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P3);
    dig_P4 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P4);
    dig_P5 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P5);
    dig_P6 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P6);
    dig_P7 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P7);
    dig_P8 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P8);
    dig_P9 = BMP280_readS16_LE(BMP280_REGISTER_DIG_P9);
    /*printf("\ndig_T1 = %i\n", dig_T1);
    printf("dig_T2 = %i\n", dig_T2);
    printf("dig_T3 = %i\n", dig_T3);
    printf("dig_P1 = %i\n", dig_P1);
    printf("dig_P2 = %i\n", dig_P2);
    printf("dig_P3 = %i\n", dig_P3);
    printf("dig_P4 = %i\n", dig_P4);
    printf("dig_P5 = %i\n", dig_P5);
    printf("dig_P6 = %i\n", dig_P6);
    printf("dig_P7 = %i\n", dig_P7);
    printf("dig_P8 = %i\n", dig_P8);
    printf("dig_P9 = %i\n", dig_P9);*/
}

uint8_t BMP280_begin(uint8_t devaddr) 
{
  BMP280_dev_addr  = devaddr;
  if (BMP280_read8(BMP280_REGISTER_CHIPID) != BMP280_CHIPID_VALUE) return 0;
  BMP280_readCoefficients();
  
  BMP280_write8(BMP280_REGISTER_CONFIG, BMP280_CONFIG);
  //printf("bmp280_config_before = %2X\n", BMP280_CONFIG);
  uint8_t icfg = BMP280_read8(BMP280_REGISTER_CONFIG);
  //printf("bmp280_config_after = %2X\n", icfg);
  //BMP280_write8(BMP280_REGISTER_CONTROL, BMP280_MEAS); 
 
  return 1;
}    


int32_t BMP280_readTemperature()
{
  int32_t var1, var2;

  int32_t adc_T;
  int32_t T;
  
  adc_T = BMP280_read24(BMP280_REGISTER_TEMPDATA);
  //printf("adc_T = %ld\n", adc_T);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)dig_T1 <<1))) *
       ((int32_t)dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)dig_T1)) *
         ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) *
       ((int32_t)dig_T3)) >> 14;

  t_fine = var1 + var2;

  T  = (t_fine * 5 + 128) >> 8;
  return T;
}

uint32_t BMP280_readPressure() {
  int32_t var1, var2;
  uint32_t p;
  int32_t adc_P;
  
  // Must be done first to get the t_fine variable set up
  BMP280_readTemperature();

  adc_P = BMP280_read24(BMP280_REGISTER_PRESSUREDATA);
  //printf("adc_P = %ld\n", adc_P);
  adc_P >>= 4;
            
    var1 = (((BMP280_S32_t)t_fine)>>1)-(BMP280_S32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BMP280_S32_t)dig_P6);
    var2 = var2 + ((var1*((BMP280_S32_t)dig_P5))<<1);
    var2 = (var2>>2)+(((BMP280_S32_t)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BMP280_S32_t)dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((BMP280_S32_t)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = (((BMP280_U32_t)(((BMP280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((BMP280_U32_t)var1);
    }
    else
    {
        p = (p / (BMP280_U32_t)var1) * 2;
    }
    var1 = (((BMP280_S32_t)dig_P9) * ((BMP280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((BMP280_S32_t)(p>>2)) * ((BMP280_S32_t)dig_P8))>>13;
    p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));
    return p;
}

void BMP280_readPressure_float(int32_t *temperature,float *pressure)
{
    float var1, var2;
    int32_t adc_P;
    
    *temperature = BMP280_readTemperature();

    adc_P = BMP280_read24(BMP280_REGISTER_PRESSUREDATA);
    //printf("adc_P = %ld\n", adc_P);
    adc_P >>= 4;
    
    var1 = ((float)t_fine/2.0)-64000.0;
    var2 = var1 * var1 * ((float)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((float)dig_P5) * 2.0;
    var2 = (var2/4.0)+(((float)dig_P4) * 65536.0);
    var1 = (((float)dig_P3) * var1 * var1 / 524288.0 + ((float)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*((float)dig_P1);
    if (var1 == 0.0)
    {    
        *pressure = 0;
        return; // avoid exception caused by division by zero
    }
    *pressure = 1048576.0-(float)adc_P;
    *pressure = (*pressure-(var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((float)dig_P9) * (*pressure) * (*pressure) / 2147483648.0;
    var2 = *pressure * ((float)dig_P8) / 32768.0;
    *pressure = *pressure + (var1 + var2 + ((float)dig_P7)) / 16.0;
    return;
}

float BMP280_altitude(float *pressure)
{
    float temp;
    float  altitude;
    temp = (float) *pressure/101325;
    temp = 1-pow(temp, 0.19029);
    //altitude = round(44330*temp*10);
    altitude = 44330*temp;
    return altitude;
}

uint8_t BMP280_receiveData(int32_t *temperature,float *pressure,float *alt)
{   
    BMP280_write8(BMP280_REGISTER_CONTROL, BMP280_MEAS); 
    _delay_ms(44);
    BMP280_readPressure_float(temperature,pressure);
    *alt = BMP280_altitude(pressure);
    return 1;
}
