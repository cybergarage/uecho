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

#define UECHO_LISTEN_PORT 3610
#define UECHO_UDP_PORT UECHO_LISTEN_PORT
#define UECHO_TCPP_PORT UECHO_LISTEN_PORT

#define UECHO_MULTICAST_ADDR "224.0.23.0"
    
#ifdef  __cplusplus
}
#endif

#endif
