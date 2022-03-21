/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_LOG_H_
#define _UECHO_UTIL_LOG_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
UECHO_LOG_NONE = 0x00,
UECHO_LOG_ERROR = 0x01,
UECHO_LOG_WARN = 0x02,
UECHO_LOG_INFO = 0x04,
UECHO_LOG_DEBUG = 0x08,
UECHO_LOG_ALL = 0xFF,
} uEchoLogLevel;

void uecho_log_setlevel(uEchoLogLevel level);

#if defined(__USE_ISOC99)
#define uecho_log(severity,format, ...) uecho_log_output(severity, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_error(format, ...) uecho_log_output(UECHO_LOG_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_warn(format, ...) uecho_log_output(UECHO_LOG_WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_info(format, ...) uecho_log_output(UECHO_LOG_INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_debug(format, ...) uecho_log_output(UECHO_LOG_DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#else
#define uecho_log(severity,format...) uecho_log_output(severity, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_error(format...) uecho_log_output(UECHO_LOG_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_warn(format...) uecho_log_output(UECHO_LOG_WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_info(format...) uecho_log_output(UECHO_LOG_INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_debug(format...) uecho_log_output(UECHO_LOG_DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#endif

void uecho_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UECHO_UTIL_LOG_H_ */
