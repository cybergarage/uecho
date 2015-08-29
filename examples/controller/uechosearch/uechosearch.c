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

void uecho_search_messagelistener(uEchoController *ctrl, uEchoMessage *msg)
{
  uEchoProperty *prop;
  size_t opc, n;

  opc = uecho_message_getopc(msg);
  printf("Message Received : from = %s, esv = %X, opc = %ld, epc = ",
    uecho_message_getsourceaddress(msg),
    uecho_message_getesv(msg),
    opc);
  printf("%02X", uecho_property_getcode(prop));


  for (n=0; n<opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    printf("%02X", uecho_property_getcode(prop));
  }

  printf("\n");
}

void uecho_search_printdevices(uEchoController *ctrl)
{
  uEchoNode *node;
  uEchoObject *obj;
  int nodeNo, objNo;
  
  nodeNo = 0;
  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    printf("Node[%d] : %s\n", (nodeNo++), uecho_node_getaddress(node));
    objNo = 0;
    for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
      printf(" - Object[%d] : %X\n", (objNo++), uecho_object_getcode(obj));
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
  
  uecho_controller_setmessagelistener(ctrl, uecho_search_messagelistener);
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  uecho_controller_searchallobjects(ctrl);
  uecho_sleep(UECHO_TEST_SEARCH_WAIT_MTIME);
  
  foundNodeCnt = uecho_controller_getnodecount(ctrl);
  if (0 < foundNodeCnt) {
    if (foundNodeCnt == 1) {
      printf("==== Found a node !! ====\n");
    }
    else {
      printf("====  Found %ld nodes !! ====\n", foundNodeCnt);
    }
    uecho_search_printdevices(ctrl);
    
  }
  else {
    printf(" ==== Not found any nodes !! ====\n");
  }
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
