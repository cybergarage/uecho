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

#include <uecho/util/log.h>
#include <uecho/util/mutex.h>

#if defined(WIN32)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

static const char* log_error_s = LOG_ERROR_S;
static const char* log_warning_s = LOG_WARNING_S;
static const char* log_info_s = LOG_INFO_S;
static const char* log_debug_s = LOG_DEBUG_S;

static const char* uecho_log_type2string(int type)
{
  switch (type) {
  case LOG_ERROR:
    return log_error_s;
    break;

  case LOG_WARNING:
    return log_warning_s;
    break;

  case LOG_INFO:
    return log_info_s;
    break;

  case LOG_DEBUG:
    return log_debug_s;
    break;
  }

  return "";
}

void uecho_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...)
{
  va_list list;

  char msg[MAX_LOG_BUF], ts_prefix[MAX_LOG_BUF];
  time_t ts;
  struct tm* localts;
  size_t prefix_len = -1;

  ts = time(NULL);
  localts = localtime(&ts);

  strftime(ts_prefix, MAX_LOG_BUF, "%c", localts);

  prefix_len = snprintf(msg, MAX_LOG_BUF, "%s : %s ", ts_prefix, uecho_log_type2string(severity));

  va_start(list, format);
  vsnprintf(msg + prefix_len, MAX_LOG_BUF - prefix_len, format, list);
  va_end(list);

  printf("%s\n", msg);
}
