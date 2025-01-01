#ifndef SERIAL_H
#define SERIAL_H

#include "error_handler.h"
#include "circular_fifo.h"
#include "uart.h"
#include "nvic_table.h"

#define CONSOLE_UART MXC_UART3
#define UART_NUM 3
#define UART_BAUD 115200

eUartErrors uartInit(void);
int uartRead(unsigned char *rxData, uint32_t size);
void uartWrite(unsigned char *txData, uint32_t size);
unsigned char uartReceiveFifoStatus(void);
unsigned char uartTransmitFifoStatus(void);

#endif