/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_STRING_H_
#define _UECHO_UTIL_STRING_H_

#include <stdlib.h>
#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define UECHO_STRING_MEMORY_ALLOCATION_UNIT 64

/* UINT_MAX : 4294967295U */
#define UECHO_STRING_INTEGER_BUFLEN 16

/* ULONG_MAX : 4294967295UL */
#define UECHO_STRING_LONG_BUFLEN 32

/*  ULLONG_MAX : 18446744073709551615ULL */
#define UECHO_STRING_LONGLONG_BUFLEN 32

#define UECHO_STRING_FLOAT_BUFLEN 64
#define UECHO_STRING_DOUBLE_BUFLEN 64

/****************************************
* Function 
****************************************/

char* uecho_strdup(const char* str);
size_t uecho_strlen(const char* str);
char* uecho_strcpy(char* dst, const char* src);
char* uecho_strcat(char* dst, const char* src);
int uecho_strcmp(const char* str1, const char* str2);
int uecho_strncmp(const char* str1, const char* str2, size_t nchars);
int uecho_strcasecmp(const char* str1, const char* str2);
bool uecho_streq(const char* str1, const char* str2);
bool uecho_strcaseeq(const char* str1, const char* str2);
ssize_t uecho_strchr(const char* str, const char* chars, size_t nchars);
ssize_t uecho_strrchr(const char* str, const char* chars, size_t nchars);
ssize_t uecho_strstr(const char* haystack, const char* needle);
char* uecho_strtrimwhite(char* str);
char* uecho_strtrim(char* str, char* delim, size_t ndelim);
char* uecho_strltrim(char* str, char* delim, size_t ndelim);
char* uecho_strrtrim(char* str, char* delim, size_t ndelim);
char* uecho_strncpy(char* str1, const char* str2, size_t cnt);
char* uecho_strncat(char* str1, const char* str2, size_t cnt);
bool uecho_strloc(const char* str, char** buf);

const char* uecho_int2str(int value, char* buf, size_t buf_size);
const char* uecho_long2str(long value, char* buf, size_t buf_size);
const char* uecho_float2str(float value, char* buf, size_t buf_size);
const char* uecho_double2str(double value, char* buf, size_t buf_size);
const char* uecho_sizet2str(size_t value, char* buf, size_t buf_size);
const char* uecho_ssizet2str(ssize_t value, char* buf, size_t buf_size);

#define uecho_str2int(value) (value ? atoi(value) : 0)
#define uecho_str2long(value) (value ? atol(value) : 0)
#define uecho_strhex2long(value) (value ? strtol(value, NULL, 16) : 0)
#define uecho_strhex2ulong(value) (value ? strtoul(value, NULL, 16) : 0)
#define uecho_str2float(value) ((float)(value ? atof(value) : 0.0))
#define uecho_str2double(value) (value ? atof(value) : 0.0)
#define uecho_str2sizet(value) ((size_t)(value ? atol(value) : 0))
#define uecho_str2ssizet(value) ((ssize_t)(value ? atol(value) : 0))

/****************************************
* Data Type
****************************************/

typedef struct UEchoString {
  char* value;
  size_t mem_size;
  size_t value_size;
} uEchoString;

typedef struct UEchoStringTokenizer {
  char* value;
  char* delim;
  size_t delim_cnt;
  size_t next_start_pos;
  size_t last_pos;
  char* curr_token;
  char* next_token;
  char rep_token;
  bool has_next_tokens;
} uEchoStringTokenizer;

/****************************************
* Function (StringTokenizer)
****************************************/

uEchoStringTokenizer* uecho_string_tokenizer_new(const char* str, const char* delim);
void uecho_string_tokenizer_delete(uEchoStringTokenizer* str_token);
bool uecho_string_tokenizer_hasmoretoken(uEchoStringTokenizer* str_token);
char* uecho_string_tokenizer_nexttoken(uEchoStringTokenizer* str_token);
char* uecho_string_tokenizer_nextalltoken(uEchoStringTokenizer* str_token);

/****************************************
* Function (String)
****************************************/

uEchoString* uecho_string_new(void);
void uecho_string_delete(uEchoString* str);
void uecho_string_clear(uEchoString* str);

void uecho_string_setvalue(uEchoString* str, const char* value);
void uecho_string_setintvalue(uEchoString* str, int value);
void uecho_string_setlongvalue(uEchoString* str, long value);
void uecho_string_setfloatvalue(uEchoString* str, float value);
void uecho_string_setdoublevalue(uEchoString* str, double value);
void uecho_string_setnvalue(uEchoString* str, const char* value, size_t len);
void uecho_string_setpointervalue(uEchoString* str, char* value, size_t len);

char* uecho_string_getvalue(uEchoString* str);
#define uecho_string_getintvalue(str) uecho_str2int(uecho_string_getvalue(str))
#define uecho_string_getlongvalue(str) uecho_str2long(uecho_string_getvalue(str))
#define uecho_string_getfloatvalue(str) uecho_str2float(uecho_string_getvalue(str))
#define uecho_string_getdoublevalue(str) uecho_str2double(uecho_string_getvalue(str))

size_t uecho_string_length(uEchoString* str);

char* uecho_string_addvalue(uEchoString* str, const char* value);
char* uecho_string_naddvalue(uEchoString* str, const char* value, size_t value_len);
char* uecho_string_addrepvalue(uEchoString* str, const char* value, size_t repeat_cnt);
char* uecho_string_naddrepvalue(uEchoString* str, const char* value, size_t value_len, size_t repeat_cnt);

char* uecho_string_replace(uEchoString* str, char* from_str[], char* to_str[], size_t from_str_cnt);

size_t uecho_string_getmemorysize(uEchoString* str);

#ifdef __cplusplus
}
#endif

#endif
