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
#include "DataProcessing.h"
#include "DataTransmit.h"

#include "MQ2/BaseMQ.h"

#define BAUD 9600L   //UART SPEED
#define CHOSEN_SECTOR 0


static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

void Sensors_Init()
{
  int i = ADXL345_begin(ADXL345_DEFAULT_ADDRESS);
  if (i==1){
	  printf ("ADXL INIT OK");
  } else {
	  printf ("ADXL INIT NOT OK");
  }
}

#define MAXSENSORS 1
uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];
static uint8_t search_sensors(void)
{
	uint8_t i;
	uint8_t id[OW_ROMCODE_SIZE];
	uint8_t diff, nSensors;

	printf("Scanning Bus for DS18X20\n");

	ow_reset();

	nSensors = 0;

	diff = OW_SEARCH_FIRST;
	while ( diff != OW_LAST_DEVICE && nSensors < MAXSENSORS ) {
		DS18X20_find_sensor( &diff, &id[0] );
		if( diff == OW_PRESENCE_ERR ) {
			printf( "No Sensor found\n");
			break;
		}
		if( diff == OW_DATA_ERR ) {
			printf( "Bus Error\n");
			break;
		}
		for ( i=0; i < OW_ROMCODE_SIZE; i++ )
			gSensorIDs[nSensors][i] = id[i];

		nSensors++;
	}

	return nSensors;
}

void print_address(unsigned char* address) {
	printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n", address[0],address[1],address[2],address[3],address[4],address[5],address[6],address[7]);
}

