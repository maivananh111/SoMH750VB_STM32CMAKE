/*
 * expression_check.h
 *
 *  Created on: Jun 19, 2023
 *      Author: anh
 */

#ifndef SDK_COMMON_EXPRESSION_CHECK_H_
#define SDK_COMMON_EXPRESSION_CHECK_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "stm32h7xx.h"
#include "logger.h"



#define RETURN_ON_FALSE(__EXPRESSION__, __TAG__, __FORMAT__, ...) {\
	if(__EXPRESSION__){\
		LOGA(__TAG__, "%s[%d]>>> " __FORMAT__, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__);\
		return;\
	}\
}

#define RETURNVAL_ON_FALSE(__EXPRESSION__, __RETURNCODE__, __TAG__, __FORMAT__, ...) {\
	if(__EXPRESSION__){\
		LOGA(__TAG__, "%s[%d]>>> " __FORMAT__, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__);\
		return __RETURNCODE__;\
	}\
}


#define GOTO_ON_FALSE(__EXPRESSION__, __LABEL__, __TAG__, __FORMAT__, ...) {\
	if(__EXPRESSION__){\
		LOGA(__TAG__, "%s[%d]>>> " __FORMAT__, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__);\
		goto __LABEL__;\
	}\
}


#define BREAK_ON_FALSE(__EXPRESSION__, __TAG__, __FORMAT__, ...) {\
	if(__EXPRESSION__){\
		LOGA(__TAG__, "%s[%d]>>> " __FORMAT__, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__);\
		break;\
	}\
}

#define CONTINUE_ON_FALSE(__EXPRESSION__, __TAG__, __FORMAT__, ...) {\
	if(__EXPRESSION__){\
		LOGA(__TAG__, "%s[%d]>>> " __FORMAT__, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__);\
		continue;\
	}\
}

#ifdef __cplusplus
}
#endif

#endif /* SDK_COMMON_EXPRESSION_CHECK_H_ */
