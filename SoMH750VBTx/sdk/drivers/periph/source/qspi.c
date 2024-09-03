/*
 * qspi.c
 *
 *  Created on: Sep 1, 2024
 *      Author: anh
 */

#include "periph/qspi.h"


static uint8_t qspi_configure(void) __attribute__((section(".ram_d3_section")));
static uint8_t qspi_write_enable(void) __attribute__((section(".ram_d3_section")));
static uint8_t qspi_poll_memory_ready(void) __attribute__((section(".ram_d3_section")));

QSPI_HandleTypeDef hqspi = {
	  .Instance = QUADSPI,
	  .Init.ClockPrescaler = 1,
	  .Init.FifoThreshold = 4,
	  .Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE,
	  .Init.FlashSize = 22,
	  .Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE,
	  .Init.ClockMode = QSPI_CLOCK_MODE_0,
	  .Init.FlashID = QSPI_FLASH_ID_1,
	  .Init.DualFlash = QSPI_DUALFLASH_DISABLE,
	  .State = HAL_QSPI_STATE_BUSY_MEM_MAPPED,
	  .Timeout = 5000,
};



static uint8_t qspi_configure(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;
    uint8_t reg_val = 0;


    if (qspi_write_enable() != HAL_OK) {
        return HAL_ERROR;
    }
    /* Read status register 2 operations ------------------------------------------ */
    sCommand.Instruction = READ_STATUS_REG2_CMD;

    if (HAL_QSPI_Command(&hqspi, &sCommand, 25000) != HAL_OK) {
        return HAL_ERROR;
    }
    if(HAL_QSPI_Receive(&hqspi, &reg_val, 25000) != HAL_OK) {
    	return HAL_ERROR;
    }

    sCommand.Instruction = WRITE_STATUS_REG2_CMD;
    sCommand.DataMode = QSPI_DATA_NONE;
    reg_val |= STATUS_REG2_QE;
    if (HAL_QSPI_Command(&hqspi, &sCommand, 25000) != HAL_OK) {
        return HAL_ERROR;
    }
    if (HAL_QSPI_Transmit(&hqspi, &reg_val, 25000) != HAL_OK) {
        return HAL_ERROR;
    }

    sConfig.Match = 0x02;
    sConfig.Mask = 0x02;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    sCommand.Instruction = READ_STATUS_REG2_CMD;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, 25000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

static uint8_t qspi_poll_memory_ready(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;

    /* Configure automatic polling mode to wait for memory ready ------ */
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = READ_STATUS_REG1_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    sConfig.Match = 0x00;
    sConfig.Mask = 0x01;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig,
                             25000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

static uint8_t qspi_write_enable(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;

    /* Enable write operations ------------------------------------------ */
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = WRITE_ENABLE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &sCommand, 25000) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match = 0x02;
    sConfig.Mask = 0x02;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    sCommand.Instruction = READ_STATUS_REG1_CMD;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig,
                             25000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}


uint8_t qspi_erase_sector(uint32_t start_address, uint32_t end_address) {
    QSPI_CommandTypeDef sCommand;

    start_address = start_address - end_address % MEMORY_SECTOR_SIZE;

    /* Erasing Sequence -------------------------------------------------- */
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = SECTOR_ERASE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;

    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    while (end_address >= start_address) {
        sCommand.Address = (start_address & 0x0FFFFFFF);

        if (qspi_write_enable() != HAL_OK) {
            return HAL_ERROR;
        }

        if (HAL_QSPI_Command(&hqspi, &sCommand, 25000)
            != HAL_OK) {
            return HAL_ERROR;
        }
        start_address += MEMORY_SECTOR_SIZE;

        if (qspi_poll_memory_ready() != HAL_OK) {
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}

uint8_t qspi_erase_chip(void) {
    QSPI_CommandTypeDef sCommand;

    if (qspi_write_enable() != HAL_OK) {
        return HAL_ERROR;
    }
    /* Erasing Sequence --------------------------------- */
    sCommand.Instruction = CHIP_ERASE_CMD;
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;


    if (HAL_QSPI_Command(&hqspi, &sCommand, 25000)
        != HAL_OK) {
        return HAL_ERROR;
    }

    if (qspi_poll_memory_ready() != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

uint8_t qspi_write_memory(uint32_t address, uint8_t *buffer, uint32_t buffer_size) {
    QSPI_CommandTypeDef sCommand;
    uint32_t end_addr, current_size, current_addr;

    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    address &= 0x0FFFFFFF;

    if (qspi_write_enable() != HAL_OK) {
        return HAL_ERROR;
    }

    while (current_addr <= address) {
        current_addr += MEMORY_PAGE_SIZE;
    }
    current_size = current_addr - address;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > buffer_size) {
        current_size = buffer_size;
    }

    /* Initialize the adress variables */
    current_addr = address;
    end_addr = address + buffer_size;

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = buffer_size;
    sCommand.Address = address;
    sCommand.DummyCycles = 0;

    /* Perform the write page by page */
    do {
        sCommand.Address = current_addr;
        sCommand.NbData = current_size;

        if (current_size == 0) {
            return HAL_OK;
        }

        /* Enable write operations */
        if (qspi_write_enable() != HAL_OK) {
            return HAL_ERROR;
        }

        /* Configure the command */
        if (HAL_QSPI_Command(&hqspi, &sCommand, 25000)
            != HAL_OK) {

            return HAL_ERROR;
        }

        /* Transmission of the data */
        if (HAL_QSPI_Transmit(&hqspi, buffer, 25000) != HAL_OK) {

            return HAL_ERROR;
        }

        /* Configure automatic polling mode to wait for end of program */
        if (qspi_poll_memory_ready() != HAL_OK) {
            return HAL_ERROR;
        }

        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        buffer += current_size;
        current_size =
            ((current_addr + MEMORY_PAGE_SIZE) > end_addr) ?
            (end_addr - current_addr) : MEMORY_PAGE_SIZE;
    } while (current_addr <= end_addr);

    return HAL_OK;

}


uint8_t qspi_enter_memory_mapped_mode(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_MemoryMappedTypeDef sMemMappedCfg;

    /* Enable Memory-Mapped mode-------------------------------------------------- */

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = 0;
    sCommand.Address = 0;
    sCommand.Instruction = QUAD_OUT_FAST_READ_CMD;
    sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_QUAD;

    sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

    if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

uint8_t qspi_exit_memory_mapped_mode(void){
//	QUADSPI->CR &=~ QUADSPI_CR_EN;
	HAL_QSPI_Abort(&hqspi);

	qspi_configure();

//	QUADSPI->CR |= QUADSPI_CR_EN;
//	QUADSPI->CCR &= (~(QUADSPI_CCR_FMODE));


	return HAL_OK;
}




























