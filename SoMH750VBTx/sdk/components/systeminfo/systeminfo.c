/*
 * systeminfo.c
 *
 *  Created on: Sep 1, 2024
 *      Author: anh
 */

#include "systeminfo.h"




extern int _stext;	  /* start of FLASH */
extern int _etext;    /* end of FLASH */

extern int _sstack;   /* start of RAM */
extern int _estack;   /* end of RAM */


extern int _siflash;  /* start of IFLASH */
extern int _eiflash;  /* end of IFLASH */
extern int _seflash;  /* start of EFLASH */
extern int _eeflash;  /* end of EFLASH */

extern int _sramd1;	  /* start of RAM_D1 */
extern int _eramd1;   /* end of RAM_D1 */
extern int _sramd2;	  /* start of RAM_D2 */
extern int _eramd2;   /* end of RAM_D2 */
extern int _sramd3;	  /* start of RAM_D3 */
extern int _eramd3;   /* end of RAM_D3 */
extern int _sramdtcm; /* start of DTCMRAM */
extern int _eramdtcm; /* end of DTCMRAM */
extern int _sramitcm; /* start of ITCMRAM */
extern int _eramitcm; /* end of ITCMRAM */

static uint32_t uniqueid[3];


const char *get_project_name(void){
	return __PROJECT_NAME__;
}

const char *get_build_target(void){
	return __BUILD_TARGET__;
}

const char *get_build_version(void){
	return __BUILD_VERSION__;
}

const char *get_build_datetime(void){
	return __DATE__ " " __TIME__;
}

const char *get_target_name(void){
	return __TARGET_NAME__;
}








uint32_t get_cpu_id(void){
	return SCB->CPUID;
}

uint16_t get_device_id(void){
	return ((DBGMCU -> IDCODE) & 0x0FFFU);
}

uint32_t *get_unique_id(void){
	uniqueid[0] = *(__IO uint32_t *)(REG_UNIQUE_ID);
	uniqueid[1] = *(__IO uint32_t *)(REG_UNIQUE_ID + 4);
	uniqueid[2] = *(__IO uint32_t *)(REG_UNIQUE_ID + 8);

	return uniqueid;
}






uint32_t get_text_section_start(void){
	return (uint32_t)&_stext;
}
uint32_t get_text_section_end(void){
	return (uint32_t)&_etext;
}
uint32_t get_text_section_size(void){
	return (uint32_t)&_etext - (uint32_t)&_stext;
}

uint32_t get_stack_section_start(void){
	return (uint32_t)&_sstack;
}
uint32_t get_stack_section_end(void){
	return (uint32_t)&_estack;
}
uint32_t get_stack_section_size(void){
	return (uint32_t)&_estack - (uint32_t)&_sstack;
}


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

