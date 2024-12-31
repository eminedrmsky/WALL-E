#ifndef SW_TIMER_H_
#define SW_TIMER_H_

#include <stddef.h>
#include <stdbool.h>
#include "error_handler.h"
#include "FreeRTOS.h"
#include "timers.h"


#define TIMER_PERIOD (pdMS_TO_TICKS(1))

typedef enum{
    TIMEOUT_HEARTBEAT_1MS,
    TIMEOUT_UART_1MS,
    TIMEOUT_MAX
}eTimeouts;

extern volatile bool heartbeatFlag;
extern volatile short timeoutCounter[TIMEOUT_MAX];
typedef void (*pTimeoutFunction) (void);

#define SET_TIMEOUT(timeout, timeoutType) (timeoutCounter[timeoutType] = timeout)
#define CLEAR_TIMEOUT(timeoutType) (timeoutCounter[timeoutType] = -1)
#define GET_TIMEOUT(timeoutType) (timeoutCounter[timeoutType])

eSwTimerErrors swTimerStop(void);
eSwTimerErrors swTimerStart(void);

#endif /* SW_TIMER_H_ */