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

void uecho_list_header_init(uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return;

	list->headFlag = TRUE;			
	list->prev = list->next = list;

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_list_node_init
****************************************/

void uecho_list_node_init(uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return;

	list->headFlag = FALSE;			
	list->prev = list->next = list;

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_list_size
****************************************/

int uecho_list_size(uEchoList *headList)
{
	uEchoList *list;
	int listCnt;
	
	uecho_log_debug_l5("Entering...\n");

	if (NULL == headList)
		return 0;

	listCnt = 0;
	for (list = uecho_list_next(headList); list != NULL; list = uecho_list_next(list))
		listCnt++;
	
	uecho_log_debug_l5("Leaving...\n");
	
	return listCnt;
}

/****************************************
* uecho_list_get
****************************************/

uEchoList *uecho_list_get(uEchoList *headList, int index)
{
	uEchoList *list;
	int n;
	
	uecho_log_debug_l5("Entering...\n");

	if (NULL == headList)
		return NULL;

	list = uecho_list_next(headList);
	for (n=0; n<index; n++) {
		if (NULL == list)
			break;
		list = uecho_list_next(list);
	}
		
	uecho_log_debug_l5("Leaving...\n");
	
	return list;
}

/****************************************
* uecho_list_insert
****************************************/

void uecho_list_insert(
uEchoList *prevList,
uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if ((NULL == prevList) || (NULL == list))
		return;

	list->prev = prevList;
	list->next = prevList->next;
	prevList->next->prev = list;
	prevList->next = list;

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_list_add
****************************************/

void uecho_list_add(
uEchoList *headList,
uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if ((NULL == headList) || (NULL == list))
		return;

	if (NULL == headList->prev)
		return;
	
	uecho_list_insert(headList->prev, list);

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_list_remove
****************************************/

void uecho_list_remove(uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return;

	if ((NULL == list->prev) || (NULL == list->next))
		return;
	
	list->prev->next = list->next;
	list->next->prev = list->prev;
	list->prev = list->next = list;

	uecho_log_debug_l5("Leaving...\n");
}

/****************************************
* uecho_list_prev_circular
****************************************/

uEchoList *uecho_list_prev_circular (
uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->prev)
		return NULL;
	
	if (list->prev->headFlag)
		list = list->prev;

	uecho_log_debug_l5("Leaving...\n");

	return list->prev;
}

/****************************************
* uecho_list_prev
****************************************/

uEchoList *uecho_list_prev(
uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->prev)
		return NULL;
	
	if (list->prev->headFlag == TRUE)
		return NULL;
	
	uecho_log_debug_l5("Leaving...\n");

	return list->prev;
}

/****************************************
* uecho_list_next_circular
****************************************/

uEchoList *uecho_list_next_circular(
uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->next)
		return NULL;
	
	if (list->next->headFlag == TRUE)
		list = list->next;

	uecho_log_debug_l5("Leaving...\n");
	
	return list->next;
}

/****************************************
* uecho_list_next
****************************************/

uEchoList *uecho_list_next(
uEchoList *list)
{
	uecho_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->next)
		return NULL;
	
	if (list->next->headFlag == TRUE)
		return NULL;

	uecho_log_debug_l5("Leaving...\n");

	return list->next;
}

/****************************************
* uecho_list_clear
****************************************/

void uecho_list_clear(uEchoList *headList, CG_LIST_DESTRUCTORFUNC destructorFunc)
{
	uEchoList *list;

	uecho_log_debug_l5("Entering...\n");

	if (NULL == headList)
		return;

	list = uecho_list_next(headList);
	while(list != NULL) {
		uecho_list_remove(list);
		//Theo Beisch: use destructorFunc or just free(listElement)
		if (destructorFunc != NULL){
			destructorFunc(list);
		} else {
			free(list);
		}
		list = uecho_list_next(headList);
	}

	/*** list header must be deleted by user ***/
	/* free(headList); */

	uecho_log_debug_l5("Leaving...\n");
}
