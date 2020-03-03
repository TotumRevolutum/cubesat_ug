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
#include "DataTransmit.h"



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
		int32_t temp, float press, float alt, int32_t temp_MXres,
		uint16_t LPG, uint16_t Methane, uint16_t Smoke, uint16_t Hydrogen, uint16_t Propane,
		uint16_t CO2, uint16_t OzoneL, uint16_t OzoneH){
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
	(*data).LPG = LPG;
	(*data).Methane = Methane;
	(*data).Smoke = Smoke;
	(*data).Hydrogen = Hydrogen;
	(*data).Propane = Propane;
	(*data).CO2 = CO2;
	(*data).OzoneL = OzoneL;
	(*data).OzoneH = OzoneH;
	(*data).crc = Crc16(&data, DATA_LEN);

	(*data).zero[0] = 0;	(*data).zero[1] = 0;
	(*data).zero[2] = 0;	(*data).zero[3] = 0;
}


DRESULT WriteSD(struct Data *data, uint32_t n){
	DRESULT dres = disk_write_continue(data, BUFFER_SIZE);
	if (n % COUNT_IN_512 == 0){
		disk_write_stop();
		disk_write_start(n/COUNT_IN_512*512);
	}
	return dres;
}


void Transmit (struct Data *data)
{
	USART_SendByteArray(data, BUFFER_SIZE);
}

