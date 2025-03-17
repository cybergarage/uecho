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

static uEchoLogLevel logLevel = UECHO_LOG_NONE;
static const char* logErrorS = UECHO_LOG_ERROR_S;
static const char* logWarningS = UECHO_LOG_WARN_S;
static const char* logInfoS = UECHO_LOG_INFO_S;
static const char* logDebugS = UECHO_LOG_DEBUG_S;

static const char* uecho_log_type2string(int type)
{
  switch (type) {
  case UECHO_LOG_ERROR:
    return logErrorS;
    break;

  case UECHO_LOG_WARN:
    return logWarningS;
    break;

  case UECHO_LOG_INFO:
    return logInfoS;
    break;

  case UECHO_LOG_DEBUG:
    return logDebugS;
    break;
  }

  return "";
}

void uecho_log_setlevel(uEchoLogLevel level)
{
  logLevel = level;
}

void uecho_log_output(int severity, const char* file, int lineN, const char* function, const char* format, ...)
{
  va_list list;

  char msg[MAX_LOG_BUF], tsPrefix[MAX_LOG_BUF];
  time_t ts;
  struct tm* localts;
  size_t prefixLen = -1;

  if (logLevel < severity)
    return;

  ts = time(NULL);
  localts = localtime(&ts);

  strftime(tsPrefix, MAX_LOG_BUF, "%F %T", localts);

  prefixLen = snprintf(msg, MAX_LOG_BUF, "%s : %s ", tsPrefix, uecho_log_type2string(severity));

  va_start(list, format);
  vsnprintf(msg + prefixLen, MAX_LOG_BUF - prefixLen, format, list);
  va_end(list);

  printf("%s\n", msg);
}
