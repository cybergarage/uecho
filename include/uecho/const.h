/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONST_H_
#define _UECHO_CONST_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define uEchoMulticastAddr "224.0.23.0"
  
enum {
  uEchoMajorVersion = 0x02,
  uEchoMinorVersion = 0x0A,
  
  uEchoUdpPort = 3610,
  uEchoTcpPort = 3610,

  uEchoTidMin = 0,
  uEchoTidMax = 65535,
};

typedef enum {
  uEchoSpecifiedMessageFormat = 0x01,
  uEchoArbitraryMessageFormat = 0x02,
} uEchoMessageFormatType;

enum {
  uEchoManufacturerCodeTestMin = 0xFFFFF0,
  uEchoManufacturerCodeTestMax = 0xFFFFFF,
};
  
#ifdef  __cplusplus
}
#endif

#endif