int main( void )
{
	struct Data data;

	uint32_t pkgNum = 0;

	float x_raw, y_raw, z_raw;
	int x_int, y_int, z_int;
	float module_g_raw;
	float x, y, z;
	float module_g;
	float roll = 0.00, pitch = 0.00;

	uint8_t bmp280_connection, bmp280_data_received=0, chid;
	int32_t temperature;
	float pressure, alt;
	unsigned long LPG = 0, Methane = 0, Smoke = 0, Hydrogen = 0, Propane = 0, CO2 = 0, OzoneL = 0, OzoneH = 0;
	float RssL;
	float RssH;
	uint8_t nSensors;
//	int16_t decicelsius;
	int32_t mxRes;
//-------INIT----------------------------------------------------------------------------------------
	Timer1_Init();
	stdout = &mystdout;
	USART_Init ( BAUD );
	I2CInit(F_CPU, 100000);
	Sensors_Init();
	ADC_Init();
	printf("Heat...\n");
	StartGasSens();
	printf("Calibrate...\n");
	printf("Ro = %f\n", mq131L_ro);
//-------SD------------------------------------------------------------------------------------------
	DSTATUS status;
	do{
		status = disk_initialize();
	} while (status);
	printf("Init disk OK\n");
	disk_write_start(CHOSEN_SECTOR);
//-------PRESSURE------------------------------------------------------------------------------------
	printf("\nBMP280 I2C Demo\n");
	bmp280_connection = BMP280_begin(BMP280_DEFAULT_ADDRESS);
	if(bmp280_connection==1){
		printf("BMP280 connection OK\n");
	}else{
		printf("BMP280 connection not OK\n");
		return 0;
	}
//-------TEMPERATURE---------------------------------------------------------------------------------
	printf("\nDS18X20 1-Wire-Reader Demo\n");
	printf("-------------------------------------------\n");
	nSensors = search_sensors();
	printf( "%i DS18X20 Sensor(s) available:\n", nSensors);
//---------------------------------------------------------------------------------------------------
	  while (1)
	  {
//---------------------------------------------------------------------------------------------------
		  printf("------------------ACCELEROMETER:---------------------\n");
		  ADXL345_readXYZ(&x_raw,&y_raw,&z_raw);
		  ADXL345_readXYZ_raw(&x_int,&y_int,&z_int);
		  module_g_raw = sqrt(pow(x_raw,2)+pow(y_raw,2)+pow(z_raw,2));
		  accelerometer_calibration(x_raw, y_raw, z_raw, &x, &y, &z);
		  module_g = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
		  RP_calculate(x, y, z, &roll, &pitch);
		  printf("Raw   data: x: %.6f y: %.6f z: %.6f g: %.6f\n", x_raw, y_raw, z_raw, module_g_raw);
		  printf("Raw   data (int): x: %d y: %d z: %d\n", x_int, y_int, z_int);
		  printf("Calibrated: x:%.6f y:%.6f z:%.6f g: %.6f\n", x, y, z, module_g);
		  printf("Roll: %.6f Pitch: %.6f\n", roll, pitch);
//---------------------------------------------------------------------------------------------------
		  printf("-------------------PRESSURE----------------------\n");
			bmp280_data_received = BMP280_receiveData(&temperature,&pressure,&alt);
			if (bmp280_data_received ==1){
				printf("temp = %ld\n press = %.4f\n alt = %.4f\n",temperature, pressure, alt);
			}
			chid = BMP280_read8(BMP280_REGISTER_CHIPID);
			printf("chid = %.2X\n", chid);
//---------------------------------------------------------------------------------------------------
			printf("-----------------TEMPERATURE---------------------\n");
			print_address(gSensorIDs[0]);
			DS18X20_set_resolution(&gSensorIDs[0][0], DS18B20_10_BIT);
			if ( DS18X20_start_meas( DS18X20_POWER_EXTERN,&gSensorIDs[0][0]) == DS18X20_OK ) {
				_delay_ms( DS18B20_TCONV_10BIT );
				//if (DS18X20_read_decicelsius(&gSensorIDs[0][0], &decicelsius)	== DS18X20_OK ) {
				//	printf("temp = %i\n", decicelsius );
				//}
				if (DS18X20_read_maxres(&gSensorIDs[0][0], &mxRes)	== DS18X20_OK ) {
					printf("temp_maxREs = %.4f\n", mxRes*0.0001 );
				}
			}
//---------------------------------GasSensors--------------------------------------------------------
			printf("-----------------GasSensors----------------------\n");
			LPG = readLPG();
			Methane = readMethane();
			Smoke = readSmoke();
			Hydrogen = readHydrogen();
			Propane = readPropane();
			CO2 = readCO2();
			OzoneL = readOzone_Low(20);
			OzoneH = readOzone_High(20);

			RssL = readRs(MQ131pinL);
			RssH = readRs(MQ131pinH);
			printf("LPG: %lu ppm    ", LPG);
			printf("Methane: %lu ppm   ", Methane);
			printf("Smoke: %lu ppm    ", Smoke);
			printf("Hydrogen: %lu ppm\n", Hydrogen);
			printf("Propane: %lu ppm\n", Propane);
			printf("CO2: %lu ppm\n", CO2);
			printf("OzoneL: %lu ppb\n", OzoneL);
			printf("OzoneH: %lu ppm\n", OzoneH);
			printf("OzoneL Rs: %f KOhm\n", RssL);
			printf("OzoneH Rs: %f KOhm\n", RssH);
//---------------------------------------------------------------------------------------------------
		printf("time ms: %lu\n", time_ms);
		CreatePackage(&data, &pkgNum, x_int, y_int, z_int, temperature, pressure, alt, mxRes,
				LPG, Methane, Smoke, Hydrogen, Propane, CO2, OzoneL, OzoneH);
		printf ("Package Num: %li\n", pkgNum);

		if (WriteSD(&data, pkgNum) == RES_OK){
			printf("WRITING OK\n");
		}else{
			printf("WRITING STH WRONG\n");
		}

		printf("--------------------TRANSMIT---------------------\n");
		Transmit(&data);
		printf("\n-------------------TRANSMITED--------------------\n");
		_delay_ms(1000);	//170 ~6 times per second
	  }
	  return 0;
}

