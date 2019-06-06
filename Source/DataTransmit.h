#ifndef DATATRANSMIT_H_
#define DATATRANSMIT_H_
//includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

uint32_t time_ms;

void Timer1_Init();
ISR (TIMER1_COMPA_vect);


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


uint16_t Crc16(uint8_t *pcBlock, uint8_t len);
void CreatePackage(struct Data * data, uint32_t *num, int x_raw, int y_raw, int z_raw,
		int32_t temp, float press, float alt, int32_t temp_MXres);

DRESULT WriteSD(struct Data *data, uint32_t n);


void transmit_buff (uint8_t *buff, uint16_t count);
void Transmit (struct Data *data);
#endif
