#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include <stdio.h>
#include "mxc_errors.h"

#define DEBUG_MODE_UART

#ifdef DEBUG_MODE_UART      
#define UART_DEBUG(...) printf(__VA_ARGS__)
#else
#define UART_DEBUG(...) _NOP()                  //do{}while(0) or _NOP()
#endif

#define E_INVALID_FIFO -18
#define E_FIFO_FULL -19
#define E_FIFO_EMPTY -20

typedef enum{
    I2S_SUCCESS = 0,
    I2S_UNINITIALIZED,
    MIC_UNINITIALIZED,
    PMIC_UNINITIALIZED,
    I2S_ERROR_MAX
}eI2SErrors;

typedef enum{
    SW_TIMER_SUCCESS = 0,
    SW_TIMER_FAIL,
    SW_TIMER_ERROR_MAX
}eSwTimerErrors;

typedef struct{
    eI2SErrors i2s_error;
}sErrorStatus;

#endif