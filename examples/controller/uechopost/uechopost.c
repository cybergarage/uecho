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

void uecho_post_print_usage()
{
  printf("echopost <address> <obj> <esv> <property ...>\n");
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  
  if (argc < 5) {
    uecho_post_print_usage();
    return EXIT_FAILURE;
  }
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  uecho_controller_searchallobjects(ctrl);
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
