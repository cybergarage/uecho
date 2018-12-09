/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_NET_CSOCKET_OPTION_H_
#define _UECHO_NET_CSOCKET_OPTION_H_

#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Data Type
****************************************/

typedef struct _uEchoSocketOption {
  bool reuse;
  bool bind;
  bool loop;
} uEchoSocketOption;

/****************************************
* Function (Socket)
****************************************/

uEchoSocketOption *uecho_socket_option_new();
bool uecho_socket_option_delete(uEchoSocketOption *opt);

#define uecho_socket_option_setreuseaddress(opt, flag) ((opt)->reuse = flag)
#define uecho_socket_option_isreuseaddress(opt) ((opt)->reuse)

#define uecho_socket_option_setbindinterface(opt, flag) ((opt)->bind = flag)
#define uecho_socket_option_isbindinterface(opt) ((opt)->bind)

#define uecho_socket_option_setmulticastloop(opt, flag) ((opt)->loop = flag)
#define uecho_socket_option_ismulticastloop(opt) ((opt)->loop)

#ifdef  __cplusplus
}
#endif

#endif
