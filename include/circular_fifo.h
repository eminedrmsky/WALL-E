#ifndef CIRCULAR_FIFO_H_
#define CIRCULAR_FIFO_H_    

#include <stdbool.h>
#include "error_handler.h"

#define DATA_CAPTURE_SIZE 512
#define DATA_CAPTURE_NUM 8
#define FIFO_DEPTH	DATA_CAPTURE_SIZE*DATA_CAPTURE_NUM

typedef struct{
    unsigned char write_pos;
    unsigned char read_pos;
    unsigned char cnt; 
    char buf[FIFO_DEPTH];
}sFifo;

typedef enum{	
    FIFO_UART0_RX,
    FIFO_UART0_TX,	
    FIFO_I2S_RX,
    FIFO_I2S_TX,
    FIFO_KEYB,
    FIFO_MAX    
}eFifoNumber;

int fifoInit(eFifoNumber fifo_num);
int fifoRead(eFifoNumber fifo_num, char* rx_data);
int fifoWrite(eFifoNumber fifo_num, unsigned char data);
bool isFifoFull(eFifoNumber fifo_num);
int fifoFlush(eFifoNumber fifo_num);
unsigned char fifoStatus(eFifoNumber fifo_num);

#endif // CIRCULAR_FIFO_H_