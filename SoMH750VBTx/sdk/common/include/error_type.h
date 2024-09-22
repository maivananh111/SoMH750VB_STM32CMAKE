/*
 * error_type.h
 *
 *  Created on: Oct 12, 2022
 *      Author: anh
 */

#ifndef SDK_COMMON_ERROR_TYPE_H_
#define SDK_COMMON_ERROR_TYPE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "macros.h"



typedef enum{
	ERRNO_NONE                = 0x0000,   /*!< No error */
	ERRNO_EXEC_FAILED         = 0x0001,   /*!< Operation execute failed */
	ERRNO_NO_MEM              = 0x0002,   /*!< Out of memory */
	ERRNO_INVALID_ARG         = 0x0004,   /*!< Invalid argument */
	ERRNO_INVALID_STATE       = 0x0008,   /*!< Invalid state */
	ERRNO_INVALID_SIZE        = 0x0010,   /*!< Invalid size */
	ERRNO_NOT_FOUND           = 0x0020,   /*!< Requested resource not found */
	ERRNO_NOT_SUPPORTED       = 0x0040,   /*!< Operation or feature not supported */
	ERRNO_TIMEOUT             = 0x0080,   /*!< Operation timed out */
	ERRNO_INVALID_RESPONSE    = 0x0100,   /*!< Received response was invalid */
	ERRNO_INVALID_CRC         = 0x0200,   /*!< CRC or checksum was invalid */
	ERRNO_INVALID_VERSION     = 0x0400,   /*!< Version was invalid */
	ERRNO_INVALID_MAC         = 0x0800,   /*!< MAC address was invalid */
	ERRNO_NOT_FINISHED        = 0x1000,   /*!< Operation has not fully completed */
	ERRNO_NOT_ALLOWED         = 0x2000,   /*!< Operation is not allowed */
} errno_t;



#define IS_ERRNO_NONE(__err__)			   if(__err__ == ERRNO_NONE)
#define IS_ERRNO_EXEC_FAILED(__err__)      if(__err__ == ERRNO_EXEC_FAILED)
#define IS_ERRNO_NO_MEM(__err__)           if(__err__ == ERRNO_NO_MEM)
#define IS_ERRNO_INVALID_ARG(__err__)      if(__err__ == ERRNO_INVALID_ARG)
#define IS_ERRNO_INVALID_STATE(__err__)    if(__err__ == ERRNO_INVALID_STATE)
#define IS_ERRNO_INVALID_SIZE(__err__)     if(__err__ == ERRNO_INVALID_SIZE)
#define IS_ERRNO_NOT_FOUND(__err__)        if(__err__ == ERRNO_NOT_FOUND)
#define IS_ERRNO_NOT_SUPPORTED(__err__)    if(__err__ == ERRNO_NOT_SUPPORTED)
#define IS_ERRNO_TIMEOUT(__err__)          if(__err__ == ERRNO_TIMEOUT)
#define IS_ERRNO_INVALID_RESPONSE(__err__) if(__err__ == ERRNO_INVALID_RESPONSE)
#define IS_ERRNO_INVALID_CRC(__err__)      if(__err__ == ERRNO_INVALID_CRC)
#define IS_ERRNO_INVALID_VERSION(__err__)  if(__err__ == ERRNO_INVALID_VERSION)
#define IS_ERRNO_INVALID_MAC(__err__)      if(__err__ == ERRNO_INVALID_MAC)
#define IS_ERRNO_NOT_FINISHED(__err__)     if(__err__ == ERRNO_NOT_FINISHED)
#define IS_ERRNO_NOT_ALLOWED(__err__)      if(__err__ == ERRNO_NOT_ALLOWED)


#ifdef __cplusplus
}
#endif

#endif /* SDK_COMMON_ERROR_TYPE_H_ */
