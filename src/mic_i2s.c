#include "mic_i2s.h"

// static mxc_i2s_req_t req;

// eI2SErrors i2s_init(void)
// {
//     eI2SErrors errorStatus = I2S_UNINITIALIZED;

//     if (max20303_init(MXC_I2C1) != E_NO_ERROR) {
//         UART_DEBUG("Unable to initialize I2C interface to commonicate with PMIC!\n");
//         errorStatus = PMIC_UNINITIALIZED;
//     }

//     if (max20303_mic_power(1) != E_NO_ERROR) {
//         UART_DEBUG("Unable to turn on microphone!\n");
//         errorStatus = MIC_UNINITIALIZED;
//     }

//     MXC_Delay(MXC_DELAY_MSEC(200));

//     printf("\nMicrophone enabled!\n");

//     return errorStatus;
// }