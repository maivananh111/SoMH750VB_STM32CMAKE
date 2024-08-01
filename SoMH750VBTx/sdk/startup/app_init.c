/*
 * app_init.c
 *
 *  Created on: Jun 15, 2024
 *      Author: anh
 */
#include "config.h"
#include "string.h"

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"
#include "app_hw_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"


static const char *TAG = "cpu startup";


void app_main_task(void *param){
	(void)param;

	LOGI(TAG, "Call app_main.");
	extern void app_main(void);
	app_main();
	LOGI(TAG, "Return from app_main.");

	vTaskDelete(NULL);
}


int app_initialize(void){
	MPU_Config();

	SCB_EnableICache();
	SCB_EnableDCache();

	HAL_Init();

	SystemClock_Config();
	MX_GPIO_Init();
	MX_UART8_Init();


	LOGI(TAG, "Application startup.");

	xTaskCreate(app_main_task, "app_main_task", 1024, NULL, 5, NULL);

	__NVIC_SetPriority(SVCall_IRQn, 0U);
	vTaskStartScheduler();

	return 0;
}



#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line){
	LOGA("ASSERT", "Assert failed at %s->%lu.", file, line);
}
#endif /* USE_FULL_ASSERT */






