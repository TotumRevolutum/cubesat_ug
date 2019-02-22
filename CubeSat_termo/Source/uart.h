/*
 * uart.h
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: KHiAS18
 */
#include <stdint.h>
#ifndef SOURCE_UART_H_
#define SOURCE_UART_H_

void uart_init();
uint8_t uart_receive();
void uart_transmitt(uint8_t data);


#endif /* SOURCE_UART_H_ */
