/*
 * port_freertos.c
 *
 *  Created on: Jun 15, 2024
 *      Author: anh
 */
#include "port_freertos.h"
#include "logger.h"




void vApplicationIdleHook(void){

}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName){
	(void)xTask;
	LOGE("RTOS", "Task %s stack overflow", pcTaskName);
}

void vApplicationMallocFailedHook(void){
	LOGE("RTOS", "Memory allocation failed");
}

#if (configSUPPORT_STATIC_ALLOCATION == 1)
void vApplicationGetIdleTaskMemory (StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  static StaticTask_t Idle_TCB;
  static StackType_t  Idle_Stack[configMINIMAL_STACK_SIZE];

  *ppxIdleTaskTCBBuffer   = &Idle_TCB;
  *ppxIdleTaskStackBuffer = &Idle_Stack[0];
  *pulIdleTaskStackSize   = (uint32_t)configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  static StaticTask_t Timer_TCB;
  static StackType_t  Timer_Stack[configTIMER_TASK_STACK_DEPTH];

  *ppxTimerTaskTCBBuffer   = &Timer_TCB;
  *ppxTimerTaskStackBuffer = &Timer_Stack[0];
  *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}
#endif /* (configSUPPORT_STATIC_ALLOCATION == 1) */
