/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdio.h>
#include <curses.h>
#include <ctype.h>

#include <uecho/uecho.h>

const int UECHO_TEST_SEARCH_WAIT_MTIME = 5000;

void uecho_print_multicastmessages(uEchoController *ctrl, uEchoMessage *msg)
{
  uEchoProperty *prop;
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

  for (n=0; n<opc; n++) {
    prop = uecho_message_getproperty(msg, n);
    printf("%02X", uecho_property_getcode(prop));
  }

  printf("\n");
}

void uecho_print_help()
{
  printf("'s' : Search\n");
  printf("'q' : Quit\n");
}

int main(int argc, char *argv[])
{
  uEchoController *ctrl;
  int key;
  
  ctrl = uecho_controller_new();
  if (!ctrl)
    return EXIT_FAILURE;
  
  uecho_controller_setmessagelistener(ctrl, uecho_print_multicastmessages);
  
  if (!uecho_controller_start(ctrl))
    return EXIT_FAILURE;

  do
  {
    key = getchar();
    
    if (key < 0) {
      uecho_sleep(100);
      continue;
    }
    
    switch (tolower(key)) {
      case 's':
        uecho_controller_searchallobjects(ctrl);
        break;
      default:
        uecho_print_help();
    }
  } while( key != 'q');
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
