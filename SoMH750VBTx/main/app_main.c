

#include <stdint.h>

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "config.h"
#include "logger.h"
#include "macro.h"

//.s
extern const unsigned char _binary_file_xyz_txt[];
extern UART_HandleTypeDef huart8;
static const char *TAG = "MAIN";



void restore_context(void){
//	#define BOOT_PC_ADDRESS  0x38000010UL
//	#define BOOT_MSP_ADDRESS 0x38000014UL
//	#define APP_PC_ADDRESS   0x38000020UL
//	#define APP_MSP_ADDRESS  0x38000024UL
//
//	__IO uint32_t app_msp, app_pc;
//	__IO uint32_t boot_msp, boot_pc;
	void (*u_app_program)(void);

	/**
	 * Save machine states.
	 */
//    __asm volatile ("mrs %0, msp" : "=r" (app_msp));
//    __asm volatile ("mov %0, pc" : "=r" (app_pc));
//    *((__IO uint32_t*)APP_PC_ADDRESS) = app_pc;
//    *((__IO uint32_t*)APP_MSP_ADDRESS) = app_msp;

	/**
	 * Jump to application.
	 */
//    boot_pc = *((volatile uint32_t*)BOOT_PC_ADDRESS)+8;
//    boot_msp = *((volatile uint32_t*)BOOT_MSP_ADDRESS);
//    LOGI(TAG, "bootloader pc = 0x%x, msp = 0x%x", boot_pc, boot_msp);

	SCB_DisableDCache();
	SCB_DisableICache();
	__disable_irq();
	u_app_program = (void (*)(void)) (*(uint32_t*) (0x08010000 + 4));
	__set_MSP(*(__IO uint32_t*) (0x08010000));
	u_app_program();
}

void task1(void *param){
	(void)param;

	while(1){
		float x = 123.45;
//		LOGE(TAG, "x = %f", x);
//		LOGA(TAG, "x = %f", x);
//		LOGW(TAG, "x = %f", x);
//		LOGI(TAG, "x = %f", x);
		LOGD(TAG, "x = %f", x);
		LOGV(TAG, "x = %f", x);
		LOGE_P("EE", "x = %f", x);
		vTaskDelay(500);
		LOGA_P("FF", "x = %f", x);
		vTaskDelay(500);
		LOGW_P("GG", "x = %f", x);
		vTaskDelay(500);
		LOGI_P("HH", "x = %f", x);
		vTaskDelay(500);

		restore_context();
//		LOGI("II", "%s", _binary_file_xyz_txt);
	}
}

void app_main(void){
	log_set_filtermax_style(TAG, LOG_VERBOSE, LOG_STYLE_DEFAULT);

	xTaskCreate(task1, "task1", 1024, NULL, 2, NULL);
}
