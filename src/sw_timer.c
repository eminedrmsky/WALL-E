#include "sw_timer.h"
#include "tmr.h"
#include "mxc_device.h"
#include "mxc_sys.h"
#include "nvic_table.h"
#include "led.h"

static xTimerHandle xTimer = NULL;
volatile short timeoutCounter[TIMEOUT_MAX] = {-1, -1};

static void heartBeatTimeout(void){
    LED_Toggle(LED1);
    SET_TIMEOUT(1000, TIMEOUT_HEARTBEAT_1MS);
}

static void uartTimeout(void){
    // Do something
}

pTimeoutFunction timeoutFunctions[TIMEOUT_MAX] = {
    heartBeatTimeout,
    uartTimeout
};

static void swTimerCallback(xTimerHandle pxTimer){

    unsigned char i = 0;

    for(i = 0; i < TIMEOUT_MAX; i++){
        if(timeoutCounter[i] > 0){
            timeoutCounter[i]--;
            if(timeoutCounter[i] == 0){
                timeoutFunctions[i]();
            }
        }
    }

}

eSwTimerErrors swTimerStart(void){
    eSwTimerErrors result = SW_TIMER_FAIL;

    if(xTimer == NULL){
        xTimer = xTimerCreate("SW Timer", TIMER_PERIOD, pdTRUE, (void*)0, swTimerCallback);
    }

    if(xTimerStart(xTimer,0) == pdPASS){
        result = SW_TIMER_SUCCESS;
    }

    SET_TIMEOUT(500, TIMEOUT_HEARTBEAT_1MS);

    return result;

}

eSwTimerErrors swTimerStop(void){
    eSwTimerErrors result = SW_TIMER_FAIL;

    if(xTimerStop(xTimer,0) == pdPASS){
        result = SW_TIMER_SUCCESS;
    }

    return result;
}



