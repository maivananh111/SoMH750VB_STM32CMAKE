/*
 * interrupts.c
 *
 *  Created on: Jun 14, 2024
 *      Author: anh
 */

#include "interrupts.h"
#include "stm32h7xx_hal.h"



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

void SVC_Handler(void){

}

void DebugMon_Handler(void){

}

void PendSV_Handler(void){

}

void SysTick_Handler(void){
	HAL_IncTick();
}

void ETH_IRQHandler(void){

}



