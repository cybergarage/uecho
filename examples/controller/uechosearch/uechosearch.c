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
#include <unistd.h>

const int UECHO_TEST_SEARCH_WAIT_MTIME = 5000;

void usage()
{
  printf("Usage : uechosearch [options]\n");
  printf(" -v : Enable verbose output\n");
  printf(" -h : Print this message\n");
}

void uecho_search_print_messages(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoProperty* prop;
  size_t opc, n;

  opc = uecho_message_getopc(msg);
  printf("%s %1X %1X %02X %03X %03X %02X %ld ",
      uecho_message_getsourceaddress(msg),
      uecho_message_getehd1(msg),
      uecho_message_getehd2(msg),
      uecho_message_gettid(msg),
      uecho_message_getsourceobjectcode(msg),
      uecho_message_getdestinationobjectcode(msg),
      uecho_message_getesv(msg),
      opc);

  for (n = 0; n < opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    printf("%02X", uecho_property_getcode(prop));
  }

  printf("\n");
}

void uecho_search_printdevices(uEchoController* ctrl)
{
  uEchoNode* node;
  uEchoObject* obj;

  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    if (uecho_node_getobjectcount(node) <= 0) {
      printf("%s\n", uecho_node_getaddress(node));
      continue;
    }
    for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
      printf("%s %06X\n", uecho_node_getaddress(node), uecho_object_getcode(obj));
    }
  }
}

int main(int argc, char* argv[])
{
  bool verboseMode;
  uEchoController* ctrl;
  size_t foundNodeCnt;
  int c;

  // Parse options

  verboseMode = false;

  while ((c = getopt(argc, argv, "vh")) != -1) {
    switch (c) {
    case 'v': {
      verboseMode = true;
    } break;
    case 'h': {
      usage();
      return EXIT_SUCCESS;
    }
    default: {
      usage();
      return EXIT_FAILURE;
    }
    }
  }

  argc -= optind;
  argv += optind;

  // Start controller

  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;

  if (verboseMode) {
    uecho_controller_setmessagelistener(ctrl, uecho_search_print_messages);
  }

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
