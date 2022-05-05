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
#include <unistd.h>

#include <uecho/uecho.h>

const int UECHO_TEST_SEARCH_WAIT_MTIME = 5000;

void usage()
{
  printf("Usage : uechosearch [options]\n");
  printf(" -v : Enable verbose output\n");
  printf(" -h : Print this message\n");
}

uEchoMessage *create_readmessage(uEchoController* ctrl, byte prop_code)
{
  uEchoMessage* msg;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setproperty(msg, prop_code, NULL, 0);
  return msg;
}

void print_founddevices(uEchoController* ctrl, bool verbose)
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
  bool verbose_mode;
  uEchoController* ctrl;
  size_t found_node_cnt;
  int c;

  // Parse options

  verbose_mode = false;

  while ((c = getopt(argc, argv, "vh")) != -1) {
    switch (c) {
    case 'v': {
      verbose_mode = true;
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

  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;

  uecho_controller_search(ctrl);
  uecho_sleep(UECHO_TEST_SEARCH_WAIT_MTIME);

  found_node_cnt = uecho_controller_getnodecount(ctrl);
  if (0 < found_node_cnt) {
    print_founddevices(ctrl, verbose_mode);
  }

  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);

  return EXIT_SUCCESS;
}
