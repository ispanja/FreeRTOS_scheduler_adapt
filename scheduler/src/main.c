/*
    FreeRTOS V8.2.1 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?".  Have you defined configASSERT()?  *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *   Investing in training allows your team to be as productive as       *
     *   possible as early as possible, lowering your overall development    *
     *   cost, and enabling you to bring a more robust product to market     *
     *   earlier than would otherwise be possible.  Richard Barry is both    *
     *   the architect and key author of FreeRTOS, and so also the world's   *
     *   leading authority on what is the world's most popular real time     *
     *   kernel for deeply embedded MCU designs.  Obtaining your training    *
     *   from Richard ensures your team will gain directly from his in-depth *
     *   product knowledge and years of usage experience.  Contact Real Time *
     *   Engineers Ltd to enquire about the FreeRTOS Masterclass, presented  *
     *   by Richard Barry:  http://www.FreeRTOS.org/contact
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *    You are receiving this top quality software for free.  Please play *
     *    fair and reciprocate by reporting any suspected issues and         *
     *    participating in the community forum:                              *
     *    http://www.FreeRTOS.org/support                                    *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"

/* Task parameters */
#define CAPACITY_A	3	//CPU time in ticks
#define CAPACITY_B	3
#define CAPACITY_C	1
#define PERIOD_A	5	//tasks periods
#define PERIOD_B	8
#define PERIOD_C	6
/*-----------------------------------------------------------*/

/* Tasks */
static void taskA( void *pvParameters );
static void taskB( void *pvParameters );
static void taskC( void *pvParameters );
/*-----------------------------------------------------------*/

int main( void )
{
	portBASE_TYPE status;

	xil_printf("Start %d/%d + %d/%d + %d/%d\r\n", CAPACITY_A, PERIOD_A, CAPACITY_B, PERIOD_B, CAPACITY_C, PERIOD_C);	/* Create tasks scheduled by EDF algorithm. */
	status = xTaskLlfCreate( taskA, ( const char * ) "A", configMINIMAL_STACK_SIZE, NULL, 1, NULL, PERIOD_A, CAPACITY_A);
	if ( status != pdTRUE)
	{
		xil_printf("Error creating task.\r\n");
	}
	status = xTaskLlfCreate( taskB, ( const char * ) "B", configMINIMAL_STACK_SIZE, NULL, 1, NULL, PERIOD_B, CAPACITY_B);
	if ( status != pdTRUE)
	{
			xil_printf("Error creating task.\r\n");
	}
	status = xTaskLlfCreate( taskC, ( const char * ) "C", configMINIMAL_STACK_SIZE, NULL, 1, NULL, PERIOD_C, CAPACITY_C);
	if ( status != pdTRUE)
	{
			xil_printf("Error creating task.\r\n");
	}

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}


/*-----------------------------------------------------------*/
static void taskA( void *pvParameters )
{
	TickType_t xLastWakeTimeA;
	const TickType_t xFrequency = PERIOD_A;	//task A frequeny
	TickType_t count = CAPACITY_A;	//task A capacity

	TickType_t x;
	TickType_t xTime;

	xLastWakeTimeA = 0;

	for( ;; )
	{

		xil_printf("TICK: %d, (%d)(taskA) start\r\n", xTaskGetTickCount(), xLastWakeTimeA );

		xTime= xTaskGetTickCount();

		/* While loop that simulates capacity */
		while(count != 0)
		{
			if((x = xTaskGetTickCount()) > xTime)
			{
				count--;
				xTime = x;
				//xil_printf("TICK: %d, (taskA)\r\n", xTaskGetTickCount());

			}
		}
		xil_printf("TICK: %d, (%d)(taskA) end\r\n", xTaskGetTickCount(), xLastWakeTimeA);
		count = CAPACITY_A;

		//Wait for the next period
		vTaskDelayUntil( &xLastWakeTimeA, xFrequency);

	}
}

/*-----------------------------------------------------------*/
static void taskB( void *pvParameters )
{
	TickType_t xLastWakeTimeB;
	const TickType_t xFrequency = PERIOD_B;	//task B frequency
	TickType_t count = CAPACITY_B;	//task B capacity

	TickType_t x;
	TickType_t xTime;

	xLastWakeTimeB = 0;

	for( ;; )
	{
		xil_printf("TICK: %d, (%d)(taskB) start\r\n", xTaskGetTickCount(), xLastWakeTimeB );



		xTime = xTaskGetTickCount();

		/* While loop that simulates capacity */
		while(count != 0)
		{
			if((x = xTaskGetTickCount()) > xTime)
			{
				count--;
				xTime = x;
				//xil_printf("TICK: %d, (taskB)\r\n", xTaskGetTickCount());

			}
		}
		xil_printf("TICK: %d, (%d)(taskB) end\r\n", xTaskGetTickCount(), xLastWakeTimeB );
		count = CAPACITY_B;

		//Wait for the next period
		vTaskDelayUntil( &xLastWakeTimeB, xFrequency);

	}
}

/*-----------------------------------------------------------*/
static void taskC( void *pvParameters )
{
	TickType_t xLastWakeTimeC;
	const TickType_t xFrequency = PERIOD_C;	//task C frequency
	TickType_t count = CAPACITY_C;	//task C capacity

	TickType_t x;
	TickType_t xTime;

	xLastWakeTimeC = 0;
	for( ;; )
	{
		xil_printf("TICK: %d, (%d)(taskC) start\r\n", xTaskGetTickCount(), xLastWakeTimeC );



		xTime = xTaskGetTickCount();

		/* While loop that simulates capacity */
		while(count != 0)
		{
			if((x = xTaskGetTickCount()) > xTime)
			{
				count--;
				xTime = x;
				//xil_printf("TICK: %d, (taskB)\r\n", xTaskGetTickCount());

			}
		}
		xil_printf("TICK: %d, (%d)(taskC) end\r\n", xTaskGetTickCount(), xLastWakeTimeC );
		count = CAPACITY_C;

		//Wait for the next period
		vTaskDelayUntil( &xLastWakeTimeC, xFrequency);

	}
}


