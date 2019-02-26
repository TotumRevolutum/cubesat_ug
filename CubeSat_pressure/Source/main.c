#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "bmp280_i2c.h"


#define F_CPU 8000000// Clock Speed

static int8_t USART_PutChar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

static int8_t USART_PutChar(char c, FILE *stream)
{
   if (c == '\n')
	   USART_PutChar('\r', stream);
   USART_SendByte(c);
   return 0;
}

int main(){
	uint8_t bmp280_connection, bmp280_data_received=0, chid;
	int32_t temperature;
	float pressure, alt;

	USART_Init ( 9600 );
	I2CInit(F_CPU, 100000);

	stdout = &mystdout; // указываем, куда будет выводить printf

	printf("\nBMP280 I2C Demo\n");
	bmp280_connection = BMP280_begin(BMP280_DEFAULT_ADDRESS);
	if(bmp280_connection==1){
		printf("BMP280 connection OK\n");
	}else{
		printf("BMP280 connection not OK\n");
		return 0;
	}
	while(1){
		bmp280_data_received = BMP280_receiveData(&temperature,&pressure,&alt);
		if (bmp280_data_received ==1){
			printf("temp = %ld\n press = %.4f\n alt = %.4f\n",temperature, pressure, alt);
		}else{
			printf("BMP280 measurement not OK\n");
		}
		chid = BMP280_read8(BMP280_REGISTER_CHIPID);
		printf("chid = %.2X\n", chid);

	}
}
