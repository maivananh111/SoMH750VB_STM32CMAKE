/*
 * port_logger.c
 *
 *  Created on: Jul 29, 2024
 *      Author: anh
 */

#include "logger.h"

#include "stm32h7xx_hal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



void log_port_flush_string(char *string, uint16_t lenght){
	extern UART_HandleTypeDef huart8;
	HAL_UART_Transmit(&huart8, (uint8_t *)string, lenght, 1000);
}

uint32_t log_port_get_systime(void){
	return (uint32_t)xTaskGetTickCount();
}
