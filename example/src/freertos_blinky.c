/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

QueueHandle_t xQueue1;

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
}

/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters) {

	xQueue1=xQueueGenericCreate(1,sizeof(char),NULL);
	bool LedState = false;
	char status = 0;
	while (1) {


		Board_LED_Set(1, LedState);			//led1 off
		LedState = (bool) !LedState;
		Board_LED_Set(0, LedState);			//led0 on

		/* About a .5Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ /1);

		Board_LED_Set(1, LedState);			//led1 on
		LedState = (bool) !LedState;
		Board_LED_Set(0, LedState);			//led0 off

		/* About a .5Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ /1);

		LedState = (bool) !LedState;
		status = 'A';
		xQueueGenericSend(xQueue1,(void *)&status,(TickType_t)10,0);

	}
}

/* LED2 toggle thread */
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;
	char status = 0;

	while (1) {

		while(!(xQueue1 != 0));
		xQueueGenericReceive(xQueue1,&(status),(TickType_t)10,0);

		if(status=='A')
		{
			status = 0;
			Board_LED_Set(5, LedState);			//led5 off
			LedState = (bool) !LedState;
			Board_LED_Set(4, LedState);			//led4 on

			/* About a 5Hz on/off toggle rate */
			vTaskDelay(configTICK_RATE_HZ /10);

			Board_LED_Set(5, LedState);			//led5 on
			LedState = (bool) !LedState;
			Board_LED_Set(4, LedState);			//led4 off
			/* About a 5Hz on/off toggle rate */
			vTaskDelay(configTICK_RATE_HZ / 10);
		}
}
}
/* LED3 toggle thread */
static void vLEDTask3(void *pvParameters) {
	bool LedState = false;

	while (1) {

		Board_LED_Set(3, LedState);			//led3 off
		LedState = (bool) !LedState;
		Board_LED_Set(2, LedState);			//led2 on

		/* About a 20Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ /40);

		Board_LED_Set(3, LedState);			//led3 on
		LedState = (bool) !LedState;
		Board_LED_Set(20, LedState);			//led2 off
		/* About a 2Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ /40);
	}
}

/* LED4 toggle thread */
static void vLEDTask4(void *pvParameters) {
	bool LedState = false;

	while (1) {

		Board_LED_Set(7, LedState);			//led7 off
		LedState = (bool) !LedState;
		Board_LED_Set(6, LedState);			//led6 on

		/* About a 15Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ /30);

		Board_LED_Set(7, LedState);			//led7 on
		LedState = (bool) !LedState;
		Board_LED_Set(6, LedState);			//led6 off
		/* About a 15Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 30);
}
}

/* UART (or output) thread */
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0;

	while (1) {
		DEBUGOUT("Tick: %d \r\n", tickCnt);
		tickCnt++;

		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	prvSetupHardware();

	/* LED1 toggle thread */
		xTaskCreate(vLEDTask1, "vTaskLed1", configMINIMAL_STACK_SIZE,
				NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	/* LED2 toggle thread */
	xTaskCreate(vLEDTask2, "vTaskLed2", configMINIMAL_STACK_SIZE,
			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	/* LED3 toggle thread */
	xTaskCreate(vLEDTask3, "vTaskLed3", configMINIMAL_STACK_SIZE,
			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	/* LED4 toggle thread */
	xTaskCreate(vLEDTask4, "vTaskLed4", configMINIMAL_STACK_SIZE,
			NULL, (tskIDLE_PRIORITY +1UL), (TaskHandle_t *) NULL);



	/* UART output thread, simply counts seconds */

	xTaskCreate(vUARTTask, "vTaskUart", configMINIMAL_STACK_SIZE,
			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
