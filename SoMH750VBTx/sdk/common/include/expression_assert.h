/*
 * expression_assert.h
 *
 *  Created on: Jun 19, 2023
 *      Author: anh
 */

#ifndef SDK_COMMON_EXPRESSION_ASSERT_H_
#define SDK_COMMON_EXPRESSION_ASSERT_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "stm32h7xx.h"
#include "logger.h"




#define RETURN_ON_FALSE(__EXPRESSION__, __TAG__, __FORMAT__, ...) {\
	if(!__EXPRESSION__){\
		(__TAG__ != NULL)? LOGA(__TAG__, "Assert failed at %s->%s[%d]>>> " __FORMAT__, __FILE__, __func__, __LINE__, ##__VA_ARGS__) : (void)0; \
		return;\
	}\
}

#define RETURNVAL_ON_FALSE(__EXPRESSION__, __RETURNCODE__, __TAG__, __FORMAT__, ...) {\
	if(!__EXPRESSION__){\
		(__TAG__ != NULL)? LOGA(__TAG__, "Assert failed at %s->%s[%d]>>> " __FORMAT__, __FILE__, __func__, __LINE__, ##__VA_ARGS__) : (void)0; \
		return __RETURNCODE__;\
	}\
}


#define GOTO_ON_FALSE(__EXPRESSION__, __LABEL__, __TAG__, __FORMAT__, ...) {\
	if(!__EXPRESSION__){\
		(__TAG__ != NULL)? LOGA(__TAG__, "Assert failed at %s->%s[%d]>>> " __FORMAT__, __FILE__, __func__, __LINE__, ##__VA_ARGS__) : (void)0; \
		goto __LABEL__;\
	}\
}


#define BREAK_ON_FALSE(__EXPRESSION__, __TAG__, __FORMAT__, ...) {\
	if(!__EXPRESSION__){\
		(__TAG__ != NULL)? LOGA(__TAG__,  "Assert failed at %s->%s[%d]>>> " __FORMAT__, __FILE__, __func__, __LINE__, ##__VA_ARGS__) : (void)0; \
		break;\
	}\
}

#define CONTINUE_ON_FALSE(__EXPRESSION__, __TAG__, __FORMAT__, ...) {\
	if(!__EXPRESSION__){\
		(__TAG__ != NULL)? LOGA(__TAG__,  "Assert failed at %s->%s[%d]>>> " __FORMAT__, __FILE__, __func__, __LINE__, ##__VA_ARGS__) : (void)0; \
		continue;\
	}\
}

#ifdef __cplusplus
}
#endif

#endif /* SDK_COMMON_EXPRESSION_ASSERT_H_ */
