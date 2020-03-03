#include "BaseMQ.h"

float mq2_ro = 1.0f;
float mq135_ro = 1.0f;
float mq131L_ro = 1.0f;
float mq131H_ro = 1.0f;

float calculateResistance(int sensorADC){
	float RL_board = MQ2_RL_BOARD;
	if (sensorADC == MQ135pin)		RL_board = MQ135_RL_BOARD;
	if (sensorADC == MQ131pinL)		RL_board = MQ131L_RL_BOARD;
	if (sensorADC == MQ131pinH)		RL_board = MQ131H_RL_BOARD;
  float sensorVoltage = sensorADC * (OPERATING_VOLTAGE / ADC_VALUE_MAX);
  float sensorResistance = (OPERATING_VOLTAGE - sensorVoltage) / sensorVoltage * RL_board;
  return sensorResistance;
}

float readRs(int ADCpin)
{
  float rs=0;
  for (int i=0;i<10;i++) {                                 // take multiple readings and average it.
    rs += calculateResistance(ADC_GetAdcValue(ADCpin));   // rs changes according to gas concentration.
    _delay_ms(50);
  }
  rs = rs/10;
  return rs;
}

float readRatio(int ADCpin){
	if (ADCpin == MQ2pin) 			return readRs(ADCpin) / mq2_ro;
	else if (ADCpin == MQ135pin) 		return readRs(ADCpin) / mq135_ro;
	else if (ADCpin == MQ131pinL) 		return readRs(ADCpin) / mq131L_ro;
	else if (ADCpin == MQ131pinH) 		return readRs(ADCpin) / mq131H_ro;
	else return 0;
}

float readScaled(float a, float b, int ADCpin){
  float ratio = readRatio(ADCpin);
  return exp((log(ratio) - b) / a);
}

void calibrateMQ2() {
  float rs = readRs(MQ2pin);
  float ro = rs / MQ2_RO_IN_CLEAR_AIR;
  mq2_ro = ro;
}

void calibrateMQ135() {
  float rs = readRs(MQ135pin);
  float ro = rs / (exp((log(PPM_CO2_IN_CLEAR_AIR) * -0.42) + 1.92));
  mq135_ro = ro;
}
void calibrateMQ131L() {
  float rs = readRs(MQ131pinL);
  float ro = MQ131_L_R0;
  mq131L_ro = ro;
}
void calibrateMQ131H() {
  float rs = readRs(MQ131pinH);
  float ro = MQ131_H_R0;
  mq131H_ro = ro;
}

unsigned long readLPG() {
  return readScaled(-0.45, 2.95, MQ2pin);
  //-0.4576 2.9258 0.9961
}

unsigned long readMethane() {
  return readScaled(-0.38, 3.21, MQ2pin);
  //-0.3827 3.2106 0.9909
}

unsigned long readSmoke() {
  return readScaled(-0.42, 3.54, MQ2pin);
  //-0.4252 3.456 0.9781
}

unsigned long readHydrogen() {
  return readScaled(-0.48, 3.32, MQ2pin);
  //-0.4811 3.3279 r^2 = 0.9946
}

unsigned long readPropane() {
  return readScaled(-0.4733, 3.0815, MQ2pin);
  //-0.4811 3.3279 r^2 = 0.9946
}
//propane -0.4733 3.0815  0.9987


unsigned long readCO2() {
  return readScaled(-0.42 , 1.92, MQ135pin);
}

float getEnvCorrOzoneRatio(int tempCels, int humidityPercent) {
 	if(humidityPercent == 60 && tempCels == 20) {
 		return 1.0;
 	}
 	// For humidity > 75%, use the 85% curve
 	if(humidityPercent > 75) {
 		// R^2 = 0.9986
 		return -0.0141 * tempCels + 1.5623;
 	}
 	// For humidity > 50%, use the 60% curve
 	if(humidityPercent > 50) {
 		// R^2 = 0.9976
 		return -0.0119 * tempCels + 1.3261;
 	}
 	// Humidity < 50%, use the 30% curve
 	// R^2 = 0.996
 	return -0.0103 * tempCels + 1.1507;
}

unsigned long readOzone_Low(int tempCels) {
//  return readScaled(-0.90, 0.69, MQ131pin);
	// R^2 = 0.9987		Compute the ratio Rs/R0 and apply the environmental correction
//	float ratio = readRatio(MQ131pinL) * getEnvCorrOzoneRatio(tempCels, 0);
//	return 9.4783 * pow(ratio, 2.3348);

	//0.4243 -0.9437
	  return readScaled(0.4243, -0.9437, MQ131pinL);
	//in ppb
}
unsigned long readOzone_High(int tempCels) {
//  return readScaled(-0.90, 0.69, MQ131pin);
	// R^2 = 0.99		Compute the ratio Rs/R0 and apply the environmental correction
//	float ratio = readRatio(MQ131pinH) * getEnvCorrOzoneRatio(tempCels, 0);
//	return 8.1399 * pow(ratio, 2.3297);
	//0.4249 -0.879
	  return readScaled(0.4249, -0.879, MQ131pinH);
	//in ppm
}


void StartGasSens(){
	 _delay_ms(1000);
	calibrateMQ2();
	calibrateMQ135();
	calibrateMQ131L();
	calibrateMQ131H();
}





