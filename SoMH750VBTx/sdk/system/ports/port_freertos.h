/*
 * port_freertos.h
 *
 *  Created on: Jun 15, 2024
 *      Author: anh
 */

#ifndef SDK_SYSTEM_PORTS_PORT_FREERTOS_H_
#define SDK_SYSTEM_PORTS_PORT_FREERTOS_H_

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


#endif /* SDK_SYSTEM_PORTS_PORT_FREERTOS_H_ */
