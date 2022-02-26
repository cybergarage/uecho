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
#include <unistd.h>

#include "lighting_dev.h"

void usage()
{
  printf("Usage : uecholight\n");
  printf(" -v        : Enable verbose output\n");
  printf(" -m XXXXXX : Set Manifacture code\n");
  printf(" -h        : Print this message\n");
}

void uecho_lighting_node_messagelitener(uEchoNode* obj, uEchoMessage* msg)
{
  uecho_light_printrequest(msg);
}

int main(int argc, char* argv[])
{
  bool verbose_mode;
  int manifacture_code;
  int c;
  uEchoNode* node;
  uEchoObject* obj;

  // Parse options

  verbose_mode = false;
  manifacture_code = 0;

  while ((c = getopt(argc, argv, "vhm:")) != -1) {
    switch (c) {
    case 'v': {
      verbose_mode = true;
    } break;
    case 'm': {
      sscanf(optarg, "%X", &manifacture_code);
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

  // Start node

  node = uecho_node_new();
  if (!node)
    return EXIT_FAILURE;

  if (verbose_mode) {
    uecho_node_setmessagelistener(node, uecho_lighting_node_messagelitener);
  }

  obj = uecho_light_new();
  if (!obj) {
    uecho_node_delete(node);
    return EXIT_FAILURE;
  }

  if (0 < manifacture_code) {
    uecho_node_setmanufacturercode(node, manifacture_code);
  }

  uecho_node_addobject(node, obj);

  if (!uecho_node_start(node)) {
    return EXIT_FAILURE;
  }

  while (uecho_node_isrunning(node)) {
    sigset_t sig_set;
    if (sigfillset(&sig_set) != 0)
      break;

    int sig_no;
    if (sigwait(&sig_set, &sig_no) != 0)
      break;

    switch (sig_no) {
    case SIGTERM:
    case SIGINT:
    case SIGKILL: {
      uecho_node_stop(node);
    } break;
    case SIGHUP: {
      uecho_node_stop(node);
      uecho_node_start(node);
    } break;
    }
  }

  uecho_node_delete(node);

  return EXIT_SUCCESS;
}
