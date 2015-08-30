/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdio.h>
#include <uecho/uecho.h>

const int UECHO_TEST_SEARCH_WAIT_MTIME = 5000;

void uecho_search_printdevices(uEchoController *ctrl)
{
  uEchoNode *node;
  uEchoObject *obj;
  
  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
      printf("%s %06X\n", uecho_node_getaddress(node), uecho_object_getcode(obj));
    }
  }
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  size_t foundNodeCnt;
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  uecho_controller_searchallobjects(ctrl);
  uecho_sleep(UECHO_TEST_SEARCH_WAIT_MTIME);
  
  foundNodeCnt = uecho_controller_getnodecount(ctrl);
  if (0 < foundNodeCnt) {
    uecho_search_printdevices(ctrl);
    
  }
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
