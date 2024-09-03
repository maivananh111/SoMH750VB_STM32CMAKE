/*
 * port_logger.c
 *
 *  Created on: Jul 29, 2024
 *      Author: anh
 */

#include "logger.h"

#include "stdio.h"

#include "stm32h7xx_hal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern UART_HandleTypeDef huart8;

void log_port_flush_string(char *string, uint16_t length){
//	HAL_UART_Transmit(&huart8, (uint8_t *)string, lenght, 1000);
	while(length--){
		while(!(huart8.Instance->ISR & USART_ISR_TXE_TXFNF));
		huart8.Instance->TDR = (uint8_t)(*string++);
		while(!(huart8.Instance->ISR & USART_ISR_TC));
	}
}

uint32_t log_port_get_systime(void){
	return (uint32_t)xTaskGetTickCount();
}


int _write(int file, char *ptr, int len) {
	(void) file;

	while (len--) {
		while (!(huart8.Instance->ISR & USART_ISR_TXE_TXFNF));
		huart8.Instance->TDR = (uint8_t) (*ptr++);
		while (!(huart8.Instance->ISR & USART_ISR_TC));
	}

	return len;
}
