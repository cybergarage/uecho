/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>
#include <uecho/object.h>
#include <uecho/profile.h>

/****************************************
* uecho_objectcode2classcode
****************************************/

uEchoClassCode uecho_objectcode2classcode(uEchoObjectCode objCode)
{
  return ((objCode >> 8) & 0xFFFF);
}

/****************************************
 * uecho_isdeviceclassgroupcode
 ****************************************/

bool uecho_isdeviceclassgroupcode(byte code)
{
  if (uEchoClassGroupDeviceMax < code)
    return false;
  return true;
}

/****************************************
 * uecho_isprofileclassgroupcode
 ****************************************/

bool uecho_isprofileclassgroupcode(byte code)
{
  if (uEchoClassGroupProfile != code)
    return false;
  return true;
}

/****************************************
 * uecho_integer2byte
 ****************************************/

void uecho_integer2byte(int val, byte *bytes, size_t byteSize)
{
  size_t n, idx;
  
  for (n=0; n<byteSize; n++) {
    idx = ((byteSize-1)-n);
    bytes[idx] = (val >> (n * 8)) & 0xFF;
  }
}

/****************************************
 * uecho_byte2integer
 ****************************************/

int uecho_byte2integer(byte *bytes, size_t byteSize)
{
  int val = 0;
  size_t n, idx;

  for (n=0; n<byteSize; n++) {
    idx = ((byteSize-1)-n);
    val += (bytes[idx] << (n * 8));
  }
  
  return val;
}