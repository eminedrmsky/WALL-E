/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/**
 * @file        main.c
 * @brief       FreeRTOS Example Application.
 */

#include <stdio.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "sw_timer.h"
#include "error_handler.h"
#include "serial.h"

/***** Definitions *****/
/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT   ( 0xfffff )



/***** Functions *****/

/***** Tasks *****/
void vTask1(void* pvParameters)
{
	const char* pcTaskName = "Task 1 is running\r\n";
	 /* As per most tasks, this task is implemented in an infinite loop. */
	for (;; )
	{
		/* Print out the name of this task. */
		printf("%s", pcTaskName);
		/* Delay for a period. */
		MXC_Delay(10000);
	}
}

void vTask2(void* pvParameters)
{
	const char* pcTaskName = "Task 2 is running\r\n";
	 /* As per most tasks, this task is implemented in an infinite loop. */
	for (;; )
	{
		/* Print out the name of this task. */
		printf("%s",pcTaskName);
		/* Delay for a period. */
		MXC_Delay(10000);
	}
}

/******************************************************************************/
int main(void)
{
	sErrorStatus errorHandler = {0};

    errorHandler.sw_timer_error =  swTimerStart();
	errorHandler.uart_error = uartInit();

	if(errorHandler.sw_timer_error != SW_TIMER_SUCCESS){
		UART_DEBUG("Error starting SW Timer\n");
	}
	if(errorHandler.uart_error != UART_SUCCESS){
		UART_DEBUG("Error initializing UART\n");
	}


		uint32_t error = xTaskCreate(vTask1, /* Pointer to the function that implements the task. */
		"Task 1",/* Text name for the task. This is to facilitate
		debugging only. */
		250, /* Stack depth - small microcontrollers will use much
		less stack than this. */
		NULL, /* This example does not use the task parameter. */
		1, /* This task will run at priority 1. */
		NULL); /* This example does not use the task handle. */
		/* Create the other task in exactly the same way and at the same priority. */
		printf("error: %ud", error);
		error = xTaskCreate(vTask2, "Task 2", 250, NULL, 1, NULL);
		printf("error: %u", error);
	/* Start the scheduler so the tasks start executing. */
	vTaskStartScheduler();

	/* If all is well then main() will never reach here as the scheduler will
	now be running the tasks. If main() does reach here then it is likely that
	there was insufficient heap memory available for the idle task to be created.
	Chapter 2 provides more information on heap memory management. */
	for (;; );

}