#ifndef DATATRANSMIT_H_
#define DATATRANSMIT_H_
//includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#define BAUD 9600L   //UART SPEED
#define CHOSEN_SECTOR 0

#define DATA_LEN 46
#define BUFFER_SIZE 64
#define COUNT_IN_512 8

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

	int16_t LPG, Methane, Smoke, Hydrogen, Propane, CO2, OzoneL, OzoneH;	//raw 	16 * 8 bit  = 16 bytes

	uint16_t crc; //16 bits - CRC = 2 bytes
	//46 bytes data
	uint16_t zero[4]; // 16 * 4 bit = 8 bytes  - fill to 64 bytes
};

uint16_t Crc16(uint8_t *pcBlock, uint8_t len);
void CreatePackage(struct Data * data, uint32_t *num, int x_raw, int y_raw, int z_raw,
		int32_t temp, float press, float alt, int32_t temp_MXres,
		uint16_t LPG, uint16_t Methane, uint16_t Smoke, uint16_t Hydrogen, uint16_t Propane,
		uint16_t CO2, uint16_t OzoneL, uint16_t OzoneH);

DRESULT WriteSD(struct Data *data, uint32_t n);


void transmit_buff (uint8_t *buff, uint16_t count);
void Transmit (struct Data *data);
#endif
