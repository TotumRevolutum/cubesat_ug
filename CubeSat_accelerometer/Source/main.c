#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include "uart.h"
#include "i2c_avr.h"
#include "adxl345_i2c.h"

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
int main( void )
{
	  float x, y, z;
	  float module_g;


	  USART_Init ( BAUD );
	  stdout = &mystdout; // указываем, куда будет выводить printf
	  Sensors_Init();

	  while (1)
	  {

		  ADXL345_readXYZ(&x,&y,&z);
		  printf("x = %.6f\n ", x);
		  printf("y = %.6f\n ", y);
		  printf("z = %.6f\n ", z);
		  module_g = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
		  printf("module_g = %.6f\n ", module_g);
		  _delay_ms(200);

	  }
	  return 0;
	}
