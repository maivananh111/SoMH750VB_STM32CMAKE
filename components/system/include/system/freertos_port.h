/*
 * freertos_port.h
 *
 *  Created on: Jun 15, 2024
 *      Author: anh
 */

#ifndef COMPONENTS_SYSTEM_FREERTOS_PORT_H_
#define COMPONENTS_SYSTEM_FREERTOS_PORT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#ifdef __cplusplus
extern "C" {
#endif

void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

#ifdef __cplusplus
}
#endif


#endif /* COMPONENTS_SYSTEM_FREERTOS_PORT_H_ */
