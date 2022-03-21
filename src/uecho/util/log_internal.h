
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

#define UECHO_LOG_ERROR_S "ERROR"
#define UECHO_LOG_WARN_S "WARN"
#define UECHO_LOG_INFO_S "INFO"
#define UECHO_LOG_DEBUG_S "DEBUG"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UECHO_UTIL_LOG_INTERNAL_H_ */
