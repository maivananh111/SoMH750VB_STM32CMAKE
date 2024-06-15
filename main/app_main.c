

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern UART_HandleTypeDef huart8;

void task1(void *){

	while(1){
		HAL_UART_Transmit(&huart8, (uint8_t *)"2222222xxx\r\n", 12, 1000);
		vTaskDelay(1200);
	}
}

void app_main(void *param){
	(void)param;

	xTaskCreate(task1, "task1", 1024, NULL, 2, NULL);

	while(1){
		HAL_UART_Transmit(&huart8, (uint8_t *)"aaa\r\n", 5, 1000);
		vTaskDelay(500);
	}
}
