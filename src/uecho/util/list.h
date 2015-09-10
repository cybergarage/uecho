/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_LIST_H_
#define _UECHO_UTIL_LIST_H_

#include <uecho/typedef.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

typedef void (*UECHO_LIST_DESTRUCTORFUNC)(void *);

#define UECHO_LIST_STRUCT_MEMBERS \
  bool headFlag; \
  struct _uEchoList *prev; \
  struct _uEchoList *next;
  
/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoList {
  UECHO_LIST_STRUCT_MEMBERS
} uEchoList;

/****************************************
 * Functions
 ****************************************/

bool uecho_list_header_init(uEchoList *list);
bool uecho_list_node_init(uEchoList *list);

bool uecho_list_insert(uEchoList *prevList, uEchoList *list);
bool uecho_list_add(uEchoList *headList, uEchoList *list);
bool uecho_list_remove(uEchoList *list);

size_t uecho_list_size(uEchoList *headList);

uEchoList *uecho_list_gets(uEchoList *headList);
uEchoList *uecho_list_get(uEchoList *headList, int index);

uEchoList *uecho_list_prev_circular(uEchoList *list);
uEchoList *uecho_list_prev(uEchoList *list);
uEchoList *uecho_list_next_circular(uEchoList *list);
uEchoList *uecho_list_next(uEchoList *list);

bool uecho_list_clear(uEchoList *headList, UECHO_LIST_DESTRUCTORFUNC dstructorFunc);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif
