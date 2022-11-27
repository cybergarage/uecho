/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/strings.h>

#include <string.h>

/* Define amount of extra characters allocated on each realloc, with this we
   can avoid many small subsequent reallocs, which takes lots of time */
#define UECHO_STRING_REALLOC_EXTRA 16

/****************************************
 * uecho_string_new
 ****************************************/

uEchoString* uecho_string_new(void)
{
  uEchoString* str;

  str = (uEchoString*)malloc(sizeof(uEchoString));

  if (NULL != str) {
    str->value = NULL;
    str->mem_size = 0;
    str->value_size = 0;
  }

  return str;
}

/****************************************
 * uecho_string_delete
 ****************************************/

void uecho_string_delete(uEchoString* str)
{
  if (NULL != str) {
    uecho_string_clear(str);
    free(str);
  }
}

/****************************************
 * uecho_string_delete
 ****************************************/

void uecho_string_clear(uEchoString* str)
{
  if (NULL != str) {
    if (str->value != NULL) {
      free(str->value);
      str->value = NULL;
      str->mem_size = 0;
      str->value_size = 0;
    }
  }
}

/****************************************
 * uecho_string_setvalue
 ****************************************/

void uecho_string_setvalue(uEchoString* str, const char* value)
{
  if (NULL != str) {
    if (value != NULL)
      uecho_string_setnvalue(str, value, uecho_strlen(value));
  }
}

/****************************************
 * uecho_string_setintvalue
 ****************************************/

void uecho_string_setintvalue(uEchoString* str, int value)
{
  char buf[UECHO_STRING_INTEGER_BUFLEN];

  uecho_string_setvalue(str, uecho_int2str(value, buf, sizeof(buf)));
}

/****************************************
 * uecho_string_setlongvalue
 ****************************************/

void uecho_string_setlongvalue(uEchoString* str, long value)
{
  char buf[UECHO_STRING_LONG_BUFLEN];

  uecho_string_setvalue(str, uecho_long2str(value, buf, sizeof(buf)));
}

/****************************************
 * uecho_string_setnvalue
 ****************************************/

void uecho_string_setnvalue(uEchoString* str, const char* value, size_t len)
{
  if (NULL != str) {
    uecho_string_clear(str);
    if (value != NULL) {
      str->value_size = len;
      str->mem_size = str->value_size + 1;
      str->value = (char*)malloc(str->mem_size * sizeof(char));

      if (NULL == str->value) {
        return;
      }

      /* memcpy works better with non-zero-terminated data than strncpy */
      memcpy(str->value, value, len);
      str->value[len] = '\0';
    }
  }
}

/****************************************
 * uecho_string_setpointervalue
 ****************************************/

void uecho_string_setpointervalue(uEchoString* str, char* value, size_t len)
{
  if (NULL != str) {
    uecho_string_clear(str);
    str->value = value;
    str->value_size = len;
    str->mem_size = str->value_size + 1;
  }
}

/****************************************
 * uecho_string_getvalue
 ****************************************/

char* uecho_string_getvalue(uEchoString* str)
{
  return (NULL != str) ? str->value : NULL;
}

/****************************************
 * uecho_string_getmemorysize
 ****************************************/

size_t uecho_string_getmemorysize(uEchoString* str)
{
  if (NULL == str)
    return 0;

  return str->mem_size;
}

/****************************************
 * uecho_string_length
 ****************************************/

size_t uecho_string_length(uEchoString* str)
{
  if (NULL == str)
    return 0;

  if (str->value == NULL)
    return 0;

  return str->value_size;
}

/****************************************
 * uecho_string_add
 ****************************************/

char* uecho_string_addvalue(uEchoString* str, const char* value)
{
  return uecho_string_naddvalue(str, value, uecho_strlen(value));
}

/****************************************
 * uecho_string_add
 ****************************************/

char* uecho_string_naddvalue(uEchoString* str, const char* value, size_t value_len)
{
  char* new_value = NULL;
  size_t new_mem_size = 0;

  if (NULL == str)
    return NULL;

  if (value == NULL || value_len <= 0) {
    /* Empty string, nothing to add */
    return uecho_string_getvalue(str);
  }

  /* Check, if we need to allocate memory for the new data */
  new_mem_size = str->value_size + value_len + 1;
  if (new_mem_size > str->mem_size || str->value == NULL) {
    /* realloc also some extra in order to avoid multiple reallocs */
    new_mem_size += UECHO_STRING_REALLOC_EXTRA;
    new_value = realloc(str->value, new_mem_size * sizeof(char));

    if (new_value == NULL) {
      /* Memory allocation failed, bail out */
      return NULL;
    }

    str->mem_size = new_mem_size;
    str->value = new_value;
  }

  /* memcpy works better with non-zero-terminated data
     than strncpy */
  memcpy(str->value + str->value_size, value, value_len);

  str->value_size += value_len;

  /* In case this is a string, append a termination character */
  str->value[str->value_size] = '\0';

  return uecho_string_getvalue(str);
}

/****************************************
 * uecho_string_addrep
 ****************************************/

char* uecho_string_addrepvalue(uEchoString* str, const char* value, size_t repeat_cnt)
{
  int n;

  for (n = 0; n < repeat_cnt; n++)
    uecho_string_addvalue(str, value);

  return uecho_string_getvalue(str);
}

/****************************************
 * uecho_string_naddrep
 ****************************************/

char* uecho_string_naddrepvalue(uEchoString* str, const char* value, size_t value_len, size_t repeat_cnt)
{
  int n;

  for (n = 0; n < repeat_cnt; n++)
    uecho_string_naddvalue(str, value, value_len);

  return uecho_string_getvalue(str);
}

/****************************************
 * uecho_string_replace
 ****************************************/

char* uecho_string_replace(uEchoString* str, char* from_str[], char* to_str[], size_t from_str_cnt)
{
  char* org_value = NULL;
  size_t org_value_len = 0;
  int n = 0;
  int copy_pos = 0;
  size_t* from_str_len = NULL;
  uEchoString* rep_value = NULL;
  bool is_replaced = false;

  if (NULL == str)
    return NULL;

  rep_value = uecho_string_new();

  from_str_len = (size_t*)malloc(sizeof(size_t) * from_str_cnt);

  if (NULL == from_str_len) {
    uecho_string_delete(rep_value);
    return NULL;
  }

  for (n = 0; n < from_str_cnt; n++)
    from_str_len[n] = uecho_strlen(from_str[n]);

  org_value = uecho_string_getvalue(str);
  org_value_len = uecho_string_length(str);

  copy_pos = 0;
  while (copy_pos < org_value_len) {
    is_replaced = false;
    for (n = 0; n < from_str_cnt; n++) {
      if (strncmp(from_str[n], org_value + copy_pos, from_str_len[n]) == 0) {
        uecho_string_addvalue(rep_value, to_str[n]);
        copy_pos += from_str_len[n];
        is_replaced = true;
        continue;
      }
    }
    if (is_replaced == true)
      continue;
    uecho_string_naddvalue(rep_value, org_value + copy_pos, 1);
    copy_pos++;
  }

  free(from_str_len);

  uecho_string_setvalue(str, uecho_string_getvalue(rep_value));

  uecho_string_delete(rep_value);

  return uecho_string_getvalue(str);
}
