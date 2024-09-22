/*
 * macro.h
 *
 *  Created on: Aug 5, 2023
 *      Author: anh
 */

#ifndef SDK_COMMON_MACRO_H_
#define SDK_COMMON_MACRO_H_

#ifdef __cplusplus
extern "C"{
#endif





#define bytes_to_words(bytes)                      		  (bytes/4U)
#define bytes_to_kilobytes(bytes)                      	  (bytes/1024U)
#define words_to_bytes(words)							  (words*4U)
#define words_to_kilobytes(words)						  ((words*4U)/1024U)
#define kilobytes_to_bytes(kbytes)                        (kbytes*1024U)
#define kilobytes_to_words(kbytes)                        ((kbytes*1024U)/4U)

#define make_string(s) #s

#define delay_us_nop(__US__)                             			\
	do {   															\
		uint64_t ovrtick = __US__*4UL*FCPU/1000000UL;				\
		for(uint64_t tick=0; tick<ovrtick; tick++) __NOP();			\
	} while(0)



#ifdef __cplusplus
}
#endif

#endif /* SDK_COMMON_MACRO_H_ */
