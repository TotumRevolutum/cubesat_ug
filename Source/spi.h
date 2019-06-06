/*
 * spi.h
 *
 * Created: 7/2/2018 4:21:09 PM
 *  Author: madiv
 */ 

#ifndef SPI_H_
#define SPI_H_
#include <avr/io.h>
#define SPIPORT			PORTB
#define SPIDDR			DDRB
#define SPI_SCK			(1 << PB1) /* PB1 */
#define SPI_MISO		(1 << PB3) /* PB3 SD card DO */
#define SPI_MOSI		(1 << PB2) /* PB2 SD card DI */
#define SPI_CS			(1 << PB0) /* PB0 */
static inline void init_spi(void)
{
	/* SPI pins */
	SPIDDR |= (SPI_MOSI | SPI_CS | SPI_SCK); /* Set outputs */
	SPIPORT |= SPI_MISO; /* Pull up on SPI_MISO (SD card DO) */
	/* Master Mode, 100-400kHz clk for SD card initialization sequence */
	/* Setup Peripheral Clock Prescaler for required SD card initialization */	
	/* SPI Master mode */
	SPCR |= (1 << MSTR);
	/* Set clock speed (8MHz oscillator / 32 = 250kHz) */
	SPCR |= (1 << SPR1);
	SPSR |= (1 << SPI2X);
	/* SPI Enable */
	SPCR |= (1 << SPE);
}
static inline uint8_t spi(uint8_t d)
{
	SPDR = d; //Load a byte into the SPI data buffer for transfer
	while(!(SPSR & (1 << SPIF))); //wait until transmission is complete - SPIF=SPI interrupt flag, when transfer is finished this flag is set
	return SPDR;
}
static inline void tx_spi (uint8_t d)
{
	spi(d);
}
static inline uint8_t rx_spi (void)
{
	return spi(0xFF);
}
#endif /* SPI_H_ */
