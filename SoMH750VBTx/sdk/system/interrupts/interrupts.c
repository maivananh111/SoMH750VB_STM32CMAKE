/*
 * interrupts.c
 *
 *  Created on: Jun 14, 2024
 *      Author: anh
 */

#include "interrupts.h"

#include "stdio.h"
#include "stm32h7xx_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"
#include "faultanalyzer.h"
#include "logger.h"



void NMI_Handler(void){
	while(1){

	}
}

void HardFault_Handler(void){
	LOG_BASE("\r\n\r\nException was unhandled (Hard fault).");

	__asm volatile (
        "tst lr, #4            \n"
		"ite eq                \n"
        "mrseq r1, msp         \n"
        "mrsne r1, psp         \n"
        "mov r0, lr            \n"
        "bl fault_analyze      \n"
        "b .                   \n"
    );

	while(1){

	}
}

void MemManage_Handler(void){
	LOG_BASE("\r\n\r\nException was unhandled (Memory manager fault).");

	__asm volatile (
        "tst lr, #4            \n"
		"ite eq                \n"
        "mrseq r1, msp         \n"
        "mrsne r1, psp         \n"
        "mov r0, lr            \n"
        "bl fault_analyze      \n"
        "b .                   \n"
    );

	while(1){

	}
}

void BusFault_Handler(void){
	LOG_BASE("\r\n\r\nException was unhandled (Bus fault).");

	__asm volatile (
        "tst lr, #4            \n"
		"ite eq                \n"
        "mrseq r1, msp         \n"
        "mrsne r1, psp         \n"
        "mov r0, lr            \n"
        "bl fault_analyze      \n"
        "b .                   \n"
    );

	while(1){

	}
}

void UsageFault_Handler(void){
	LOG_BASE("\r\n\r\nException was unhandled (Usage fault).");

	__asm volatile (
        "tst lr, #4            \n"
		"ite eq                \n"
        "mrseq r1, msp         \n"
        "mrsne r1, psp         \n"
        "mov r0, lr            \n"
        "bl fault_analyze      \n"
        "b .                   \n"
    );

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



