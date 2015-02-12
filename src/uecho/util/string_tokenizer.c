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

/****************************************
* uecho_string_tokenizer_new
****************************************/

uEchoStringTokenizer *uecho_string_tokenizer_new(const char *value, const char *delim)
{
	uEchoStringTokenizer *strToken;

	uecho_log_debug_l4("Entering...\n");

	strToken = (uEchoStringTokenizer *)malloc(sizeof(uEchoStringTokenizer));

	if ( NULL != strToken )
	{
		strToken->value = uecho_strdup(value);
		strToken->delim = uecho_strdup(delim);
		strToken->delimCnt = uecho_strlen(strToken->delim);
		strToken->nextStartPos = 0;
		strToken->lastPos = uecho_strlen(value) - 1;
		strToken->currToken = NULL;
		strToken->nextToken = NULL;
		uecho_string_tokenizer_nexttoken(strToken);
	}
  
	uecho_log_debug_l4("Leaving...\n");
  
	return strToken;
}

/****************************************
* uecho_string_tokenizer_delete
****************************************/

void uecho_string_tokenizer_delete(uEchoStringTokenizer *strToken)
{
	uecho_log_debug_l4("Entering...\n");

	free(strToken->value);
	free(strToken->delim);
	free(strToken);

	uecho_log_debug_l4("Leaving...\n");
}

/****************************************
* uecho_string_tokenizer_hasmoretoken
****************************************/

BOOL uecho_string_tokenizer_hasmoretoken(uEchoStringTokenizer *strToken)
{
	uecho_log_debug_l4("Entering...\n");

	return strToken->hasNextTokens;

	uecho_log_debug_l4("Leaving...\n");
}

/****************************************
* uecho_string_tokenizer_nexttoken
****************************************/

char *uecho_string_tokenizer_nexttoken(uEchoStringTokenizer *strToken)
{
	size_t tokenCnt;
	size_t i, j;
	
	uecho_log_debug_l4("Entering...\n");

	strToken->currToken = strToken->nextToken;
	strToken->nextToken = NULL;
	strToken->hasNextTokens = FALSE;
	strToken->repToken = '\0';
	
	tokenCnt = 0;
	for (i=strToken->nextStartPos; i<=strToken->lastPos; i++) {
		BOOL isDelimChar = FALSE;
		for (j=0; j<strToken->delimCnt; j++) {
			if (strToken->value[i] == strToken->delim[j]) {
				isDelimChar = TRUE;
				if (tokenCnt == 0) {
					strToken->nextStartPos = i + 1;
					break;
				}
				strToken->hasNextTokens = TRUE;
				strToken->repToken = strToken->value[i];
				strToken->value[i] = '\0';
				strToken->nextToken = strToken->value + strToken->nextStartPos;
				strToken->nextStartPos = i + 1;
			}
		}
		if (strToken->hasNextTokens == TRUE)
			break;
		if (isDelimChar == FALSE)
			tokenCnt++;
	}
	
	if (strToken->hasNextTokens == FALSE && 0 < tokenCnt) {
		strToken->hasNextTokens = TRUE;
		strToken->nextToken = strToken->value + strToken->nextStartPos;
		strToken->nextStartPos = strToken->lastPos + 1;
	}

	return strToken->currToken;

	uecho_log_debug_l4("Leaving...\n");
}

/****************************************
* uecho_string_tokenizer_nextalltoken
****************************************/

char *uecho_string_tokenizer_nextalltoken(uEchoStringTokenizer *strToken)
{
	size_t nextTokenLen;

	uecho_log_debug_l4("Entering...\n");

	nextTokenLen = uecho_strlen(strToken->nextToken);
	strToken->nextToken[nextTokenLen] = strToken->repToken;
	strToken->currToken = strToken->nextToken;
	strToken->nextToken = NULL;
	strToken->hasNextTokens = FALSE;
	return strToken->currToken;

	uecho_log_debug_l4("Leaving...\n");
}

/****************************************
* uecho_string_tokenizer_print
****************************************/

void uecho_string_tokenizer_print(uEchoStringTokenizer *strToken)
{
	uecho_log_debug_l4("Entering...\n");

uecho_log_debug_s(
		"uecho_string_tokenizer_print\n"
		"value = %s\n, delim = %s,\n delimCnt = %d,\n nextStartPos = %d,\n lastPos = %d,\n currToken = %s,\n nextToken = %s,\n repToken = %c,\n hasNextTokens = %d\n",
		strToken->value,
		strToken->delim,
		strToken->delimCnt,
		strToken->nextStartPos,
		strToken->lastPos,
		strToken->currToken,
		strToken->nextToken,
		strToken->repToken,
		strToken->hasNextTokens);

	uecho_log_debug_l4("Leaving...\n");
}
