/*
 * memory_section.h
 *
 *  Created on: Sep 22, 2024
 *      Author: anh
 */

#ifndef SDK_COMMON_MEMORY_SECTION_H_
#define SDK_COMMON_MEMORY_SECTION_H_


#define IFLASH_ATTR 			__attribute__((section(".section_data_iflash")))
#define EFLASH_ATTR 			__attribute__((section(".section_data_eflash")))

#define IRAMD1_ATTR 			__attribute__((section(".section_ramd1")))
#define IRAMD2_ATTR 			__attribute__((section(".section_ramd2")))
#define IRAMD3_ATTR 			__attribute__((section(".section_ramd3")))
#define IRAMDTCM_ATTR			__attribute__((section(".section_ramdtcm")))
#define IRAMITCM_ATTR			__attribute__((section(".section_ramitcm")))

#define IRAMFUNC_ATTR			__attribute__((section(".section_function_in_ram")))


#endif /* SDK_COMMON_MEMORY_SECTION_H_ */
