/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/string.h>

#include <string.h>

/* Define amount of extra characters allocated on each realloc, with this we
   can avoid many small subsequent reallocs, which takes lots of time */
#define CG_STRING_REALLOC_EXTRA		16

/****************************************
* uecho_string_new
****************************************/

mUpnpString *uecho_string_new()
{
	mUpnpString *str;

	uecho_log_debug_l5("Entering...\n");

	str = (mUpnpString *)malloc(sizeof(mUpnpString));

	if (NULL != str) {
		str->value = NULL;
		str->memSize = 0;
		str->valueSize = 0;
	}

	uecho_log_debug_l5("Leaving...\n");

	return str;
}

/****************************************
* uecho_string_delete
****************************************/

void uecho_string_delete(mUpnpString *str)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL != str) {
		uecho_string_clear(str);
		free(str);
	}
	
	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_delete
****************************************/

void uecho_string_clear(mUpnpString *str)
{
	uecho_log_debug_l5("Entering...\n");
	
	if (NULL != str) {
		if (str->value != NULL) {
			free(str->value);
			str->value = NULL;
			str->memSize = 0;
			str->valueSize = 0;
		}
	}

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_setvalue
****************************************/

void uecho_string_setvalue(mUpnpString *str, const char *value)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL != str) {
		if (value != NULL)
			uecho_string_setnvalue(str, value, uecho_strlen(value));
	}
	
	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_setintvalue
****************************************/

void uecho_string_setintvalue(mUpnpString *str, int value)
{
	char buf[CG_STRING_INTEGER_BUFLEN];

	uecho_log_debug_l5("Entering...\n");

	uecho_string_setvalue(str, uecho_int2str(value, buf, sizeof(buf)));

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_setlongvalue
****************************************/

void uecho_string_setlongvalue(mUpnpString *str, long value)
{
	char buf[CG_STRING_LONG_BUFLEN];

	uecho_log_debug_l5("Entering...\n");

	uecho_string_setvalue(str, uecho_long2str(value, buf, sizeof(buf)));

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_setnvalue
****************************************/

void uecho_string_setnvalue(mUpnpString *str, const char *value, size_t len)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL != str) {
		uecho_string_clear(str);
		if (value != NULL) {
			str->valueSize = len;
			str->memSize = str->valueSize + 1;
			str->value = (char *)malloc(str->memSize * sizeof(char));

			if ( NULL == str->value ) {
				uecho_log_debug_s("Memory allocation failure!\n");
				return;
			}

			/* memcpy works better with non-zero-terminated data than strncpy */
			memcpy(str->value, value, len);
			str->value[len] = '\0';
		}
	}

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_setpointervalue
****************************************/

void uecho_string_setpointervalue(mUpnpString *str, char *value, size_t len)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL != str) {
		uecho_string_clear(str);
		str->value = value;
		str->valueSize = len;
		str->memSize = str->valueSize + 1;
	}
	
	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_string_getvalue
****************************************/

char *uecho_string_getvalue(mUpnpString *str)
{
	uecho_log_debug_l5("Entering...\n");

	uecho_log_debug_l5("Leaving...\n");

	return (NULL != str) ? str->value : NULL;
}

/****************************************
* uecho_string_getmemorysize
****************************************/

size_t uecho_string_getmemorysize(mUpnpString *str)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == str)
		return 0;

	uecho_log_debug_l5("Leaving...\n");

	return str->memSize;
}

/****************************************
* uecho_string_length
****************************************/

size_t uecho_string_length(mUpnpString *str)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == str)
		return 0;

	if (str->value == NULL)
		return 0;

	uecho_log_debug_l5("Leaving...\n");

	return str->valueSize;
}

/****************************************
* uecho_string_add
****************************************/

char *uecho_string_addvalue(mUpnpString *str, const char *value)
{
	uecho_log_debug_l5("Entering...\n");

	uecho_log_debug_l5("Leaving...\n");

	return uecho_string_naddvalue(str, value, uecho_strlen(value));
}

/****************************************
* uecho_string_add
****************************************/

char *uecho_string_naddvalue(mUpnpString *str, const char *value, size_t valueLen)
{
	char *newValue = NULL;
	size_t newMemSize = 0;

	uecho_log_debug_l5("Entering...\n");

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
		newMemSize += CG_STRING_REALLOC_EXTRA;
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

	uecho_log_debug_l5("Leaving...\n");

	return uecho_string_getvalue(str);
}

/****************************************
* uecho_string_addrep
****************************************/

char *uecho_string_addrepvalue(mUpnpString *str, const char *value, size_t repeatCnt)
{
	int n;
	uecho_log_debug_l5("Entering...\n");

	for (n = 0; n < repeatCnt; n++)
		uecho_string_addvalue(str, value);

	uecho_log_debug_l5("Leaving...\n");

	return uecho_string_getvalue(str);
}

/****************************************
* uecho_string_naddrep
****************************************/

char *uecho_string_naddrepvalue(mUpnpString *str, const char *value, size_t valueLen, size_t repeatCnt)
{
	int n;

	uecho_log_debug_l5("Entering...\n");

	for (n = 0; n < repeatCnt; n++)
		uecho_string_naddvalue(str, value, valueLen);

	uecho_log_debug_l5("Leaving...\n");

	return uecho_string_getvalue(str);
}

/****************************************
* uecho_string_replace
****************************************/

char *uecho_string_replace(mUpnpString *str, char *fromStr[], char *toStr[], size_t fromStrCnt)
{
	char *orgValue = NULL;
	size_t orgValueLen = 0;
	int n = 0;
	int copyPos = 0;
	size_t *fromStrLen = NULL;
	mUpnpString *repValue = NULL;
	BOOL isReplaced = FALSE;

	uecho_log_debug_l5("Entering...\n");

	if (NULL == str )
		return NULL;
	
	repValue = uecho_string_new();
	
	fromStrLen = (size_t *)malloc(sizeof(size_t) * fromStrCnt);

	if ( NULL == fromStrLen )
	{
        uecho_string_delete(repValue);
		uecho_log_debug_s("Memory allocation failure!\n");
		return NULL;
	}
	
	for (n=0; n<fromStrCnt; n++)
		fromStrLen[n] = uecho_strlen(fromStr[n]);
	
	orgValue = uecho_string_getvalue(str);
	orgValueLen = uecho_string_length(str);
	
	copyPos = 0;
	while (copyPos<orgValueLen) {
		isReplaced = FALSE;
		for (n=0; n<fromStrCnt; n++) {
			if (strncmp(fromStr[n], orgValue + copyPos,  fromStrLen[n]) == 0) {
				uecho_string_addvalue(repValue, toStr[n]);
				copyPos += fromStrLen[n];
				isReplaced = TRUE;
				continue;
			}
		}
		if (isReplaced == TRUE)
			continue;
		uecho_string_naddvalue(repValue, orgValue + copyPos, 1);
		copyPos++;
	}
	
	free(fromStrLen);

	uecho_string_setvalue(str, uecho_string_getvalue(repValue));	

	uecho_string_delete(repValue);
		
	uecho_log_debug_l5("Leaving...\n");

	return uecho_string_getvalue(str);
}
