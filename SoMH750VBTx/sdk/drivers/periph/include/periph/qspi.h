/*
 * qspi.h
 *
 *  Created on: Sep 1, 2024
 *      Author: anh
 */

#ifndef SDK_DRIVERS_PERIPH__QSPI_H_
#define SDK_DRIVERS_PERIPH__QSPI_H_

#include "stm32h7xx_hal.h"
#include "memory_section.h"
#include "error_type.h"



extern QSPI_HandleTypeDef hqspi;


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


typedef struct{
	uint16_t commid;
	uint8_t mfid;
	uint8_t devid;
	char mf[10];
	uint32_t sizekb;
} qspi_flash_info_t;


errno_t IRAMFUNC_ATTR qspi_init(void);
errno_t IRAMFUNC_ATTR qspi_deinit(void);

errno_t IRAMFUNC_ATTR qspi_read_info(qspi_flash_info_t *pinfo);

errno_t IRAMFUNC_ATTR qspi_erase_chip(void);
errno_t IRAMFUNC_ATTR qspi_erase_sector(uint32_t saddress, uint32_t eaddress);

errno_t IRAMFUNC_ATTR qspi_write_memory(uint32_t waddress, uint8_t *wbuffer, uint32_t wsize);
errno_t IRAMFUNC_ATTR qspi_read_memmory(uint32_t raddress, uint8_t *rbuffer, uint32_t rsize);

errno_t IRAMFUNC_ATTR qspi_enter_memory_mapped_mode(void);
errno_t IRAMFUNC_ATTR qspi_exit_memory_mapped_mode(void);





#endif /* SDK_DRIVERS_PERIPH__QSPI_H_ */
