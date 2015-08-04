/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONST_H_
#define _UECHO_CONST_H_

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Constants
****************************************/

#define uEchoMulticastAddr "224.0.23.0"
  
enum {
  uEchoUdpPort = 3610,
  uEchoTcpPort = 3610,

  uEchoTidMin = 0,
  uEchoTidMax = 65535,
};
  
#ifdef  __cplusplus
}
#endif

#endif
