/******************************************************************
 *
 * mUPnP for C
 *
 * Copyright (C) The uecho Authors 2005
 * Copyright (C) 2006 Nokia Corporation. All rights reserved.
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_STRING_H_
#define _UECHO_UTIL_STRING_H_

#include <uecho/typedef.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_STRING_MEMORY_ALLOCATION_UNIT 64
 
/* UINT_MAX : 4294967295U */
#define CG_STRING_INTEGER_BUFLEN 16 
 
 /* ULONG_MAX : 4294967295UL */
#define CG_STRING_LONG_BUFLEN 32

 /*  ULLONG_MAX : 18446744073709551615ULL */
#define CG_STRING_LONGLONG_BUFLEN 32

#define CG_STRING_FLOAT_BUFLEN 64
#define CG_STRING_DOUBLE_BUFLEN 64
	
/****************************************
* Function 
****************************************/
	
char *uecho_strdup(const char *str);
size_t uecho_strlen(const char *str);
char *uecho_strcpy(char *dest, const char *src);
char *uecho_strcat(char *dest, const char *src);
int uecho_strcmp(const char *str1, const char *str2);
int uecho_strncmp(const char *str1, const char *str2, int nchars);
int uecho_strcasecmp(const char *str1, const char *str2);
BOOL uecho_streq(const char *str1, const char *str2);
BOOL uecho_strcaseeq(const char *str1, const char *str2);
ssize_t uecho_strchr(const char *str, const char *chars, size_t nchars);
ssize_t uecho_strrchr(const char *str, const char *chars, size_t nchars);
ssize_t uecho_strstr(const char *haystack, const char *needle);
char *uecho_strtrimwhite(char *str);
char *uecho_strtrim(char *str, char *delim, size_t ndelim);
char *uecho_strltrim(char *str, char *delim, size_t ndelim);
char *uecho_strrtrim(char *str, char *delim, size_t ndelim);
char *uecho_strncpy(char *str1, const char *str2, size_t cnt);
char *uecho_strncat(char *str1, const char *str2, size_t cnt);
	
const char *uecho_int2str(int value, char *buf, size_t bufSize);
const char *uecho_long2str(long value, char *buf, size_t bufSize);
const char *uecho_float2str(float value, char *buf, size_t bufSize);
const char *uecho_double2str(double value, char *buf, size_t bufSize);
const char *uecho_sizet2str(size_t value, char *buf, size_t bufSize);
const char *uecho_ssizet2str(ssize_t value, char *buf, size_t bufSize);

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
	
typedef struct _uEchoString {
	char *value;
	size_t memSize;
	size_t valueSize;
} mUpnpString;
	
typedef struct _uEchoStringTokenizer {
	char *value;
	char *delim;
	size_t delimCnt;
	size_t nextStartPos;
	size_t lastPos;
	char *currToken;
	char *nextToken;
	char repToken;
	BOOL hasNextTokens;
} mUpnpStringTokenizer;
	
/****************************************
* Function (StringTokenizer)
****************************************/
	
mUpnpStringTokenizer *uecho_string_tokenizer_new(const char *str, const char *delim);
void uecho_string_tokenizer_delete(mUpnpStringTokenizer *strToken);
BOOL uecho_string_tokenizer_hasmoretoken(mUpnpStringTokenizer *strToken);
char *uecho_string_tokenizer_nexttoken(mUpnpStringTokenizer *strToken);
char *uecho_string_tokenizer_nextalltoken(mUpnpStringTokenizer *strToken);
	
void uecho_string_tokenizer_print(mUpnpStringTokenizer *strToken);
	
/****************************************
* Function (String)
****************************************/

mUpnpString *uecho_string_new();
void uecho_string_delete(mUpnpString *str);
void uecho_string_clear(mUpnpString *str);

void uecho_string_setvalue(mUpnpString *str, const char *value);
void uecho_string_setintvalue(mUpnpString *str, int value);
void uecho_string_setlongvalue(mUpnpString *str, long value);
void uecho_string_setfloatvalue(mUpnpString *str, float value);
void uecho_string_setdoublevalue(mUpnpString *str, double value);
void uecho_string_setnvalue(mUpnpString *str, const char *value, size_t len);
void uecho_string_setpointervalue(mUpnpString *str, char *value, size_t len);

char *uecho_string_getvalue(mUpnpString *str);
#define uecho_string_getintvalue(str) uecho_str2int(uecho_string_getvalue(str))
#define uecho_string_getlongvalue(str) uecho_str2long(uecho_string_getvalue(str))
#define uecho_string_getfloatvalue(str) uecho_str2float(uecho_string_getvalue(str))
#define uecho_string_getdoublevalue(str) uecho_str2double(uecho_string_getvalue(str))
	
size_t uecho_string_length(mUpnpString *str);

char *uecho_string_addvalue(mUpnpString *str, const char *value);
char *uecho_string_naddvalue(mUpnpString *str, const char *value, size_t valueLen);
char *uecho_string_addrepvalue(mUpnpString *str, const char *value, size_t repeatCnt);
char *uecho_string_naddrepvalue(mUpnpString *str, const char *value, size_t valueLen, size_t repeatCnt);

char *uecho_string_replace(mUpnpString *str, char *fromStr[], char *toStr[], size_t fromStrCnt);

size_t uecho_string_getmemorysize(mUpnpString *str);

#ifdef  __cplusplus
}
#endif

#endif
