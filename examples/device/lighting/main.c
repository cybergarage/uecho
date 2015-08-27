/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include "lighting_dev.h"

int main(int argc, char *argv[])
{
  uEchoNode *node;
  
  node = uecho_create_lighting_node();
  uecho_node_delete(node);
  
  return EXIT_SUCCESS;
}

