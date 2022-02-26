/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/list.h>

/****************************************
* uecho_list_header_init
****************************************/

bool uecho_list_header_init(uEchoList* list)
{
  if (!list)
    return false;

  list->headFlag = true;
  list->prev = list->next = list;

  return true;
}

/****************************************
* uecho_list_node_init
****************************************/

bool uecho_list_node_init(uEchoList* list)
{
  if (!list)
    return false;

  list->headFlag = false;
  list->prev = list->next = list;

  return true;
}

/****************************************
* uecho_list_size
****************************************/

size_t uecho_list_size(uEchoList* head_list)
{
  uEchoList* list;
  int list_cnt;

  if (!head_list)
    return 0;

  list_cnt = 0;
  for (list = uecho_list_next(head_list); list != NULL; list = uecho_list_next(list))
    list_cnt++;

  return list_cnt;
}

/****************************************
* uecho_list_get
****************************************/

uEchoList* uecho_list_get(uEchoList* head_list, int index)
{
  uEchoList* list;
  int n;

  if (!head_list)
    return NULL;

  list = uecho_list_next(head_list);
  for (n = 0; n < index; n++) {
    if (!list)
      break;
    list = uecho_list_next(list);
  }

  return list;
}

/****************************************
* uecho_list_insert
****************************************/

bool uecho_list_insert(
    uEchoList* prev_list,
    uEchoList* list)
{
  if (!prev_list || !list)
    return false;

  list->prev = prev_list;
  list->next = prev_list->next;
  prev_list->next->prev = list;
  prev_list->next = list;

  return true;
}

/****************************************
* uecho_list_add
****************************************/

bool uecho_list_add(
    uEchoList* head_list,
    uEchoList* list)
{
  if (!head_list || !list)
    return false;

  if (!head_list->prev)
    return false;

  uecho_list_insert(head_list->prev, list);

  return true;
}

/****************************************
* uecho_list_remove
****************************************/

bool uecho_list_remove(uEchoList* list)
{
  if (!list)
    return false;

  if (!list->prev || !list->next)
    return false;

  list->prev->next = list->next;
  list->next->prev = list->prev;
  list->prev = list->next = list;

  return true;
}

/****************************************
* uecho_list_prev_circular
****************************************/

uEchoList* uecho_list_prev_circular(
    uEchoList* list)
{
  if (!list)
    return NULL;

  if (!list->prev)
    return NULL;

  if (list->prev->headFlag)
    list = list->prev;

  return list->prev;
}

/****************************************
* uecho_list_prev
****************************************/

uEchoList* uecho_list_prev(
    uEchoList* list)
{
  if (!list)
    return NULL;

  if (!list->prev)
    return NULL;

  if (list->prev->headFlag)
    return NULL;

  return list->prev;
}

/****************************************
* uecho_list_next_circular
****************************************/

uEchoList* uecho_list_next_circular(
    uEchoList* list)
{
  if (!list)
    return NULL;

  if (!list->next)
    return NULL;

  if (list->next->headFlag == true)
    list = list->next;

  return list->next;
}

/****************************************
 * uecho_list_gets
 ****************************************/

uEchoList* uecho_list_gets(uEchoList* head_list)
{
  if (!head_list)
    return NULL;

  return uecho_list_next(head_list);
}

/****************************************
* uecho_list_next
****************************************/

uEchoList* uecho_list_next(uEchoList* list)
{
  if (!list)
    return NULL;

  if (!list->next)
    return NULL;

  if (list->next->headFlag == true)
    return NULL;

  return list->next;
}

/****************************************
* uecho_list_clear
****************************************/

bool uecho_list_clear(uEchoList* head_list, UECHO_LIST_DESTRUCTORFUNC dstructor_func)
{
  uEchoList *list, *next_list;

  if (!head_list)
    return false;

  list = uecho_list_next(head_list);
  while (list != NULL) {
    next_list = uecho_list_next(list);
    uecho_list_remove(list);
    if (dstructor_func != NULL) {
      dstructor_func(list);
    }
    else {
      free(list);
    }
    list = next_list;
  }

  return true;
}
