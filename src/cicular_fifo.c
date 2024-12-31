#include "circular_fifo.h"

static sFifo fifoList[FIFO_MAX]; // array that holds all the fifos

//for error handling status flags and error codes

int fifoInit(eFifoNumber fifo_num){
    
    return fifoFlush(fifo_num);
}

//disable irq before calling this function
int fifoFlush(eFifoNumber fifo_num){
    if(fifo_num >= FIFO_MAX){
        UART_DEBUG("\nInvalid FIFO number");
        return E_INVALID_FIFO;
    }
    sFifo *pFifo = &fifoList[fifo_num]; // get the pointer to the fifo
    pFifo->write_pos = 0;
    pFifo->read_pos = 0;
    pFifo->cnt = 0;
    return E_SUCCESS;
}

int fifoWrite(eFifoNumber fifo_num, unsigned char data){

    if(fifo_num >= FIFO_MAX){
        UART_DEBUG("\nInvalid FIFO number");
        return E_INVALID_FIFO;
    }

    sFifo *pFifo = &fifoList[fifo_num]; // get the pointer to the fifo

    if(pFifo->cnt >= FIFO_DEPTH){
        UART_DEBUG("\nFifo is full");
        return E_FIFO_FULL;
    }

    pFifo->buf[pFifo->write_pos] = data;

    if(++(pFifo->write_pos) >= FIFO_DEPTH){
        pFifo->write_pos = 0;
    }

    pFifo->cnt++;

    return E_SUCCESS;
}

int fifoRead(eFifoNumber fifo_num, char* rx_data){

    sFifo *pFifo = &fifoList[fifo_num]; // get the pointer to the fifo

    if(pFifo->cnt == 0){
        UART_DEBUG("\nFifo is empty");
        return E_FIFO_EMPTY;
    }

    *rx_data = pFifo->buf[pFifo->read_pos];

    if(++(pFifo->read_pos) >= FIFO_DEPTH){
        pFifo->read_pos = 0;
    }

    pFifo->cnt--;

    return E_SUCCESS;
}

unsigned char fifoStatus(eFifoNumber fifo_num){
	return(fifoList[fifo_num].cnt);
}

bool isFifoFull(eFifoNumber fifo_num){
	if(fifoList[fifo_num].cnt == FIFO_DEPTH)
		return true;
	else
		return false;
}