
/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_LOG_INTERNAL_H_
#define _UECHO_UTIL_LOG_INTERNAL_H_

#include <uecho/util/log.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_LOG_BUF 2048

#define LOG_ERROR 1
#define LOG_WARNING 2
#define LOG_INFO 4
#define LOG_DEBUG 8

#define LOG_ALL (LOG_ERROR | LOG_WARNING | LOG_INFO | LOG_DEBUG)

void uecho_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UECHO_UTIL_LOG_INTERNAL_H_ */
