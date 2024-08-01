/*
 * app_hw_init.h
 *
 *  Created on: Jul 29, 2024
 *      Author: anh
 */

#ifndef SDK_STARTUP_APP_HW_INIT_H_
#define SDK_STARTUP_APP_HW_INIT_H_

#ifdef __cplusplus
extern "C"{
#endif


#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"


extern UART_HandleTypeDef huart8;

void SystemClock_Config(void);
void MPU_Config(void);
void MX_GPIO_Init(void);
void MX_UART8_Init(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* SDK_STARTUP_APP_HW_INIT_H_ */
