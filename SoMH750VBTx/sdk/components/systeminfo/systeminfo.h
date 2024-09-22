/*
 * systeminfo.h
 *
 *  Created on: Sep 1, 2024
 *      Author: anh
 */

#ifndef SDK_COMPONENTS_SYSTEMINFO_H_
#define SDK_COMPONENTS_SYSTEMINFO_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32h7xx.h"



#define REG_UNIQUE_ID 0x1FF1E800




const char *get_project_name(void);
const char *get_build_target(void);
const char *get_build_version(void);
const char *get_build_datetime(void);
const char *get_target_name(void);






uint32_t get_cpu_id(void);
uint16_t get_device_id(void);
uint32_t *get_unique_id(void);






uint32_t get_text_section_start(void);
uint32_t get_text_section_end(void);
uint32_t get_text_section_size(void);

uint32_t get_stack_section_start(void);
uint32_t get_stack_section_end(void);
uint32_t get_stack_section_size(void);


uint32_t get_iflash_section_start(void);
uint32_t get_iflash_section_end(void);
uint32_t get_iflash_section_size(void);

uint32_t get_eflash_section_start(void);
uint32_t get_eflash_section_end(void);
uint32_t get_eflash_section_size(void);

uint32_t get_ramd1_section_start(void);
uint32_t get_ramd1_section_end(void);
uint32_t get_ramd1_section_size(void);

uint32_t get_ramd2_section_start(void);
uint32_t get_ramd2_section_end(void);
uint32_t get_ramd2_section_size(void);

uint32_t get_ramd3_section_start(void);
uint32_t get_ramd3_section_end(void);
uint32_t get_ramd3_section_size(void);

uint32_t get_ramdtcm_section_start(void);
uint32_t get_ramdtcm_section_end(void);
uint32_t get_ramdtcm_section_size(void);

uint32_t get_ramitcm_section_start(void);
uint32_t get_ramitcm_section_end(void);
uint32_t get_ramitcm_section_size(void);




#ifdef __cplusplus
}
#endif

#endif /* SDK_COMPONENTS_SYSTEMINFO_H_ */
