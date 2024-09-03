/*
 * logger.h
 *
 *  Created on: Jul 28, 2024
 *      Author: anh
 */

#ifndef SDK_COMPONENTS_LOGGER_LOGGER_H_
#define SDK_COMPONENTS_LOGGER_LOGGER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"

#define LOG_MAX_TAG_LENGHT 20
#define LOG_NUM_PROP_SET   20
#define LOG_NEXTLINE false
#define LOG_PREVLINE (!LOG_NEXTLINE)

typedef enum {
	LOG_NONE    = 0x00,
	LOG_ERROR   = 0x01,
	LOG_ASSERT  = 0x02,
	LOG_WARNING = 0x04,
	LOG_INFO    = 0x08,
	LOG_DEBUG   = 0x10,
	LOG_VERBOSE = 0x20,
} log_level_t;

typedef enum {
	LOG_STYLE_BASIC,
	LOG_STYLE_BOLD,
	LOG_STYLE_ITALIC,
	LOG_STYLE_BGRCOLOR,
	LOG_STYLE_DEFAULT = LOG_STYLE_BASIC,
} log_style_t;

#define LOGE(tag, ...) LOG_LEVEL_ERROR(tag, LOG_NEXTLINE, __VA_ARGS__)
#define LOGA(tag, ...) LOG_LEVEL_ASSERT(tag, LOG_NEXTLINE, __VA_ARGS__)
#define LOGW(tag, ...) LOG_LEVEL_WARNING(tag, LOG_NEXTLINE, __VA_ARGS__)
#define LOGI(tag, ...) LOG_LEVEL_INFO(tag, LOG_NEXTLINE, __VA_ARGS__)
#define LOGD(tag, ...) LOG_LEVEL_DEBUG(tag, LOG_NEXTLINE, __VA_ARGS__)
#define LOGV(tag, ...) LOG_LEVEL_VERBOSE(tag, LOG_NEXTLINE, __VA_ARGS__)

#define LOGE_P(tag, ...) LOG_LEVEL_ERROR(tag, LOG_PREVLINE, __VA_ARGS__)
#define LOGA_P(tag, ...) LOG_LEVEL_ASSERT(tag, LOG_PREVLINE, __VA_ARGS__)
#define LOGW_P(tag, ...) LOG_LEVEL_WARNING(tag, LOG_PREVLINE, __VA_ARGS__)
#define LOGI_P(tag, ...) LOG_LEVEL_INFO(tag, LOG_PREVLINE, __VA_ARGS__)
#define LOGD_P(tag, ...) LOG_LEVEL_DEBUG(tag, LOG_PREVLINE, __VA_ARGS__)
#define LOGV_P(tag, ...) LOG_LEVEL_VERBOSE(tag, LOG_PREVLINE, __VA_ARGS__)


void log_port_flush_string(char *string, uint16_t length);
uint32_t log_port_get_systime(void);

void log_enable(void);
void log_disable(void);

void log_set_level_style(log_level_t level, log_style_t style);
void log_set_global_style(log_style_t style);
void log_set_filter_style(const char *tag, log_level_t level, log_style_t style);
void log_set_filtermax_style(const char *tag, log_level_t level, log_style_t style);

void LOG_LEVEL_ERROR  (const char *tag, bool prevline, const char *format, ...);
void LOG_LEVEL_ASSERT (const char *tag, bool prevline, const char *format, ...);
void LOG_LEVEL_WARNING(const char *tag, bool prevline, const char *format, ...);
void LOG_LEVEL_INFO   (const char *tag, bool prevline, const char *format, ...);
void LOG_LEVEL_DEBUG  (const char *tag, bool prevline, const char *format, ...);
void LOG_LEVEL_VERBOSE(const char *tag, bool prevline, const char *format, ...);

void LOG_FAULT(const char *format, ...);
void LOG_BASE (const char *format, ...);


#ifdef __cplusplus
}
#endif

#endif /* SDK_COMPONENTS_LOGGER_LOGGER_H_ */
