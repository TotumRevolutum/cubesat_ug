#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include "uart.h"
#include "sdcard_raw.h"

//#define F_CPU 8000000// Clock Speed
#define BAUD 9600L   //UART SPEED
#define CHOSEN_SECTOR 0
#define BUFFER_SIZE 26


#include <stdint.h>
/*struct telemetry {
   uint16_t packet_num;
   int32_t accel_x;
};
struct telemetry tl;*/

static int8_t USART_PutChar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);

static int8_t USART_PutChar(char c, FILE *stream)
{
	if (c == '\n')
		USART_PutChar('\r', stream);
	USART_SendByte(c);
	return 0;
}



struct Data{
	uint16_t packageNum;	// 16 bit = 2 bytes
	uint32_t time;	//32 bit  = 4 bytes
	int16_t x, y, z;	//raw 	16 * 3 bit  = 6 bytes
	int16_t temperature;	//16 = 2 bytes
	uint32_t pressure;	//	32 = 4 bytes
	uint16_t altitude;	//16 bit = 2 bytes
	int16_t temp_res;	//max res temperature	16 bit = 2 bytes
	uint16_t crc; //16 bits - CRC = 2 bytes
	//22 * 8 bit = 176 bit - Data
	// SUM: 24 * 8 bits = 192 bits
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

int main( void )
{

//	struct Data *data;
	uint8_t isReading = 1;
	int num = 0;

	USART_Init ( BAUD );
	stdout = &mystdout; // указываем, куда будет выводить printf

	DSTATUS status;
	do{
		status = disk_initialize();
	} while (status);
	printf("Init disk OK\n");

	int n = 0;
	uint8_t Package[32];
//	uint8_t Package[32] = {"Maybie it works???? testing...__"};


	disk_read_start(CHOSEN_SECTOR);
//	disk_write_start(CHOSEN_SECTOR);

	while (isReading && (n < num || num == 0) )
	{
		n++;
		disk_read_continue(Package, 32);
//		disk_write_continue(Package, 32);
		printf("Package:	%d\n", n);
		for (uint8_t i = 0; i < 32; i++){
			printf("%c", Package[i]);
		}
		printf("\n");
		if (n % 16 == 0){
			printf("-----------------------------------------512---%d-----\n", n/16);
			disk_read_stop();
			disk_read_start(n/16*512);

//			disk_write_stop();
//			disk_write_start(n/16*512);
		}




		/*if (n % 19 == 0){
//					disk_write_continue(zero_buff, 8);
					disk_read_continue(Package, 8);
//					disk_write_stop();
					disk_read_stop();
//					disk_write_start(n/19*512);
					disk_read_start(n/19*512);
//					printf("00000000\n");
					printf("Package:	%d\n", n);
					for (uint8_t i = 0; i < 8; i++){
						printf("%c", Package[i]);
					}
					printf("\n");
					printf("-----------------------------------------512---%d-----\n", n/19);
		}else{
//			disk_write_continue(Package, 28);
			disk_read_continue(Package, 28);
			printf("Package:	%d\n", n);
			for (uint8_t i = 0; i < 28; i++){
				printf("%c", Package[i]);
			}
			printf("\n");
		}
*/

//		data = Package;
		//(*data).altitude
		/*if ( Package[1]<<4 + Package[0] == n + 1 ){
//		if (Crc16(data, BUFFER_SIZE) == (*data).crc){
			printf("Correct data. Must be checked\n");
		}else{
			printf("Uncorrect data. Stop reading.\n");
//			isReading = 0;
		}*/
	}
//	disk_read_stop();
//	disk_write_stop();
	return 0;
}
