/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
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

uEchoClassCode uecho_objectcode2classcode(uEchoObjectCode obj_code)
{
  return ((obj_code >> 8) & 0xFFFF);
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

bool uecho_integer2byte(int val, byte* bytes, size_t byte_size)
{
  size_t n, idx;

  if (!bytes)
    return false;

  for (n = 0; n < byte_size; n++) {
    idx = ((byte_size - 1) - n);
    bytes[idx] = (val >> (n * 8)) & 0xFF;
  }

  return true;
}

/****************************************
 * uecho_byte2integer
 ****************************************/

int uecho_byte2integer(byte* bytes, size_t byte_size)
{
  int val = 0;
  size_t n, idx;

  if (!bytes)
    return 0;

  for (n = 0; n < byte_size; n++) {
    idx = ((byte_size - 1) - n);
    val += (bytes[idx] << (n * 8));
  }

  return val;
}
