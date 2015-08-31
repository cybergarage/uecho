/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <signal.h>

#include "lighting_dev.h"

void uecho_lighting_node_messagelitener(uEchoNode *obj, uEchoMessage *msg)
{
  uecho_lighting_printrequest(msg);
}

int main(int argc, char *argv[])
{
  uEchoNode *node;
  uEchoObject *obj;
  
  node = uecho_node_new();
  if (!node)
    return EXIT_FAILURE;
  
#if defined(DEBUG)
  uecho_node_setmessagelistener(node, uecho_lighting_node_messagelitener);
#endif
  
  obj = uecho_create_lighting_deviceobject();
  if (!obj) {
    uecho_node_delete(node);
    return EXIT_FAILURE;
  }
  
  uecho_node_addobject(node, obj);

  if (!uecho_node_start(node)) {
    return EXIT_FAILURE;
  }
  
  while (uecho_node_isrunning(node)) {
    sigset_t sigSet;
    if (sigfillset(&sigSet) != 0)
      break;
    
    int sigNo;
    if (sigwait(&sigSet, &sigNo) != 0)
      break;
    
    switch (sigNo) {
      case SIGTERM:
      case SIGINT:
      case SIGKILL:
        {
          uecho_node_stop(node);
        }
        break;
      case SIGHUP:
        {
          uecho_node_stop(node);
          uecho_node_start(node);
        }
        break;
    }
  }
  
  uecho_node_delete(node);
  
  return EXIT_SUCCESS;
}

