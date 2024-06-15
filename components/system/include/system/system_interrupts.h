/*
 * interrupts.h
 *
 *  Created on: Jun 14, 2024
 *      Author: anh
 */

#ifndef COMPONENTS_SYSTEM_SYSTEM_INTERRUPTS_H_
#define COMPONENTS_SYSTEM_SYSTEM_INTERRUPTS_H_


#ifdef __cplusplus
extern "C" {
#endif


void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void ETH_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_SYSTEM_SYSTEM_INTERRUPTS_H_ */
