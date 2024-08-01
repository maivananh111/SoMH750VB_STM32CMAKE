/*
 * error_check.h
 *
 *  Created on: Oct 12, 2022
 *      Author: anh
 */

#ifndef SDK_COMMON_ERROR_CHECK_H_
#define SDK_COMMON_ERROR_CHECK_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "macro.h"

typedef enum{
	ERR_NONE                = 0x00,   /*!< No error */
	ERR_NO_MEM              = 0x01,   /*!< Out of memory */
	ERR_INVALID_ARG         = 0x02,   /*!< Invalid argument */
	ERR_INVALID_STATE       = 0x03,   /*!< Invalid state */
	ERR_INVALID_SIZE        = 0x04,   /*!< Invalid size */
	ERR_NOT_FOUND           = 0x05,   /*!< Requested resource not found */
	ERR_NOT_SUPPORTED       = 0x06,   /*!< Operation or feature not supported */
	ERR_TIMEOUT             = 0x07,   /*!< Operation timed out */
	ERR_INVALID_RESPONSE    = 0x08,   /*!< Received response was invalid */
	ERR_INVALID_CRC         = 0x09,   /*!< CRC or checksum was invalid */
	ERR_INVALID_VERSION     = 0x0A,   /*!< Version was invalid */
	ERR_INVALID_MAC         = 0x0B,   /*!< MAC address was invalid */
	ERR_NOT_FINISHED        = 0x0C,   /*!< Operation has not fully completed */
	ERR_NOT_ALLOWED         = 0x0D,   /*!< Operation is not allowed */
} err_t;


typedef struct {
	err_t id = ERR_NONE;
	int line = 0U;
} err_prop_t;



#define ERROR_CAPTURE(__err__, __id__) {\
	__err__.line = CODE_LINE;\
	__err__.id = __id__;\
}

#define ERROR_CAPTURE_LINE(__err__) {\
	__err__.line = CODE_LINE;\
}

#define ERROR_NONE(__err__)			    if(__err__.id == ERR_NONE && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_NO_MEM(__err__)           if(__err__.id == ERR_NO_MEM && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_ARG(__err__)      if(__err__.id == ERR_INVALID_ARG && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_STATE(__err__)    if(__err__.id == ERR_INVALID_STATE && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_SIZE(__err__)     if(__err__.id == ERR_INVALID_SIZE && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_NOT_FOUND(__err__)        if(__err__.id == ERR_NOT_FOUND && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_NOT_SUPPORTED(__err__)    if(__err__.id == ERR_NOT_SUPPORTED && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_TIMEOUT(__err__)          if(__err__.id == ERR_TIMEOUT && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_RESPONSE(__err__) if(__err__.id == ERR_INVALID_RESPONSE && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_CRC(__err__)      if(__err__.id == ERR_INVALID_CRC && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_VERSION(__err__)  if(__err__.id == ERR_INVALID_VERSION && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_INVALID_MAC(__err__)      if(__err__.id == ERR_INVALID_MAC && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_NOT_FINISHED(__err__)     if(__err__.id == ERR_NOT_FINISHED && ((__err__.line = CODE_LINE) != 0U))
#define ERROR_NOT_ALLOWED(__err__)      if(__err__.id == ERR_NOT_ALLOWED && ((__err__.line = CODE_LINE) != 0U))


#ifdef __cplusplus
}
#endif

#endif /* SDK_COMMON_ERROR_CHECK_H_ */
