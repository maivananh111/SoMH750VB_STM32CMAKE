/*
 * logger.c
 *
 *  Created on: Jul 28, 2024
 *      Author: anh
 */


#include "logger.h"

#include <inttypes.h>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"




typedef enum{
	LOG_BASIC_BLACK=0,
	LOG_BASIC_RED,
	LOG_BASIC_PURPLE,
	LOG_BASIC_YELLOW,
	LOG_BASIC_GREEN,
	LOG_BASIC_BLUE,
	LOG_BASIC_CYAN,
	LOG_BASIC_WHITE,

	LOG_BOLD_BLACK=8,
	LOG_BOLD_RED,
	LOG_BOLD_PURPLE,
	LOG_BOLD_YELLOW,
	LOG_BOLD_GREEN,
	LOG_BOLD_BLUE,
	LOG_BOLD_CYAN,
	LOG_BOLD_WHITE,

	LOG_ITALIC_BLACK=16,
	LOG_ITALIC_RED,
	LOG_ITALIC_PURPLE,
	LOG_ITALIC_YELLOW,
	LOG_ITALIC_GREEN,
	LOG_ITALIC_BLUE,
	LOG_ITALIC_CYAN,
	LOG_ITALIC_WHITE,

	LOG_BCKGND_BLACK=24,
	LOG_BCKGND_RED,
	LOG_BCKGND_PURPLE,
	LOG_BCKGND_YELLOW,
	LOG_BCKGND_GREEN,
	LOG_BCKGND_BLUE,
	LOG_BCKGND_CYAN,
	LOG_BCKGND_WHITE,
} log_stylecolor_t;

typedef struct{
	char tag[LOG_MAX_TAG_LENGHT];
	log_level_t level;
	log_style_t style;
} log_tagprop_t;

static bool _enable = true;
static log_tagprop_t _tagprop[LOG_NUM_PROP_SET];
static uint8_t _num_tagprop = 0;
static bool _prev_is_prevline = false;

static log_stylecolor_t _default_loge = LOG_BASIC_RED;	  // error.
static log_stylecolor_t _default_loga = LOG_BASIC_PURPLE;  // assert.
static log_stylecolor_t _default_logw = LOG_BASIC_YELLOW;  // warning.
static log_stylecolor_t _default_logi = LOG_BASIC_GREEN;	  // info.
static log_stylecolor_t _default_logd = LOG_BASIC_BLUE;    // debug.
static log_stylecolor_t _default_logv = LOG_BASIC_CYAN;    // debug.

static const char *COLOR_END = "\033[0m";
static const char *COLOR_START[] = {
	"\033[0;30m",
	"\033[0;31m",
	"\033[0;35m",
	"\033[0;33m",
	"\033[0;32m",
	"\033[0;34m",
	"\033[0;36m",
	"\033[0;37m",
	// Bold
	"\033[1;30m",
	"\033[1;31m",
	"\033[1;35m",
	"\033[1;33m",
	"\033[1;32m",
	"\033[1;34m",
	"\033[1;36m",
	"\033[1;37m",
	// Italic
	"\033[4;30m",
	"\033[4;31m",
	"\033[4;35m",
	"\033[4;33m",
	"\033[4;32m",
	"\033[4;34m",
	"\033[4;36m",
	"\033[4;37m",
	// Background
	"\033[40m",
	"\033[41m",
	"\033[45m",
	"\033[43m",
	"\033[42m",
	"\033[44m",
	"\033[46m",
	"\033[47m",
};

__attribute__((weak)) void log_port_flush_string(char *string, uint16_t lenght){
	(void)string;
	(void)lenght;
}

__attribute__((weak)) uint32_t log_port_get_systime(void){
	return 0;
}

static log_tagprop_t *log_get_tagprop(const char *tag){
	for(uint8_t i=0; i<_num_tagprop; i++){
		if(strcmp(_tagprop[i].tag, tag) == 0)
			return &_tagprop[i];
	}

	return NULL;
}

static uint8_t log_tag_available(const char *tag){
	for(uint8_t i=0; i<_num_tagprop; i++){
		if(strcmp(_tagprop[i].tag, tag) == 0)
			return i;
	}

	return UINT8_MAX;
}



void log_enable(void){
	_enable = true;
}

void log_disable(void){
	_enable = false;
}

void log_set_global_style(log_style_t style){
	_default_loge = (log_stylecolor_t)(LOG_BASIC_RED + style*8);
	_default_loga = (log_stylecolor_t)(LOG_BASIC_PURPLE + style*8);
	_default_logw = (log_stylecolor_t)(LOG_BASIC_YELLOW + style*8);
	_default_logi = (log_stylecolor_t)(LOG_BASIC_GREEN + style*8);
	_default_logd = (log_stylecolor_t)(LOG_BASIC_BLUE + style*8);
	_default_logv = (log_stylecolor_t)(LOG_BASIC_CYAN + style*8);
}

