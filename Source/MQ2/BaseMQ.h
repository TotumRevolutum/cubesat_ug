#ifndef SOURCE_MQ2_BASEMQ_H_
#define SOURCE_MQ2_BASEMQ_H_

#include "adc.h"
#include "math.h"
#include <util/delay.h>

#define OPERATING_VOLTAGE   5.0
// разр€дность ј÷ѕ
#define ADC_BIT             10
// масимальное значение ј÷ѕ
#define ADC_VALUE_MAX       pow(2, ADC_BIT)
// резистор установленный на плату (кќм)
#define MQ2_RL_BOARD            5.0
// коэффициент чистого воздуха из графика
#define MQ2_RO_IN_CLEAR_AIR     9.83


// резистор установленный на плату (кќм)
#define MQ135_RL_BOARD          10.0


// концентраци€ углекислого газа в сухом воздухе
#define PPM_CO2_IN_CLEAR_AIR    397.13

#define MQ131_RO_IN_CLEAR_AIR     29.0
#define MQ131_L_R0           9.7                  // Default R0 for low concentration MQ131
#define MQ131_H_R0           6.7

#define MQ131L_RL_BOARD          9.7
#define MQ131H_RL_BOARD          6.7

//virtual float getRoInCleanAir() const { return exp((log(PPM_CO2_IN_CLEAR_AIR) * -0.42) + 1.92); }

#define MQ2pin 0
#define MQ135pin 1
#define MQ131pinL 2
#define MQ131pinH 3

float mq2_ro;
float mq135_ro;
float mq131L_ro;
float mq131H_ro;


float calculateResistance(int sensorADC);
float readRs(int ADCpin);
float readRatio(int ADCpin);
float readScaled(float a, float b, int ADCpin);

void calibrateMQ2();
void calibrateMQ135();
void calibrateMQ131L();
void calibrateMQ131H();

unsigned long readLPG();
unsigned long readMethane();
unsigned long readSmoke();
unsigned long readHydrogen();
unsigned long readPropane();


unsigned long readCO2();

float getEnvCorrOzoneRatio(int tempCels, int humidityPercent);
unsigned long readOzone_Low(int tempCels);
unsigned long readOzone_High(int tempCels);


void StartGasSens();

#endif /* SOURCE_MQ2_BASEMQ_H_ */
