/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdlib.h>
#include <uecho/net/socket_opt.h>

/****************************************
* uecho_socket_option_new
****************************************/

uEchoSocketOption* uecho_socket_option_new()
{
  uEchoSocketOption* opt;

  opt = (uEchoSocketOption*)malloc(sizeof(uEchoSocketOption));
  if (!opt)
    return NULL;

  uecho_socket_option_setreuseaddress(opt, false);
  uecho_socket_option_setbindinterface(opt, false);
  uecho_socket_option_setmulticastloop(opt, false);

  return opt;
}

/****************************************
* uecho_socket_option_delete
****************************************/

bool uecho_socket_option_delete(uEchoSocketOption* opt)
{
  if (!opt)
    return true;

  free(opt);

  return true;
}
