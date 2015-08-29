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
#include <curses.h>
 #include <ctype.h>

#include <uecho/uecho.h>

const int UECHO_TEST_SEARCH_WAIT_MTIME = 5000;

void uecho_print_multicastmessages(uEchoController *ctrl, uEchoMessage *msg)
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
  key = 0;
  do
  {
    key = getch();
    key = tolower(key);
    switch (key) {
      case 'S':
      default:
        uecho_print_help();
    }
  } while( key != 'q');
  
  uecho_controller_stop(ctrl);
  uecho_controller_delete(ctrl);
  
  return EXIT_SUCCESS;
}