void log_set_level_style(log_level_t level, log_style_t style){
	if(level&LOG_ERROR)
		_default_loge = (log_stylecolor_t)(LOG_BASIC_RED + style*8);
	if(level&LOG_ASSERT)
		_default_loga = (log_stylecolor_t)(LOG_BASIC_PURPLE + style*8);
	if(level&LOG_WARNING)
		_default_logw = (log_stylecolor_t)(LOG_BASIC_YELLOW + style*8);
	if(level&LOG_INFO)
		_default_logi = (log_stylecolor_t)(LOG_BASIC_GREEN + style*8);
	if(level&LOG_DEBUG)
		_default_logd = (log_stylecolor_t)(LOG_BASIC_BLUE + style*8);
	if(level&LOG_VERBOSE)
		_default_logv = (log_stylecolor_t)(LOG_BASIC_CYAN + style*8);
}


void log_set_filter_style(const char *tag, log_level_t level, log_style_t style){
	if(_num_tagprop >= LOG_NUM_PROP_SET) return;

	uint8_t idx = log_tag_available(tag);
	uint8_t tag_set_idx = (idx != UINT8_MAX && idx < LOG_NUM_PROP_SET)? idx : _num_tagprop;

	memcpy(_tagprop[tag_set_idx].tag, tag, strlen(tag));
	_tagprop[tag_set_idx].level = level;
	_tagprop[tag_set_idx].style = style;

	if(idx > LOG_NUM_PROP_SET) _num_tagprop++;
}

void log_set_filtermax_style(const char *tag, log_level_t level, log_style_t style){
	if(_num_tagprop >= LOG_NUM_PROP_SET) return;

	uint8_t idx = log_tag_available(tag);
	uint8_t tag_set_idx = (idx != UINT8_MAX && idx < LOG_NUM_PROP_SET)? idx : _num_tagprop;

	memcpy(_tagprop[tag_set_idx].tag, tag, strlen(tag));
	_tagprop[tag_set_idx].style = style;

	uint8_t tag_level = 0x01;
	_tagprop[tag_set_idx].level = 0;
	do _tagprop[tag_set_idx].level |= tag_level;
	while((tag_level <<= 1) <= level);

	if(idx > LOG_NUM_PROP_SET) _num_tagprop++;
}


static inline void _log(log_level_t level, log_stylecolor_t style, bool prevline, const char header, const char *tag, const char *format, va_list args){
	uint32_t time = log_port_get_systime();
	char *arg_buffer = NULL;
	char *out_buffer = NULL;
	log_stylecolor_t log_style = style;

	log_tagprop_t *tagprop = log_get_tagprop(tag);
	if(tagprop != NULL){
		if(!((uint8_t)level & tagprop->level) && _enable)
			return;
		log_style = (log_stylecolor_t)(32 - __builtin_clz(level) + tagprop->style*8);
	}

	if(vasprintf(&arg_buffer, format, args) == -1)
		return;
	if(asprintf(&out_buffer, "%s%s%c [%"PRIu32"] %s: %s%s%s", (!prevline)? "\r\n":"\033[2K\r", COLOR_START[log_style], header, time, tag, arg_buffer, COLOR_END,
			(_prev_is_prevline == true && prevline == false)? "\r\n":"") == -1)
		return;
	log_port_flush_string(out_buffer, strlen(out_buffer));
	_prev_is_prevline = prevline;

	if(arg_buffer) free(arg_buffer);
	if(out_buffer) free(out_buffer);
}

void LOG_LEVEL_ERROR(const char *tag, bool prevline, const char *format, ...){
	va_list args;
	va_start(args, format);
	_log(LOG_ERROR, _default_loge, prevline, 'E', tag, format, args);
	va_end(args);
}

void LOG_LEVEL_ASSERT(const char *tag, bool prevline, const char *format, ...){
	va_list args;
	va_start(args, format);
	_log(LOG_ASSERT, _default_loga, prevline, 'A', tag, format, args);
	va_end(args);
}

void LOG_LEVEL_WARNING(const char *tag, bool prevline, const char *format, ...){
	va_list args;
	va_start(args, format);
	_log(LOG_WARNING, _default_logw, prevline, 'W', tag, format, args);
	va_end(args);
}

void LOG_LEVEL_INFO(const char *tag, bool prevline, const char *format, ...){
	va_list args;
	va_start(args, format);
	_log(LOG_INFO, _default_logi, prevline, 'I', tag, format, args);
	va_end(args);
}

void LOG_LEVEL_DEBUG(const char *tag, bool prevline, const char *format, ...){
	va_list args;
	va_start(args, format);
	_log(LOG_DEBUG, _default_logd, prevline, 'D', tag, format, args);
	va_end(args);
}

void LOG_LEVEL_VERBOSE(const char *tag, bool prevline, const char *format, ...){
	va_list args;
	va_start(args, format);
	_log(LOG_VERBOSE, _default_logv, prevline, 'V', tag, format, args);
	va_end(args);
}

