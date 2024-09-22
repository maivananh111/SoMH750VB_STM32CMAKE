/*
 * qspi.c
 *
 *  Created on: Sep 1, 2024
 *      Author: anh
 */

#include "periph/qspi.h"
#include "string.h"
#include "expression_assert.h"


#define TAG "qspi"


static errno_t IRAMFUNC_ATTR qspi_base_init(void);
static errno_t IRAMFUNC_ATTR _qspi_write_enable(void);
static errno_t IRAMFUNC_ATTR _qspi_poll_mem_ready(void);
static errno_t IRAMFUNC_ATTR _qspi_config_directmode(void);
static errno_t IRAMFUNC_ATTR _qspi_sw_reset(void);

QSPI_HandleTypeDef hqspi = {
	.Instance = QUADSPI,
};



errno_t qspi_init(void) {
	RETURNVAL_ON_FALSE((qspi_base_init() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to initialize peripheral");
    RETURNVAL_ON_FALSE((_qspi_sw_reset() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to reset flash memory chip (software reset)");
    for (uint32_t temp = 0; temp < 10*1000*120; temp++) __NOP();
    RETURNVAL_ON_FALSE((_qspi_poll_mem_ready() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to poll memory ready");
    RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");
    RETURNVAL_ON_FALSE((_qspi_config_directmode() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to set direct mode");

    return ERRNO_NONE;
}

errno_t qspi_deinit(void){
	RETURNVAL_ON_FALSE((HAL_QSPI_DeInit(&hqspi) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to reset peripheral");

	return ERRNO_NONE;
}

errno_t qspi_read_info(qspi_flash_info_t *pinfo) {
	uint8_t data[2];
	QSPI_CommandTypeDef sCommand;

    RETURNVAL_ON_FALSE((_qspi_poll_mem_ready() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to poll memory ready");

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = DEVICEID_READ_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.Address     = 0x00000000;
	sCommand.NbData      = 2;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	sCommand.DummyCycles = 4;
	RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
	RETURNVAL_ON_FALSE((HAL_QSPI_Receive(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to receive");

	pinfo->commid = (uint16_t)(data[0]<<8 | data[1]);
	pinfo->mfid = data[0];
	pinfo->devid = data[1];
	pinfo->sizekb = (1 << (pinfo->devid - 0x13)) * 1024U;
	memset(pinfo->mf, 0, 10);
	switch (pinfo->mfid) {
		case 0x40:
			memcpy(pinfo->mf, "Winbond", 7);
		break;
		case 0x20:
			memcpy(pinfo->mf, "XMC", 3);
		break;
		case 0x70:
			memcpy(pinfo->mf, "Eon", 3);
		break;
		case 0xC2:
			memcpy(pinfo->mf, "MXIC", 4);
		break;
		case 0xC8:
			memcpy(pinfo->mf, "GigaDevice", 10);
		break;
		default:
			memcpy(pinfo->mf, "Unknown", 7);
		break;
	}

	return ERRNO_NONE;
}

errno_t qspi_erase_chip(void) {
    QSPI_CommandTypeDef sCommand;

    RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");

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

    RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
    RETURNVAL_ON_FALSE((_qspi_poll_mem_ready() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to poll memory ready");

    return ERRNO_NONE;
}

errno_t qspi_erase_sector(uint32_t saddress, uint32_t eaddress) {
    QSPI_CommandTypeDef sCommand;

    saddress = saddress - saddress % MEMORY_SECTOR_SIZE;
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

    while (eaddress >= saddress) {
        sCommand.Address = (saddress & 0x0FFFFFFF);
        RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");
        RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
        saddress += MEMORY_SECTOR_SIZE;
        RETURNVAL_ON_FALSE((_qspi_poll_mem_ready() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to poll memory ready");
    }

    return ERRNO_NONE;
}

errno_t qspi_write_memory(uint32_t waddress, uint8_t *wbuffer, uint32_t wsize) {
    QSPI_CommandTypeDef sCommand;
    uint32_t end_addr, current_size, current_addr;

    current_addr = 0;
    waddress &= 0x0FFFFFFF;
    RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");

    while (current_addr <= waddress) current_addr += MEMORY_PAGE_SIZE;
    current_size = current_addr - waddress;
    if (current_size > wsize) current_size = wsize;

    current_addr = waddress;
    end_addr = waddress + wsize;
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = wsize;
    sCommand.Address = waddress;
    sCommand.DummyCycles = 0;

    do {
        sCommand.Address = current_addr;
        sCommand.NbData = current_size;
        if (current_size == 0) return ERRNO_NONE;

        RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");
        RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
        RETURNVAL_ON_FALSE((HAL_QSPI_Transmit(&hqspi, wbuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to transmit");
        RETURNVAL_ON_FALSE((_qspi_poll_mem_ready() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to poll memory ready");

        current_addr += current_size;
        wbuffer += current_size;
        current_size = ((current_addr + MEMORY_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : MEMORY_PAGE_SIZE;
    }
    while (current_addr <= end_addr);

    return ERRNO_NONE;
}

errno_t qspi_read_memmory(uint32_t raddress, uint8_t *rbuffer, uint32_t rsize){
	QSPI_CommandTypeDef sCommand;

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = QUAD_OUT_FAST_READ_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
	sCommand.Address = raddress;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_4_LINES;
	sCommand.DummyCycles = 6U;
	sCommand.NbData = rsize;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
	MODIFY_REG(hqspi.Instance->DCR, QUADSPI_DCR_CSHT, QSPI_CS_HIGH_TIME_3_CYCLE);
	RETURNVAL_ON_FALSE((HAL_QSPI_Receive(&hqspi, rbuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to receive");
	MODIFY_REG(hqspi.Instance->DCR, QUADSPI_DCR_CSHT, QSPI_CS_HIGH_TIME_6_CYCLE);

	return ERRNO_NONE;
}

errno_t qspi_enter_memory_mapped_mode(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_MemoryMappedTypeDef sMemMappedCfg;

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
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
    RETURNVAL_ON_FALSE((HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to enable memory mapped");

    return ERRNO_NONE;
}

errno_t qspi_exit_memory_mapped_mode(void){
	RETURNVAL_ON_FALSE((HAL_QSPI_DeInit(&hqspi) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to reset peripheral");
	RETURNVAL_ON_FALSE((qspi_base_init() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to initialize peripheral");
    RETURNVAL_ON_FALSE((_qspi_sw_reset() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to reset flash memory chip (software reset)");
    HAL_Delay(10);
    RETURNVAL_ON_FALSE((_qspi_poll_mem_ready() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to poll memory ready");
    RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");
    RETURNVAL_ON_FALSE((_qspi_config_directmode() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to set direct mode");

    return ERRNO_NONE;
}






static errno_t qspi_base_init(void) {
	hqspi.Instance = QUADSPI;
	hqspi.Init.ClockPrescaler = 1;
	hqspi.Init.FifoThreshold = 4;
	hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	hqspi.Init.FlashSize = 22;
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	hqspi.Init.FlashID = QSPI_FLASH_ID_1;
	hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
	RETURNVAL_ON_FALSE((HAL_QSPI_Init(&hqspi) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to base initialize");

	return ERRNO_NONE;
}

static errno_t _qspi_write_enable(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = WRITE_ENABLE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");

    sConfig.Match = 0x02;
    sConfig.Mask = 0x02;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
    sCommand.Instruction = READ_STATUS_REG1_CMD;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    RETURNVAL_ON_FALSE((HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");

    return ERRNO_NONE;
}

static errno_t _qspi_sw_reset(void) {
    QSPI_CommandTypeDef sCommand;

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_ENABLE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");

    for (uint32_t temp = 0; temp < 256; temp++) __NOP();

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_EXECUTE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");

    return ERRNO_NONE;
}

static errno_t _qspi_poll_mem_ready(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;

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
    RETURNVAL_ON_FALSE((HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to poll");

    return ERRNO_NONE;
}

static errno_t _qspi_config_directmode(void) {
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;
    uint8_t reg_val = 0;

    RETURNVAL_ON_FALSE((_qspi_write_enable() == ERRNO_NONE), ERRNO_EXEC_FAILED, TAG, "Fail to enable flash memory write");

    sCommand.Instruction = READ_STATUS_REG2_CMD;
    RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
    RETURNVAL_ON_FALSE((HAL_QSPI_Receive(&hqspi, &reg_val, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to receive");

    sCommand.Instruction = WRITE_STATUS_REG2_CMD;
    sCommand.DataMode = QSPI_DATA_NONE;
    reg_val |= STATUS_REG2_QE;
    RETURNVAL_ON_FALSE((HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to send command");
    RETURNVAL_ON_FALSE((HAL_QSPI_Transmit(&hqspi, &reg_val, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to transmit");

    sConfig.Match = 0x02;
    sConfig.Mask = 0x02;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
    sCommand.Instruction = READ_STATUS_REG2_CMD;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    RETURNVAL_ON_FALSE((HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) == HAL_OK), ERRNO_EXEC_FAILED, TAG, "Fail to poll");

    return ERRNO_NONE;
}


