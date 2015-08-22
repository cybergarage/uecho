/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>

/****************************************
* uecho_node_new
****************************************/

uEchoNode *uecho_node_new(void)
{
	uEchoNode *node;

	node = (uEchoNode *)malloc(sizeof(uEchoNode));

  if (!node)
    return NULL;
        
  uecho_list_node_init((uEchoList *)node);
  
  node->mutex = uecho_mutex_new();
  node->classes = uecho_classlist_new();
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
  uecho_classlist_delete(node->classes);
  uecho_objectlist_delete(node->objects);
  
	free(node);
}

/****************************************
* uecho_node_clear
****************************************/

void uecho_node_clear(uEchoNode *node)
{
}

/****************************************
 * uecho_node_getclasses
 ****************************************/

uEchoClass *uecho_node_getclasses(uEchoNode *node)
{
  return uecho_classlist_gets(node->classes);
}

/****************************************
 * uecho_node_getclassbycode
 ****************************************/

uEchoClass *uecho_node_getclassbycode(uEchoNode *node, uEchoObjectCode code)
{
  return uecho_classlist_getbycode(node->classes, code);
}

/****************************************
 * uecho_node_getclasscount
 ****************************************/

size_t uecho_node_getclasscount(uEchoNode *node)
{
  return uecho_classlist_size(node->classes);
}

/****************************************
 * uecho_node_setobject
 ****************************************/

bool uecho_node_setobject(uEchoNode *node, uEchoObjectCode code)
{
  return uecho_objectlist_set(node->objects, code);
}
  
/****************************************
 * uecho_node_getobjects
 ****************************************/

uEchoObject *uecho_node_getobjects(uEchoNode *node)
{
  return uecho_objectlist_gets(node->objects);
}
    
/****************************************
 * uecho_node_getobjectbycode
 ****************************************/

uEchoObject *uecho_node_getobjectbycode(uEchoNode *node, uEchoObjectCode code)
{
  return uecho_objectlist_getbycode(node->objects, code);
}
      
/****************************************
 * uecho_node_getobjectcount
 ****************************************/

size_t uecho_node_getobjectcount(uEchoNode *node)
{
   return uecho_objectlist_size(node->objects);
}
