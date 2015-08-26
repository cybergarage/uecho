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
* uecho_nodelist_new
****************************************/

uEchoNodeList *uecho_nodelist_new(void)
{
	uEchoNodeList *nodes;

	nodes = (uEchoNodeList *)malloc(sizeof(uEchoNodeList));
  if (!nodes)
    return NULL;

  uecho_list_header_init((uEchoList *)nodes);

	return nodes;
}

/****************************************
* uecho_nodelist_delete
****************************************/

void uecho_nodelist_delete(uEchoNodeList *nodes)
{
	uecho_nodelist_clear(nodes);

	free(nodes);
}

/****************************************
 * uecho_nodelist_getbycode
 ****************************************/

uEchoNode *uecho_nodelist_getbycode(uEchoNodeList *nodes, const  code)
{
  uEchoNode *obj;
  
  for (obj = uecho_nodelist_gets(nodes); obj; obj = uecho_class_next(obj)) {
    if (uecho_class_getcode(obj) == code)
      return obj;
  }
  
  return NULL;
}
