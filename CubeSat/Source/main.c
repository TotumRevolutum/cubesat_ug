//includes
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"

#include "i2c_avr.h"
#include "adxl345_i2c.h"

#include "bmp280_i2c.h"

#include "1wire_h/onewire.h"
#include "ds18_h/ds18x20.h"
//includes

////////////////////////////////////////////////////
//accelerometer
#define F_CPU 8000000// Clock Speed
#define BAUD 9600L   //UART SPEED

static int8_t USART_PutChar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

static int8_t USART_PutChar(char c, FILE *stream)
{
   if (c == '\n')
	   USART_PutChar('\r', stream);
   USART_SendByte(c);
   return 0;
}

//Инициализация всех сенсоров
void Sensors_Init()
{
 // UART_Init(0, 9600, 0);
  I2CInit(F_CPU, 100000);
  int i = ADXL345_begin(ADXL345_DEFAULT_ADDRESS);
  if (i==1){
	  printf ("ADXL INIT OK");
  } else {
	  printf ("ADXL INIT NOT OK");
  }
}
//accelerometer functions

////////////////////////////////////////////////////////////////////////////

//pressure
#define F_CPU 8000000// Clock Speed

//static int8_t USART_PutChar(char c, FILE *stream);
//static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

/*static int8_t USART_PutChar(char c, FILE *stream)
{
   if (c == '\n')
	   USART_PutChar('\r', stream);
   USART_SendByte(c);
   return 0;
}*/
// pressure functions

/////////////////////////////////////////////////////////////////////////////

//temperature

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

//static int uart_putchar(char c, FILE *stream);
//static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

/*static int uart_putchar(char c, FILE *stream){
	uart_transmitt(c);
	return 0;
}*/
//temperarure functions




//accelerometer main
int main( void )
{
/////////////////////////////////////////////////////////////////////////
	//accelerometer start
	  float x, y, z;
	  float module_g;


	  USART_Init ( BAUD );
	  stdout = &mystdout; // указываем, куда будет выводить printf
	  Sensors_Init();
	  //accelerometer end
////////////////////////////////////////////////////////////////////////////
		//pressure start
		uint8_t bmp280_connection, bmp280_data_received=0, chid;
		int32_t temperature;
		float pressure, alt;

		//USART_Init ( 9600 );
		I2CInit(F_CPU, 100000);

		//stdout = &mystdout; // указываем, куда будет выводить printf

		printf("\nBMP280 I2C Demo\n");
		bmp280_connection = BMP280_begin(BMP280_DEFAULT_ADDRESS);
		if(bmp280_connection==1){
			printf("BMP280 connection OK\n");
		}else{
			printf("BMP280 connection not OK\n");
			return 0;
		}
		//pressure end
////////////////////////////////////////////////////////////////////////////////////
		//temperature start
		uint8_t nSensors, i;
		int16_t decicelsius;
		int32_t mxRes;
		uint8_t error;
		//uart_init();
		//USART_Init( 9600 );
		//stdout = &mystdout;
		printf("\nDS18X20 1-Wire-Reader Demo\n");
		printf("-------------------------------------------\n");
		nSensors = search_sensors();
		printf( "%i DS18X20 Sensor(s) available:\n", nSensors);
		//temperature end
/////////////////////////////////////////////////////////////////////////////////////

	  while (1)
	  {
//////////////////////////////////////////////////////////////////////
		  //accelerometer loop
		  printf("------------------ACCELEROMETER:---------------------\n");
		  ADXL345_readXYZ(&x,&y,&z);
		  printf("x = %.6f\n ", x);
		  printf("y = %.6f\n ", y);
		  printf("z = %.6f\n ", z);
		  module_g = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
		  printf("module_g = %.6f\n ", module_g);

		  //every 200 ms read accelerometer data
		  //_delay_ms(200);
		  //accelerometer loop
//////////////////////////////////////////////////////////////////////
			//pressure loop
		  printf("-------------------PRESSURE----------------------\n");
			bmp280_data_received = BMP280_receiveData(&temperature,&pressure,&alt);
			if (bmp280_data_received ==1){
				printf("temp = %ld\n press = %.4f\n alt = %.4f\n",temperature, pressure, alt);
			}else{
				printf("BMP280 measurement not OK\n");
			}
			chid = BMP280_read8(BMP280_REGISTER_CHIPID);
			printf("chid = %.2X\n", chid);
			//pressure loop
////////////////////////////////////////////////////////////////////////
			//temperature loop
			printf("-----------------TEMPERATURE---------------------\n");
			for ( i = nSensors; i > 0; i-- ) {
				printf("Sensor# %i \n",i);
				print_address(gSensorIDs[i-1]);
				DS18X20_set_resolution(&gSensorIDs[i-1][0], DS18B20_12_BIT);

				if ( DS18X20_start_meas( DS18X20_POWER_EXTERN,&gSensorIDs[i-1][0]) == DS18X20_OK ) {
					_delay_ms( DS18B20_TCONV_12BIT );
					if (DS18X20_read_decicelsius(&gSensorIDs[i-1][0], &decicelsius)	== DS18X20_OK ) {
						printf("temp = %i\n", decicelsius );
					} else {
						printf( "CRC Error (lost connection?)" );
						error++;
					}
					if (DS18X20_read_maxres(&gSensorIDs[i-1][0], &mxRes)	== DS18X20_OK ) {
										printf("temp_maxREs = %.4f\n", mxRes*0.0001 );
									} else {
										printf( "CRC Error (lost connection?)" );
										error++;
									}
				}
				else {
					printf( "Start meas. failed (short circuit?)\n" );
					error++;
				}
			}
			//read temperature data every 500 ms
			//_delay_ms(500);
			//temperature loop
////////////////////////////////////////////////////////////////////////////

			_delay_ms(1000);
	  }
	  return 0;
}

