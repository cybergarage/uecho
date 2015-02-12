/******************************************************************
 *
 * mUPnP for C
 *
 * Copyright (C) The uecho Authors 2005
 * Copyright (C) 2006 Nokia Corporation. All rights reserved.
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

/**
 * Prototype for individual list node destructor functions
 *
 */
typedef void (*CG_LIST_DESTRUCTORFUNC)(void *);

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic list interface in CyberLinkC.
 *
 * Each struct in CyberLinkC, that is designed to be a part of
 * a list, must have these elements in their definition before the
 * actual struct definition. The struct pointers are then cast to
 * uEchoList* and operated with uecho_list_* functions.
 */
typedef struct _uEchoList {

  /** Marks the beginning of a list */
  BOOL headFlag;
  /** Pointer to the next list node */
  struct _uEchoList *prev;
  /** Pointer to the previous list node */
  struct _uEchoList *next;

} uEchoList;

/****************************************
 * Functions
 ****************************************/

/**
 * Initialize a list node to act as the first node
 *
 * \param list List node
 */
void uecho_list_header_init(uEchoList *list);

/**
 * Initialize a list node to act as a regular node, part of a list.
 *
 * \param list List node
 */
void uecho_list_node_init(uEchoList *list);

/**
 * Insert a list node or a complete list structure after the given node
 *
 * \param prevList Insert after this node
 * \param list List node or list structure to insert
 */
void uecho_list_insert(uEchoList *prevList, uEchoList *list);

/**
 * \todo This works essentially like insert, although it is more error-prone?!
 * \todo There might be a bug in this function.
 *
 */
void uecho_list_add(uEchoList *headList, uEchoList *list);

/**
 * Remove a node from a list. Does not free any memory, but only removes
 * the next and previous link associations regarding the given node.
 *
 * \param list List node to remove
 */
void uecho_list_remove(uEchoList *list);

/**
 * Get the number of nodes in the current list structure. Counts forwards from the given
 * node, so if you want to get the complete size, give a header node as the parameter.
 *
 * \param headList List header
 */
int uecho_list_size(uEchoList *headList);

/**
 * Get an item from the list by the item's index
 *
 * \param headList List header
 * \param index The index of the item to get
 */
uEchoList *uecho_list_get(uEchoList *headList, int index);

/**
 * Get the previous node. Wrap around if the beginning has been reached.
 * 
 *
 * \param list Current node
 */
uEchoList *uecho_list_prev_circular(uEchoList *list);

/**
 * Get the previous node. Returns NULL if beginning has been reached
 *
 * \param list Current node
 */
uEchoList *uecho_list_prev(uEchoList *list);

/**
 * Get the next node. Wrap around if the end has been reached.
 *
 * \param list Current node
 */
uEchoList *uecho_list_next_circular(uEchoList *list);

/**
 * Get the next node. Returns NULL if end has been reached.
 *
 * \param list Current node
 */
uEchoList *uecho_list_next(uEchoList *list);

/**
 * Clear the list and delete all of its contents with \ref CG_LIST_DESTRUCTORFUNC
 *
 * \param headList List header
 * \param destructorFunc Function pointer that clears the contents of individual nodes
 */
void uecho_list_clear(uEchoList *headList, CG_LIST_DESTRUCTORFUNC destructorFunc);

/**
 * Get the first actual item from a list for iteration
 *
 * \param headList List header
 */
#define uecho_list_gets(headList) uecho_list_next(headList)

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif
