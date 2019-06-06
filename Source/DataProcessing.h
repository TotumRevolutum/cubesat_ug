#ifndef DATAPROCESSING_H_
#define DATAPROCESSING_H_

void RP_calculate(float x, float y, float z, float * roll, float * pitch);
void accelerometer_calibration(float x_raw, float y_raw, float z_raw, float *x, float *y, float *z);
#endif
