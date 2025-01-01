#include "serial.h"
#include "sw_timer.h"

#define FIFO_RX FIFO_UART0_RX
#define FIFO_TX FIFO_UART0_TX

static bool uart_active_flag = false;

static void uartCallback(void){

    unsigned char data;
    uint32_t intfl = MXC_UART_GetFlags(CONSOLE_UART);
    MXC_UART_ClearFlags(CONSOLE_UART, intfl);

    if(intfl & MXC_F_UART_INT_FL_RX_THD){

        MXC_UART_DisableInt(CONSOLE_UART, MXC_F_UART_INT_EN_RX_THD);
        while(MXC_UART_GetStatus(CONSOLE_UART) &  MXC_F_UART_STATUS_RX_EM){
            MXC_UART_ReadRXFIFO(CONSOLE_UART, &data, 1);
            fifoWrite(FIFO_RX, data);
            SET_TIMEOUT(5, TIMEOUT_UART_1MS);
        }
        MXC_UART_EnableInt(CONSOLE_UART, MXC_F_UART_INT_EN_RX_THD);
    }

    if(intfl & (MXC_F_UART_INT_FL_TX_HE | MXC_F_UART_INT_FL_TX_OB)){

        if(fifoStatus(FIFO_TX) > 0){
			MXC_UART_DisableInt(CONSOLE_UART, (MXC_F_UART_INT_FL_TX_HE | MXC_F_UART_INT_FL_TX_OB));
			fifoRead(FIFO_TX, &data);
            if(MXC_UART_GetStatus(CONSOLE_UART) & MXC_F_UART_STATUS_TX_FULL){
                MXC_UART_WriteTXFIFO(CONSOLE_UART, &data, 1);
            }
			MXC_UART_EnableInt(CONSOLE_UART, (MXC_F_UART_INT_FL_TX_HE | MXC_F_UART_INT_FL_TX_OB));
		}else{
			uart_active_flag = false;
			MXC_UART_DisableInt(CONSOLE_UART, (MXC_F_UART_INT_FL_TX_HE | MXC_F_UART_INT_FL_TX_OB));
		}

    }
}

eUartErrors uartInit(){

    eUartErrors result = UART_UNINITIALIZED;

    if ((MXC_UART_Init(CONSOLE_UART, UART_BAUD, MXC_UART_APB_CLK)) != E_NO_ERROR) {
        UART_DEBUG("\n-->Error initializing UART");
        return result;
    }

    UART_DEBUG("UART Initialized\n");

    NVIC_ClearPendingIRQ(MXC_UART_GET_IRQ(UART_NUM));
    NVIC_DisableIRQ(MXC_UART_GET_IRQ(UART_NUM));

    if(fifoInit(FIFO_RX) != E_SUCCESS){
        UART_DEBUG("\nRX FIFO init failed");
        result = UART_FIFO_INIT_ERROR;
    }

    if(fifoInit(FIFO_TX) != E_SUCCESS){
        UART_DEBUG("\nTX FIFO init failed");
        result = UART_FIFO_INIT_ERROR;
    }

    MXC_NVIC_SetVector(MXC_UART_GET_IRQ(UART_NUM), uartCallback);
    NVIC_EnableIRQ(MXC_UART_GET_IRQ(UART_NUM));
    MXC_UART_ClearFlags(CONSOLE_UART, MXC_F_UART_INT_FL_RX_THD | (MXC_F_UART_INT_FL_TX_HE | MXC_F_UART_INT_FL_TX_OB));
    MXC_UART_EnableInt(CONSOLE_UART, MXC_F_UART_INT_EN_RX_THD);
    MXC_UART_ClearRXFIFO(CONSOLE_UART);
    MXC_UART_ClearTXFIFO(CONSOLE_UART);

    return UART_SUCCESS;
}

int uartRead(unsigned char *rxData, uint32_t size){
    int result = E_SUCCESS;

	MXC_UART_DisableInt(CONSOLE_UART, MXC_F_UART_INT_EN_RX_THD);

	for(int i = 0; i < size; i++){
		if(result != E_FIFO_EMPTY){
			result = fifoRead(FIFO_RX, &rxData[i]);
		}
	}
    MXC_UART_EnableInt(CONSOLE_UART, MXC_F_UART_INT_EN_RX_THD);

	return result;
}

void uartWrite(unsigned char *txData, uint32_t size){
	for(int i=0; i < size; i++){

		fifoWrite(FIFO_TX, txData[i]);
		
		if(!uart_active_flag){
			uart_active_flag = true;
			MXC_UART_EnableInt(CONSOLE_UART, (MXC_F_UART_INT_FL_TX_HE | MXC_F_UART_INT_FL_TX_OB));
		}

	}

	return;
}

unsigned char uartReceiveFifoStatus(void){
	return fifoStatus(FIFO_RX);
}

unsigned char uartTransmitFifoStatus(void){
	return fifoStatus(FIFO_TX);
}
