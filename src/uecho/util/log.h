/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_LOG_H_
#define _UECHO_UTIL_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LOG_BUF 2048

#define LOG_ERROR 1
#define LOG_ERROR_S "ERROR"

#define LOG_WARNING 2
#define LOG_WARNING_S "WARN"

#define LOG_INFO 4
#define LOG_INFO_S "INFO"

#define LOG_DEBUG 8
#define LOG_DEBUG_S "DEBUG"

#define LOG_ALL (LOG_ERROR | LOG_WARNING | LOG_INFO | LOG_DEBUG)

void uecho_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...);

#ifdef CLOG_DEBUG

#if defined(__USE_ISOC99)
#define uecho_log_error(format, ...) uecho_log_output(LOG_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_warn(format, ...) uecho_log_output(LOG_WARNING, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_info(format, ...) uecho_log_output(LOG_INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#define uecho_log_debug(format, ...) uecho_log_output(LOG_DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, __VA_ARGS__)
#else
#define uecho_log_error(format...) uecho_log_output(LOG_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_warning(format...) uecho_log_output(LOG_WARNING, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_info(format...) uecho_log_output(LOG_INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#define uecho_log_debug(format...) uecho_log_output(LOG_DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, format)
#endif

#else

#if defined(__USE_ISOC99)
#define uecho_log_error(format, ...)
#define uecho_log_warn(format, ...)
#define uecho_log_info(format, ...)
#define uecho_log_debug(format, ...)
#else
#define uecho_log_error(format...)
#define uecho_log_warn(format...)
#define uecho_log_info(format...)
#define uecho_log_debug(format...)
#endif

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UECHO_UTIL_LOG_H_ */
