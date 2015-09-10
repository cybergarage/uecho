/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <unistd.h>
#include <signal.h>

#include "lighting_dev.h"

void usage()
{
  printf("Usage : uecholight\n");
  printf(" -v        : Enable verbose output\n");
  printf(" -m XXXXXX : Set Manifacture code\n");
  printf(" -h        : Print this message\n");
}

void uecho_lighting_node_messagelitener(uEchoNode *obj, uEchoMessage *msg)
{
  uecho_lighting_printrequest(msg);
}

int main(int argc, char *argv[])
{
  bool verboseMode;
  int manifactureCode;
  int c;
  uEchoNode *node;
  uEchoObject *obj;
  
  // Parse options
  
  verboseMode = false;
  manifactureCode = 0;
  
  while ((c = getopt(argc, argv, "vhm:")) != -1) {
    switch (c) {
      case 'v':
        {
          verboseMode = true;
        }
        break;
      case 'm':
        {
          sscanf(optarg, "%X", &manifactureCode);
        }
        break;
      case 'h':
        {
          usage();
          return EXIT_SUCCESS;
        }
      default:
        {
          usage();
          return EXIT_FAILURE;
        }
    }
  }
  
  argc -= optind;
  argv += optind;

  // Start node
  
  node = uecho_node_new();
  if (!node)
    return EXIT_FAILURE;

  if (verboseMode) {
    uecho_node_setmessagelistener(node, uecho_lighting_node_messagelitener);
  }
  
  obj = uecho_create_lighting_deviceobject();
  if (!obj) {
    uecho_node_delete(node);
    return EXIT_FAILURE;
  }

  if (0 < manifactureCode) {
    uecho_node_setmanufacturercode(node, manifactureCode);
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

