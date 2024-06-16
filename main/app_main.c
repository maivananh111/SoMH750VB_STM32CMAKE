

#include <stdint.h>
#include "config.h"
#include "stm32h7xx_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern unsigned char _binary_xyz_txt[];
extern unsigned int _binary_xyz_txt_len;

extern UART_HandleTypeDef huart8;

void task1(void *param){
	(void)param;
	HAL_UART_Transmit(&huart8, _binary_xyz_txt, _binary_xyz_txt_len, 1000);
	while(1){

		vTaskDelay(1200);
		if (1) {
			
		}
	}

}

void app_main(void *param){
	(void)param;

	xTaskCreate(task1, "task1", 1024, NULL, 2, NULL);

	while(1){
		HAL_UART_Transmit(&huart8, (uint8_t *)"ccc\r\n", 5, 1000);
		vTaskDelay(500);
	}
}
