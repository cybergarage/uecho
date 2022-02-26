/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <string.h>

#include <uecho/util/strings.h>

/****************************************
* uecho_strdup
****************************************/

char* uecho_strdup(const char* str)
{
#if !defined(HAVE_STRDUP)
  char* cp_str_buf;
#endif

  if (str == NULL)
    return NULL;

#if defined(HAVE_STRDUP)
  return strdup(str);
#else
  cp_str_buf = (char*)malloc(strlen(str) + 1);
  if (NULL != cp_str_buf)
    strcpy(cp_str_buf, str);
  return cp_str_buf;
#endif
}

/****************************************
 * uecho_strloc
 ****************************************/

bool uecho_strloc(const char* str, char** buf)
{
  if (!str || !buf)
    return false;

  if (*buf) {
    free(*buf);
    *buf = NULL;
  }

  if (!str)
    return true;

  *buf = uecho_strdup(str);

  return true;
}

/****************************************
* uecho_strlen
****************************************/

size_t uecho_strlen(const char* str)
{
  return (str == NULL) ? 0 : strlen(str);
}

/****************************************
* uecho_strcpy
****************************************/

char* uecho_strcpy(char* dst, const char* src)
{
  return strcpy(dst, src);
}

/****************************************
* uecho_strcat
****************************************/

char* uecho_strcat(char* dst, const char* src)
{
  return strcat(dst, src);
}

/****************************************
* uecho_strcmp
****************************************/

int uecho_strcmp(const char* str1, const char* str2)
{
  if (str1 == NULL)
    return -1;
  if (str2 == NULL)
    return 1;
  return strcmp(str1, str2);
}

/****************************************
* uecho_strncmp
****************************************/

int uecho_strncmp(const char* str1, const char* str2, int nchars)
{
  if (str1 == NULL)
    return -1;
  if (str2 == NULL)
    return 1;

  return strncmp(str1, str2, nchars);
}

/****************************************
* uecho_strcasecmp
****************************************/

int uecho_strcasecmp(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL)
    return -1;
#if !defined(WIN32)
  return strcasecmp(str1, str2);
#else
  for (; toupper(*str1) == toupper(*str2); str1++, str2++) {
    if (*str1 == '\0')
      return 0;
  }
  return *str1 - *str2;
#endif
}

/****************************************
* uecho_streq
****************************************/

bool uecho_streq(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL)
    return false;

  return ((uecho_strcmp(str1, str2) == 0) ? true : false);
}

/****************************************
* uecho_strcaseeq
****************************************/

bool uecho_strcaseeq(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL)
    return false;

  return ((uecho_strcasecmp(str1, str2) == 0) ? true : false);
}

/****************************************
* uecho_strstr
****************************************/

ssize_t uecho_strstr(const char* haystack, const char* needle)
{
  char* str_pos;

  if (haystack == NULL || needle == NULL)
    return -1;
  str_pos = strstr(haystack, needle);
  if (str_pos == NULL)
    return -1;
  return (str_pos - haystack);
}

/****************************************
* uecho_strchr
****************************************/

ssize_t uecho_strchr(const char* str, const char* chars, size_t nchars)
{
  size_t str_len;
  ssize_t i, j;

  if (str == NULL || chars == NULL)
    return -1;

  str_len = uecho_strlen(str);
  for (i = 0; i < str_len; i++) {
    for (j = 0; j < nchars; j++) {
      if (str[i] == chars[j])
        return i;
    }
  }

  return -1;
}

/****************************************
* uecho_strrchr
****************************************/

ssize_t uecho_strrchr(const char* str, const char* chars, size_t nchars)
{
  size_t str_len;
  ssize_t i, j;

  if (str == NULL || chars == NULL)
    return -1;

  str_len = uecho_strlen(str);
  for (i = (str_len - 1); 0 <= i; i--) {
    for (j = 0; j < nchars; j++) {
      if (str[i] == chars[j])
        return i;
    }
  }

  return -1;
}

/****************************************
* uecho_strtrimwhite
****************************************/

char* uecho_strtrimwhite(char* str)
{
  size_t str_len;
  ssize_t i;
  str_len = uecho_strlen(str);
  if (str_len == 0)
    return str;
  for (i = (str_len - 1); 0 <= i; i--) {
    if (isspace(str[i])) {
      str_len--;
    }
  }
  for (i = 0; i < str_len; i++) {
    if (!isspace(str[i]))
      break;
  }

  if (i > 0)
    memmove(str, str + i, str_len - i);

  str[str_len] = 0;
  return str;
}

/****************************************
* uecho_strtrim
****************************************/

char* uecho_strtrim(char* str, char* delim, size_t ndelim)
{
  if (str == NULL || delim == NULL)
    return NULL;

  uecho_strrtrim(str, delim, ndelim);
  return uecho_strltrim(str, delim, ndelim);
}

/****************************************
* uecho_strltrim
****************************************/

char* uecho_strltrim(char* str, char* delim, size_t ndelim)
{
  size_t str_len;
  ssize_t i, j;

  str_len = uecho_strlen(str);
  for (i = 0; i < str_len; i++) {
    bool has_delim = false;
    for (j = 0; j < ndelim; j++) {
      if (str[i] == delim[j]) {
        has_delim = true;
        break;
      }
    }
    if (has_delim == false)
      return (str + i);
  }

  return (str + str_len);
}

/****************************************
* uecho_strrtrim
****************************************/

char* uecho_strrtrim(char* str, char* delim, size_t ndelim)
{
  size_t str_len;
  ssize_t i, j;

  str_len = uecho_strlen(str);
  for (i = (str_len - 1); 0 <= i; i--) {
    bool has_delim = false;
    for (j = 0; j < ndelim; j++) {
      if (str[i] == delim[j]) {
        has_delim = true;
        str[i] = '\0';
        break;
      }
    }
    if (has_delim == false)
      break;
  }

  return str;
}

/****************************************
* uecho_strncpy
****************************************/

char* uecho_strncpy(char* str1, const char* str2, size_t cnt)
{
  strncpy(str1, str2, cnt);
  return str1;
}

/****************************************
* uecho_strncat
****************************************/

char* uecho_strncat(char* str1, const char* str2, size_t cnt)
{
  size_t str1_len;
  str1_len = uecho_strlen(str1);
  return uecho_strncpy((str1 + str1_len), str2, cnt);
}

/****************************************
* uecho_int2str
****************************************/

const char* uecho_int2str(int value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%d", value);
#else
  sprintf(buf, "%d", value);
#endif
  return buf;
}

/****************************************
* uecho_long2str
****************************************/

const char* uecho_long2str(long value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%ld", value);
#else
  sprintf(buf, "%ld", value);
#endif
  return buf;
}

/****************************************
* uecho_float2str
****************************************/

const char* uecho_float2str(float value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%f", value);
#else
  sprintf(buf, "%f", value);
#endif
  return buf;
}

/****************************************
* uecho_double2str
****************************************/

const char* uecho_double2str(double value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%lf", value);
#else
  sprintf(buf, "%lf", value);
#endif
  return buf;
}

/****************************************
* uecho_sizet2str
****************************************/

const char* uecho_sizet2str(size_t value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%zd", value);
#else
  sprintf(buf, "%zd", value);
#endif
  return buf;
}

/****************************************
* uecho_ssizet2str
****************************************/

const char* uecho_ssizet2str(ssize_t value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%zd", value);
#else
  sprintf(buf, "%zd", value);
#endif
  return buf;
}
