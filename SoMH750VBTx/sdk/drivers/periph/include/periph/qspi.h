/*
 * qspi.h
 *
 *  Created on: Sep 1, 2024
 *      Author: anh
 */

#ifndef SDK_DRIVERS_PERIPH__QSPI_H_
#define SDK_DRIVERS_PERIPH__QSPI_H_


#include "stm32h7xx_hal.h"



/*XM25QH64C memory parameters*/
#define MEMORY_FLASH_SIZE               0x200000  /* 16 MBits*/
#define MEMORY_SECTOR_SIZE              0x1000    /* 4kBytes */
#define MEMORY_SECTOR_NUM 				0x800     /* 128*16 sector */
#define MEMORY_PAGE_SIZE                0x100     /* 256 bytes */



/*XM25QH64C  commands */
#define RESET_ENABLE_CMD  0x66//
#define RESET_EXECUTE_CMD 0x99//

#define WRITE_ENABLE_CMD   0x06//
#define ENTER_QPI_MODE_CMD 0x38

#define READ_STATUS_REG1_CMD  0x05//
#define READ_STATUS_REG2_CMD  0x35//
#define READ_STATUS_REG3_CMD  0x15//
#define WRITE_STATUS_REG1_CMD 0x01//
#define WRITE_STATUS_REG2_CMD 0x31//
#define WRITE_STATUS_REG3_CMD 0x11//

#define STATUS_REG2_QE        0x02
#define STATUS_REG3_DRV_MASK  0x60

#define SECTOR_ERASE_CMD 0x20//
#define CHIP_ERASE_CMD   0xC7//

#define QUAD_IN_FAST_PROG_CMD 0x33//32
#define QUAD_OUT_FAST_READ_CMD 0xEB//eb
#define DUMMY_CLOCK_CYCLES_READ_QUAD 6

#define DEVICEID_READ_CMD 0x94
#define UNIQUEID_READ_CMD 0x4B
#define JEDECID_READ_CMD 0x9F



uint8_t qspi_erase_sector(uint32_t start_address, uint32_t end_address) __attribute__((section(".ram_d3_section")));
uint8_t qspi_write_memory(uint32_t address, uint8_t *buffer, uint32_t buffer_size) __attribute__((section(".ram_d3_section")));
uint8_t qspi_enter_memory_mapped_mode(void) __attribute__((section(".ram_d3_section")));
uint8_t qspi_exit_memory_mapped_mode(void) __attribute__((section(".ram_d3_section")));
uint8_t qspi_erase_chip(void) __attribute__((section(".ram_d3_section")));



#endif /* SDK_DRIVERS_PERIPH__QSPI_H_ */
