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
    str_token->delim_cnt = uecho_strlen(str_token->delim);
    str_token->next_start_pos = 0;
    str_token->last_pos = uecho_strlen(value) - 1;
    str_token->curr_token = NULL;
    str_token->next_token = NULL;
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
  return str_token->has_next_tokens;
}

/****************************************
* uecho_string_tokenizer_nexttoken
****************************************/

char* uecho_string_tokenizer_nexttoken(uEchoStringTokenizer* str_token)
{
  size_t token_cnt;
  size_t i, j;

  str_token->curr_token = str_token->next_token;
  str_token->next_token = NULL;
  str_token->has_next_tokens = false;
  str_token->rep_token = '\0';

  token_cnt = 0;
  for (i = str_token->next_start_pos; i <= str_token->last_pos; i++) {
    bool is_delim_char = false;
    for (j = 0; j < str_token->delim_cnt; j++) {
      if (str_token->value[i] == str_token->delim[j]) {
        is_delim_char = true;
        if (token_cnt == 0) {
          str_token->next_start_pos = i + 1;
          break;
        }
        str_token->has_next_tokens = true;
        str_token->rep_token = str_token->value[i];
        str_token->value[i] = '\0';
        str_token->next_token = str_token->value + str_token->next_start_pos;
        str_token->next_start_pos = i + 1;
      }
    }
    if (str_token->has_next_tokens == true)
      break;
    if (is_delim_char == false)
      token_cnt++;
  }

  if (str_token->has_next_tokens == false && 0 < token_cnt) {
    str_token->has_next_tokens = true;
    str_token->next_token = str_token->value + str_token->next_start_pos;
    str_token->next_start_pos = str_token->last_pos + 1;
  }

  return str_token->curr_token;
}

/****************************************
* uecho_string_tokenizer_nextalltoken
****************************************/

char* uecho_string_tokenizer_nextalltoken(uEchoStringTokenizer* str_token)
{
  size_t next_token_len;

  next_token_len = uecho_strlen(str_token->next_token);
  str_token->next_token[next_token_len] = str_token->rep_token;
  str_token->curr_token = str_token->next_token;
  str_token->next_token = NULL;
  str_token->has_next_tokens = false;
  return str_token->curr_token;
}
