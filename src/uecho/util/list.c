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

void uecho_list_header_init(uEchoList *list)
{
	if (NULL == list)
		return;

	list->headFlag = true;			
	list->prev = list->next = list;
}

/****************************************
* uecho_list_node_init
****************************************/

void uecho_list_node_init(uEchoList *list)
{
	if (NULL == list)
		return;

	list->headFlag = false;			
	list->prev = list->next = list;
}

/****************************************
* uecho_list_size
****************************************/

int uecho_list_size(uEchoList *headList)
{
	uEchoList *list;
	int listCnt;

	if (NULL == headList)
		return 0;

	listCnt = 0;
	for (list = uecho_list_next(headList); list != NULL; list = uecho_list_next(list))
		listCnt++;
	
	return listCnt;
}

/****************************************
* uecho_list_get
****************************************/

uEchoList *uecho_list_get(uEchoList *headList, int index)
{
	uEchoList *list;
	int n;
	
	if (NULL == headList)
		return NULL;

	list = uecho_list_next(headList);
	for (n=0; n<index; n++) {
		if (NULL == list)
			break;
		list = uecho_list_next(list);
	}
	
	return list;
}

/****************************************
* uecho_list_insert
****************************************/

void uecho_list_insert(
uEchoList *prevList,
uEchoList *list)
{
	if ((NULL == prevList) || (NULL == list))
		return;

	list->prev = prevList;
	list->next = prevList->next;
	prevList->next->prev = list;
	prevList->next = list;
}

/****************************************
* uecho_list_add
****************************************/

void uecho_list_add(
uEchoList *headList,
uEchoList *list)
{
	if ((NULL == headList) || (NULL == list))
		return;

	if (NULL == headList->prev)
		return;
	
	uecho_list_insert(headList->prev, list);
}

/****************************************
* uecho_list_remove
****************************************/

void uecho_list_remove(uEchoList *list)
{
	if (NULL == list)
		return;

	if ((NULL == list->prev) || (NULL == list->next))
		return;
	
	list->prev->next = list->next;
	list->next->prev = list->prev;
	list->prev = list->next = list;
}

/****************************************
* uecho_list_prev_circular
****************************************/

uEchoList *uecho_list_prev_circular (
uEchoList *list)
{
	if (NULL == list)
		return NULL;

	if (NULL == list->prev)
		return NULL;
	
	if (list->prev->headFlag)
		list = list->prev;

	return list->prev;
}

/****************************************
* uecho_list_prev
****************************************/

uEchoList *uecho_list_prev(
uEchoList *list)
{
	if (NULL == list)
		return NULL;

	if (NULL == list->prev)
		return NULL;
	
	if (list->prev->headFlag == true)
		return NULL;

	return list->prev;
}

/****************************************
* uecho_list_next_circular
****************************************/

uEchoList *uecho_list_next_circular(
uEchoList *list)
{
	if (NULL == list)
		return NULL;

	if (NULL == list->next)
		return NULL;
	
	if (list->next->headFlag == true)
		list = list->next;
	
	return list->next;
}

/****************************************
* uecho_list_next
****************************************/

uEchoList *uecho_list_next(
uEchoList *list)
{
	if (NULL == list)
		return NULL;

	if (NULL == list->next)
		return NULL;
	
	if (list->next->headFlag == true)
		return NULL;

	return list->next;
}

/****************************************
* uecho_list_clear
****************************************/

void uecho_list_clear(uEchoList *headList, CG_LIST_DESTRUCTORFUNC destructorFunc)
{
	uEchoList *list;

	if (NULL == headList)
		return;

	list = uecho_list_next(headList);
	while(list != NULL) {
		uecho_list_remove(list);
		if (destructorFunc != NULL){
			destructorFunc(list);
		} else {
			free(list);
		}
		list = uecho_list_next(headList);
	}
}
