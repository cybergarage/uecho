/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MISC_H_
#define _UECHO_MISC_H_

#include <uecho/class.h>
#include <uecho/object.h>

#ifdef  __cplusplus
extern "C" {
#endif
  
/****************************************
 * Function
 ****************************************/

uEchoClassCode uecho_objectcode2classcode(uEchoObjectCode objCode);

void uecho_integer2byte(int val, byte *bytes, size_t byteSize);
int uecho_byte2integer(byte *bytes, size_t byteSize);

#ifdef  __cplusplus
}
#endif

#endif
