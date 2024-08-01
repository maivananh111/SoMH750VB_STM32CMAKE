/*
 * app_hw_mspinit.c
 *
 *  Created on: Jul 29, 2024
 *      Author: anh
 */

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"



extern void Error_Handler(void);

/**************************************************** HAL PERIPHERAL MSP INIT *********************************************************/
void HAL_MspInit(void){
	__HAL_RCC_SYSCFG_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if(huart->Instance==UART8){
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART8;
		PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
			Error_Handler();

		__HAL_RCC_UART8_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();
		/**UART8 GPIO Configuration
		PE0     ------> UART8_RX
		PE1     ------> UART8_TX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart){
	if(huart->Instance==UART8){
		__HAL_RCC_UART8_CLK_DISABLE();

		/**UART8 GPIO Configuration
		PE0     ------> UART8_RX
		PE1     ------> UART8_TX
		*/
		HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0|GPIO_PIN_1);
	}
}