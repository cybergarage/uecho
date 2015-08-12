/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/string.h>

#include <string.h>

/* Define amount of extra characters allocated on each realloc, with this we
   can avoid many small subsequent reallocs, which takes lots of time */
#define UECHO_STRING_REALLOC_EXTRA		16

/****************************************
* uecho_string_new
****************************************/

uEchoString *uecho_string_new(void)
{
	uEchoString *str;

	str = (uEchoString *)malloc(sizeof(uEchoString));

	if (NULL != str) {
		str->value = NULL;
		str->memSize = 0;
		str->valueSize = 0;
	}

	return str;
}

/****************************************
* uecho_string_delete
****************************************/

void uecho_string_delete(uEchoString *str)
{
	if (NULL != str) {
		uecho_string_clear(str);
		free(str);
	}
}

/****************************************
* uecho_string_delete
****************************************/

void uecho_string_clear(uEchoString *str)
{
	if (NULL != str) {
		if (str->value != NULL) {
			free(str->value);
			str->value = NULL;
			str->memSize = 0;
			str->valueSize = 0;
		}
	}
}

/****************************************
* uecho_string_setvalue
****************************************/

void uecho_string_setvalue(uEchoString *str, const char *value)
{
	if (NULL != str) {
		if (value != NULL)
			uecho_string_setnvalue(str, value, uecho_strlen(value));
	}
}

/****************************************
* uecho_string_setintvalue
****************************************/

void uecho_string_setintvalue(uEchoString *str, int value)
{
	char buf[UECHO_STRING_INTEGER_BUFLEN];

	uecho_string_setvalue(str, uecho_int2str(value, buf, sizeof(buf)));
}

/****************************************
* uecho_string_setlongvalue
****************************************/

void uecho_string_setlongvalue(uEchoString *str, long value)
{
	char buf[UECHO_STRING_LONG_BUFLEN];

	uecho_string_setvalue(str, uecho_long2str(value, buf, sizeof(buf)));
}

/****************************************
* uecho_string_setnvalue
****************************************/

void uecho_string_setnvalue(uEchoString *str, const char *value, size_t len)
{
	if (NULL != str) {
		uecho_string_clear(str);
		if (value != NULL) {
			str->valueSize = len;
			str->memSize = str->valueSize + 1;
			str->value = (char *)malloc(str->memSize * sizeof(char));

			if ( NULL == str->value ) {
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

void uecho_string_setpointervalue(uEchoString *str, char *value, size_t len)
{
	if (NULL != str) {
		uecho_string_clear(str);
		str->value = value;
		str->valueSize = len;
		str->memSize = str->valueSize + 1;
	}
}

/****************************************
* uecho_string_getvalue
****************************************/

char *uecho_string_getvalue(uEchoString *str)
{
	return (NULL != str) ? str->value : NULL;
}

/****************************************
* uecho_string_getmemorysize
****************************************/

size_t uecho_string_getmemorysize(uEchoString *str)
{
	if (NULL == str)
		return 0;

	return str->memSize;
}

/****************************************
* uecho_string_length
****************************************/

size_t uecho_string_length(uEchoString *str)
{
	if (NULL == str)
		return 0;

	if (str->value == NULL)
		return 0;

	return str->valueSize;
}

/****************************************
* uecho_string_add
****************************************/

char *uecho_string_addvalue(uEchoString *str, const char *value)
{
	return uecho_string_naddvalue(str, value, uecho_strlen(value));
}

/****************************************
* uecho_string_add
****************************************/

char *uecho_string_naddvalue(uEchoString *str, const char *value, size_t valueLen)
{
	char *newValue = NULL;
	size_t newMemSize = 0;

	if (NULL == str)
		return NULL;

	if (value == NULL || valueLen <= 0)
	{
		/* Empty string, nothing to add */
		return uecho_string_getvalue(str);
	}

	/* Check, if we need to allocate memory for the new data */
	newMemSize = str->valueSize + valueLen + 1;
	if (newMemSize > str->memSize || str->value == NULL)
	{
		/* realloc also some extra in order to avoid multiple reallocs */
		newMemSize += UECHO_STRING_REALLOC_EXTRA;
		newValue = realloc(str->value, newMemSize * sizeof(char));

		if (newValue == NULL)
		{
			/* Memory allocation failed, bail out */
			return NULL;
		}

		str->memSize = newMemSize;
		str->value = newValue;
	}

	/* memcpy works better with non-zero-terminated data
	   than strncpy */
	memcpy(str->value + str->valueSize, value, valueLen);

	str->valueSize += valueLen;
	
	/* In case this is a string, append a termination character */
	str->value[str->valueSize] = '\0';

	return uecho_string_getvalue(str);
}

/****************************************
* uecho_string_addrep
****************************************/

char *uecho_string_addrepvalue(uEchoString *str, const char *value, size_t repeatCnt)
{
	int n;

	for (n = 0; n < repeatCnt; n++)
		uecho_string_addvalue(str, value);

	return uecho_string_getvalue(str);
}

/****************************************
* uecho_string_naddrep
****************************************/

char *uecho_string_naddrepvalue(uEchoString *str, const char *value, size_t valueLen, size_t repeatCnt)
{
	int n;

	for (n = 0; n < repeatCnt; n++)
		uecho_string_naddvalue(str, value, valueLen);

	return uecho_string_getvalue(str);
}

/****************************************
* uecho_string_replace
****************************************/

char *uecho_string_replace(uEchoString *str, char *fromStr[], char *toStr[], size_t fromStrCnt)
{
	char *orgValue = NULL;
	size_t orgValueLen = 0;
	int n = 0;
	int copyPos = 0;
	size_t *fromStrLen = NULL;
	uEchoString *repValue = NULL;
	bool isReplaced = false;

	if (NULL == str )
		return NULL;
	
	repValue = uecho_string_new();
	
	fromStrLen = (size_t *)malloc(sizeof(size_t) * fromStrCnt);

	if ( NULL == fromStrLen )
	{
        uecho_string_delete(repValue);
		return NULL;
	}
	
	for (n=0; n<fromStrCnt; n++)
		fromStrLen[n] = uecho_strlen(fromStr[n]);
	
	orgValue = uecho_string_getvalue(str);
	orgValueLen = uecho_string_length(str);
	
	copyPos = 0;
	while (copyPos<orgValueLen) {
		isReplaced = false;
		for (n=0; n<fromStrCnt; n++) {
			if (strncmp(fromStr[n], orgValue + copyPos,  fromStrLen[n]) == 0) {
				uecho_string_addvalue(repValue, toStr[n]);
				copyPos += fromStrLen[n];
				isReplaced = true;
				continue;
			}
		}
		if (isReplaced == true)
			continue;
		uecho_string_naddvalue(repValue, orgValue + copyPos, 1);
		copyPos++;
	}
	
	free(fromStrLen);

	uecho_string_setvalue(str, uecho_string_getvalue(repValue));	

	uecho_string_delete(repValue);
		
	return uecho_string_getvalue(str);
}
