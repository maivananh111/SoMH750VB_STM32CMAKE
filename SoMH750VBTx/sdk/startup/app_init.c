/*
 * app_init.c
 *
 *  Created on: Jun 15, 2024
 *      Author: anh
 */
#include "config.h"
#include "string.h"

#include "app_hw_init.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"
#include "faultanalyzer.h"
#include "systeminfo.h"



#define TAG "app"



static void log_system_info(void);

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

	fault_analyzer_init();

	log_system_info();

	xTaskCreate(app_main_task, "app_main_task", 1024, NULL, 5, NULL);

	__NVIC_SetPriority(SVCall_IRQn, 0U);
	vTaskStartScheduler();

	return 0;
}




static void log_system_info(void){
	LOGI(TAG, "Application startup.");
	LOGI(TAG, "%s %s[%s]",       get_project_name(), get_build_version(), get_build_datetime());
	LOGI(TAG, "Build target: %s",   get_build_target());
	LOGI(TAG, "Target name : %s",   get_target_name());
	LOGI(TAG, "Target id   : 0x%x", get_cpu_id());
	LOGI(TAG, "Device id   : 0x%x", get_device_id());
	LOGI(TAG, "Unique id   : 0x%x%x%x", get_unique_id()[2], get_unique_id()[1], get_unique_id()[0]);
}


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line){
	LOGA("ASSERT", "Assert failed at %s->%lu.", file, line);
}
#endif /* USE_FULL_ASSERT */






