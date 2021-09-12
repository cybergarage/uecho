/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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

size_t uecho_list_size(uEchoList* headList)
{
  uEchoList* list;
  int listCnt;

  if (!headList)
    return 0;

  listCnt = 0;
  for (list = uecho_list_next(headList); list != NULL; list = uecho_list_next(list))
    listCnt++;

  return listCnt;
}

/****************************************
* uecho_list_get
****************************************/

uEchoList* uecho_list_get(uEchoList* headList, int index)
{
  uEchoList* list;
  int n;

  if (!headList)
    return NULL;

  list = uecho_list_next(headList);
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
    uEchoList* prevList,
    uEchoList* list)
{
  if (!prevList || !list)
    return false;

  list->prev = prevList;
  list->next = prevList->next;
  prevList->next->prev = list;
  prevList->next = list;

  return true;
}

/****************************************
* uecho_list_add
****************************************/

bool uecho_list_add(
    uEchoList* headList,
    uEchoList* list)
{
  if (!headList || !list)
    return false;

  if (!headList->prev)
    return false;

  uecho_list_insert(headList->prev, list);

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

uEchoList* uecho_list_gets(uEchoList* headList)
{
  if (!headList)
    return NULL;

  return uecho_list_next(headList);
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

bool uecho_list_clear(uEchoList* headList, UECHO_LIST_DESTRUCTORFUNC dstructorFunc)
{
  uEchoList *list, *nextList;

  if (!headList)
    return false;

  list = uecho_list_next(headList);
  while (list != NULL) {
    nextList = uecho_list_next(list);
    uecho_list_remove(list);
    if (dstructorFunc != NULL) {
      dstructorFunc(list);
    }
    else {
      free(list);
    }
    list = nextList;
  }

  return true;
}
