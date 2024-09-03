/*
 * faultanalyzer.h
 *
 *  Created on: Aug 22, 2024
 *      Author: anh
 */

#ifndef SDK_COMPONENTS_FAULTANALYZER_H_
#define SDK_COMPONENTS_FAULTANALYZER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"


#define CALL_STACK_MAX_DEPTH  10
#define DUMP_STACK_DEPTH_SIZE 16


void fault_analyzer_init(void);

void fault_analyze(uint32_t lr, uint32_t sp);

#ifdef __cplusplus
}
#endif

#endif /* SDK_COMPONENTS_FAULTANALYZER_H_ */
