/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/uecho.h>

const int UECHO_TEST_SEARCH_WAIT_MTIME = 5000;

void uecho_search_messagelistener(uEchoController *ctrl, uEchoMessage *msg)
{
}

void uecho_search_printdevices(uEchoController *ctrl)
{
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  uecho_controller_setmessagerequeslistener(ctrl, uecho_search_messagelistener);
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;
  
  uecho_sleep(UECHO_TEST_SEARCH_WAIT_MTIME);
  
  uecho_search_printdevices(ctrl);
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
