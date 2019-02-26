#include "uart.h"
#include <stdio.h>
#include <string.h>

void USART_Init(uint16_t baudrate)
{
	uint16_t ubrr = F_CPU/16/baudrate-1;
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00); //8 bit, 1 stop bit
}

void USART_SendByte(uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_SendByteArray(uint8_t *data, uint8_t len)
{
    int i;

    if (len == -1) len = strlen((char *)data);
	for (i=0; i<len; i++) USART_SendByte(data[i]);
	while(!(UCSR0A&(1<<TXC0)));
}
