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

uEchoObject *uecho_lighting_device_new(void)
{
  uEchoObject *obj = uecho_object_new();
  return obj;
}

uEchoNode *uecho_create_lighting_node(void)
{
  uEchoNode *node = uecho_node_new();
  return node;
}