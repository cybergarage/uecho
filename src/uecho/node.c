/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>

/****************************************
* uecho_node_new
****************************************/

uEchoNode *uecho_node_new()
{
	uEchoNode *node;

	node = (uEchoNode *)malloc(sizeof(uEchoNode));

  if (!node)
    return NULL;
        
  uecho_list_node_init((uEchoList *)node);
  
  node->mutex = uecho_mutex_new();
  node->objects = uecho_objectlist_new();
  
	return node;
}

/****************************************
* uecho_node_delete
****************************************/

void uecho_node_delete(uEchoNode *node)
{
	uecho_list_remove((uEchoList *)node);
  
  uecho_mutex_delete(node->mutex);
  uecho_objectlist_delete(node->objects);
  
	free(node);
}

/****************************************
* uecho_node_clear
****************************************/

void uecho_node_clear(uEchoNode *node)
{
}
