/*
 * uart.c
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: KHiAS18
 */
#include "uart.h"
#include <avr/io.h>

void uart_init(){
	/* Set baud rate */
	UBRR0H = 0;
	UBRR0L = 51;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (3<<UCSZ00);
}
uint8_t uart_receive(){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}
void uart_transmitt(uint8_t data){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
