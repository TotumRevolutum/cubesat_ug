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


//#include "main.c"

#define BAUD 9600L   //UART SPEED
#define CHOSEN_SECTOR 0
#define BUFFER_SIZE 28


uint32_t time_ms = 0;

void Timer1_Init(){
	TCCR1B|=(1<<WGM12)|(1<<CS12)|(0<<CS11)|(0<<CS10);
	//установлен режим CTC (Clear Timer on Compare) или "сброс при совпадении" и установлен предделитель 256
	TIMSK|=(1<<OCIE1A);//разрешает прерывание CTC по значениею
	OCR1A=31;//FCPU-8000000/256 -1 =31249
	asm("sei");
}
ISR (TIMER1_COMPA_vect)
{
	time_ms++;
}



struct Data{
	uint32_t packageNum;	// 32 bit = 4 bytes
	uint32_t time;	//32 bit  = 4 bytes
	int16_t x, y, z;	//raw 	16 * 3 bit  = 6 bytes
	uint32_t temperature;	//32 = 4 bytes
	uint32_t pressure;	//	32 = 4 bytes
	uint16_t altitude;	//16 bit = 2 bytes
	uint32_t temp_res;	//max res temperature	32 bit = 4 bytes
	uint16_t crc; //16 bits - CRC = 2 bytes

	uint16_t zero; // 16 bit = 2 bytes  - useless data
	// 28 * 8 bit = 224 bit - Data
	// 16 = 2 bytes - useless data
	// SUM: 32 * 8 bits = 264 bits
};

/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021    x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
uint16_t Crc16(uint8_t *pcBlock, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    uint8_t i;

    while (len--)
    {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

void CreatePackage(struct Data * data, uint32_t *num, int x_raw, int y_raw, int z_raw,
		int32_t temp, float press, float alt, int32_t temp_MXres){
	(*num)++;
	(*data).packageNum = (*num);
	(*data).time = time_ms;
	(*data).x = x_raw;
	(*data).y = y_raw;
	(*data).z = z_raw;
	(*data).temperature = (uint32_t)temp;
	(*data).pressure = (uint32_t)(press  * 100);
	(*data).altitude = (uint16_t) (alt + 10);
	(*data).temp_res = (uint32_t)(temp_MXres + 1000000);
	(*data).crc = Crc16(&data, BUFFER_SIZE);

	(*data).zero = 0;


	/*(*data).packageNum = 1;
	(*data).time = 100;
	(*data).x = 111;
	(*data).y = 222;
	(*data).z = 333;
	(*data).temperature = 123;
	(*data).pressure = 513;
	(*data).altitude = 404;
	(*data).temp_res = 342;
	(*data).crc = 253;*/
}


DRESULT WriteSD(struct Data *data, uint32_t n){
	DRESULT dres = disk_write_continue(data, BUFFER_SIZE+4);
	if (n % 16 == 0){
		disk_write_stop();
		disk_write_start(n/16*512);
	}
	return dres;
}


void Transmit (struct Data *data)
{
	USART_SendByteArray(data, BUFFER_SIZE+2);
}

