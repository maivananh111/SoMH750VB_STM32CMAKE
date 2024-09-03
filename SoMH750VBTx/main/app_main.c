

#include <macros.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "config.h"
#include "logger.h"
#include "string.h"
#include "stm32h7xx.h"
#include "periph/qspi.h"


extern const unsigned char _binary_file_xyz_txt[];
static const char *TAG = "MAIN";


void write_flash(void) __attribute__((section(".ram_d3_section")));

void write_flash(void){
	LOGW("QSPI", "start write to flash");

	__disable_irq();
	qspi_exit_memory_mapped_mode();

	if(qspi_erase_sector(0x400000, 0x4000FF) != HAL_OK)
		LOGE(TAG, "qspi erase sector failed");

	if(qspi_write_memory(0x400000, (uint8_t *)"abcd", 4) != HAL_OK)
		LOGE(TAG, "qspi write memory failed");

	qspi_enter_memory_mapped_mode();
	__enable_irq();

	LOGW("QSPI", "end write to flash");
}



void task1(void *param){
	(void)param;

	while(1){
		write_flash();

		vTaskDelay(1000);
//		LOGI("II", "%s", z);
	}
}

void app_main(void){
	log_set_filtermax_style(TAG, LOG_VERBOSE, LOG_STYLE_DEFAULT);

	xTaskCreate(task1, "task1", bytes_to_words(1024), NULL, 2, NULL);
}
