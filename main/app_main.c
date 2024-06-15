

#include <stdint.h>
#include "lib2.h"
#include "stm32h7xx_hal.h"



extern UART_HandleTypeDef huart8;

int xyz = 0;
x_t x;


int app_main(void){


	while(1){
		HAL_UART_Transmit(&huart8, (uint8_t *)"\r\nxxx", 5, 1000);
		xyz++;
		xyz++;
		xxx(&x);
		HAL_Delay(500);
	}
}
