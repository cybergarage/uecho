/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller.h>

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  
  ctrl = uecho_controller_new();
  
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}

