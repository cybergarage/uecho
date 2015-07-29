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
  
	return strToken;
}

/****************************************
* uecho_string_tokenizer_delete
****************************************/

void uecho_string_tokenizer_delete(uEchoStringTokenizer *strToken)
{
	free(strToken->value);
	free(strToken->delim);
	free(strToken);
}

/****************************************
* uecho_string_tokenizer_hasmoretoken
****************************************/

bool uecho_string_tokenizer_hasmoretoken(uEchoStringTokenizer *strToken)
{
	return strToken->hasNextTokens;
}

/****************************************
* uecho_string_tokenizer_nexttoken
****************************************/

char *uecho_string_tokenizer_nexttoken(uEchoStringTokenizer *strToken)
{
	size_t tokenCnt;
	size_t i, j;
	
	strToken->currToken = strToken->nextToken;
	strToken->nextToken = NULL;
	strToken->hasNextTokens = false;
	strToken->repToken = '\0';
	
	tokenCnt = 0;
	for (i=strToken->nextStartPos; i<=strToken->lastPos; i++) {
		bool isDelimChar = false;
		for (j=0; j<strToken->delimCnt; j++) {
			if (strToken->value[i] == strToken->delim[j]) {
				isDelimChar = true;
				if (tokenCnt == 0) {
					strToken->nextStartPos = i + 1;
					break;
				}
				strToken->hasNextTokens = true;
				strToken->repToken = strToken->value[i];
				strToken->value[i] = '\0';
				strToken->nextToken = strToken->value + strToken->nextStartPos;
				strToken->nextStartPos = i + 1;
			}
		}
		if (strToken->hasNextTokens == true)
			break;
		if (isDelimChar == false)
			tokenCnt++;
	}
	
	if (strToken->hasNextTokens == false && 0 < tokenCnt) {
		strToken->hasNextTokens = true;
		strToken->nextToken = strToken->value + strToken->nextStartPos;
		strToken->nextStartPos = strToken->lastPos + 1;
	}

	return strToken->currToken;
}

/****************************************
* uecho_string_tokenizer_nextalltoken
****************************************/

char *uecho_string_tokenizer_nextalltoken(uEchoStringTokenizer *strToken)
{
	size_t nextTokenLen;

	nextTokenLen = uecho_strlen(strToken->nextToken);
	strToken->nextToken[nextTokenLen] = strToken->repToken;
	strToken->currToken = strToken->nextToken;
	strToken->nextToken = NULL;
	strToken->hasNextTokens = false;
	return strToken->currToken;
}
