#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <avr/io.h>

void USART_Init(uint16_t baudrate);
void USART_SendByte(uint8_t data );
uint8_t USART_Receive( void );
void USART_SendByteArray(uint8_t *data, uint8_t len);

#endif /* UART_H_ */
