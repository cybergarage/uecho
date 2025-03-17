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

/****************************************
 * uecho_string_tokenizer_new
 ****************************************/

uEchoStringTokenizer* uecho_string_tokenizer_new(const char* value, const char* delim)
{
  uEchoStringTokenizer* str_token;

  str_token = (uEchoStringTokenizer*)malloc(sizeof(uEchoStringTokenizer));

  if (NULL != str_token) {
    str_token->value = uecho_strdup(value);
    str_token->delim = uecho_strdup(delim);
    str_token->delimCnt = uecho_strlen(str_token->delim);
    str_token->nextStartPos = 0;
    str_token->lastPos = uecho_strlen(value) - 1;
    str_token->currToken = NULL;
    str_token->nextToken = NULL;
    uecho_string_tokenizer_nexttoken(str_token);
  }

  return str_token;
}

/****************************************
 * uecho_string_tokenizer_delete
 ****************************************/

void uecho_string_tokenizer_delete(uEchoStringTokenizer* str_token)
{
  free(str_token->value);
  free(str_token->delim);
  free(str_token);
}

/****************************************
 * uecho_string_tokenizer_hasmoretoken
 ****************************************/

bool uecho_string_tokenizer_hasmoretoken(uEchoStringTokenizer* str_token)
{
  return str_token->hasNextTokens;
}

/****************************************
 * uecho_string_tokenizer_nexttoken
 ****************************************/

char* uecho_string_tokenizer_nexttoken(uEchoStringTokenizer* str_token)
{
  size_t token_cnt;
  size_t i, j;

  str_token->currToken = str_token->nextToken;
  str_token->nextToken = NULL;
  str_token->hasNextTokens = false;
  str_token->repToken = '\0';

  token_cnt = 0;
  for (i = str_token->nextStartPos; i <= str_token->lastPos; i++) {
    bool is_delim_char = false;
    for (j = 0; j < str_token->delimCnt; j++) {
      if (str_token->value[i] == str_token->delim[j]) {
        is_delim_char = true;
        if (token_cnt == 0) {
          str_token->nextStartPos = i + 1;
          break;
        }
        str_token->hasNextTokens = true;
        str_token->repToken = str_token->value[i];
        str_token->value[i] = '\0';
        str_token->nextToken = str_token->value + str_token->nextStartPos;
        str_token->nextStartPos = i + 1;
      }
    }
    if (str_token->hasNextTokens == true)
      break;
    if (is_delim_char == false)
      token_cnt++;
  }

  if (str_token->hasNextTokens == false && 0 < token_cnt) {
    str_token->hasNextTokens = true;
    str_token->nextToken = str_token->value + str_token->nextStartPos;
    str_token->nextStartPos = str_token->lastPos + 1;
  }

  return str_token->currToken;
}

/****************************************
 * uecho_string_tokenizer_nextalltoken
 ****************************************/

char* uecho_string_tokenizer_nextalltoken(uEchoStringTokenizer* str_token)
{
  size_t nextToken_len;

  nextToken_len = uecho_strlen(str_token->nextToken);
  str_token->nextToken[nextToken_len] = str_token->repToken;
  str_token->currToken = str_token->nextToken;
  str_token->nextToken = NULL;
  str_token->hasNextTokens = false;
  return str_token->currToken;
}
