/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <uecho/util/_log.h>
#include <uecho/util/mutex.h>

#if defined(WIN32)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

static uEchoLogLevel log_level = UECHO_LOG_NONE;
static const char* log_error_s = UECHO_LOG_ERROR_S;
static const char* log_warning_s = UECHO_LOG_WARN_S;
static const char* log_info_s = UECHO_LOG_INFO_S;
static const char* log_debug_s = UECHO_LOG_DEBUG_S;

static const char* uecho_log_type2string(int type)
{
  switch (type) {
  case UECHO_LOG_ERROR:
    return log_error_s;
    break;

  case UECHO_LOG_WARN:
    return log_warning_s;
    break;

  case UECHO_LOG_INFO:
    return log_info_s;
    break;

  case UECHO_LOG_DEBUG:
    return log_debug_s;
    break;
  }

  return "";
}

void uecho_log_setlevel(uEchoLogLevel level)
{
  log_level = level;
}

void uecho_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...)
{
  va_list list;

  char msg[MAX_LOG_BUF], ts_prefix[MAX_LOG_BUF];
  time_t ts;
  struct tm* localts;
  size_t prefix_len = -1;

  if (log_level < severity)
    return;
  
  ts = time(NULL);
  localts = localtime(&ts);

  strftime(ts_prefix, MAX_LOG_BUF, "%c", localts);

  prefix_len = snprintf(msg, MAX_LOG_BUF, "%s : %s ", ts_prefix, uecho_log_type2string(severity));

  va_start(list, format);
  vsnprintf(msg + prefix_len, MAX_LOG_BUF - prefix_len, format, list);
  va_end(list);

  printf("%s\n", msg);
}
