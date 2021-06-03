#ifndef  SERIAL_COMM_H
#define  SERIAL_COMM_H

#include <stdint.h>
#include <stdlib.h>

#define BAUD (9600)
#define MYUBRR (F_CPU/16/BAUD-1)

#define  RX_BUFFER_SIZE  64
#define  TX_BUFFER_SIZE  64

void serial_comm_init(unsigned int ubrr);

void uart_putchar(uint8_t c);

uint8_t uart_getchar(void);

uint8_t uart_available(void);

void serial_comm_send(char* string, size_t size);

#endif //SERIAL_COMM_H