/*
 * interrupts.c
 *
 *  Created on: Jun 14, 2024
 *      Author: anh
 */

#include "interrupts.h"

#include "stm32h7xx_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"



void NMI_Handler(void){
	while(1){

	}
}

void HardFault_Handler(void){
	while(1){

	}
}

void MemManage_Handler(void){
	while(1){

	}
}

void BusFault_Handler(void){
	while(1){

	}
}

void UsageFault_Handler(void){
	while(1){

	}
}

void DebugMon_Handler(void){

}

void SysTick_Handler(void){
	HAL_IncTick();

	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED){
		xPortSysTickHandler();
	}
}

void ETH_IRQHandler(void){

}



