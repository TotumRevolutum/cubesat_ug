//includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "i2c_avr.h"
#include "adxl345_i2c.h"
#include "bmp280_i2c.h"
#include "1wire_h/onewire.h"
#include "ds18_h/ds18x20.h"
#include "sdcard_raw.h"
#include <stdint.h>

void RP_calculate(float x, float y, float z, float * roll, float * pitch){
  (*roll) = atan2(y, z) * 57.3;
  (*pitch) = atan2((- x) , sqrt(y * y + z * z)) * 57.3;
}

void accelerometer_calibration(float x_raw, float y_raw, float z_raw, float *x, float *y, float *z)
{
	float calibrated_values[3];
	float uncalibrated_values[3] = {x_raw, y_raw, z_raw};
  double calibration_matrix[3][3] =
  {
    {0.986,	0.009,	0.012},
    {-0.005,	1.005,	-0.006},
    {0.01,	-0.001,	1.019}
  };
  double bias[3] =
  {
	-0.111,
	-0.625,
	-0.524
  };
  //calculation
  for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {0, 0, 0};
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
  (*x) = calibrated_values[0];
  (*y) = calibrated_values[1];
  (*z) = calibrated_values[2];
}
