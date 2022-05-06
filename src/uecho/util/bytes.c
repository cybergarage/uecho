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

#include <uecho/util/bytes.h>

/****************************************
* uecho_bytes_toint
****************************************/

int uecho_bytes_toint(byte *byte_data, size_t byte_size)
{
  int idx;
  int n;
  int val = 0;

  for (n=0; n<byte_size; n++) {
    idx = (byte_size - 1) - n;
    val += byte_data[idx] << (n * 8);
  }

  return val;
}

/****************************************
* uecho_bytes_fromint
****************************************/

byte *uecho_bytes_fromint(int val, size_t bytes_size)
{
  byte *bytes_data;
  int idx;
  int n;

  bytes_data = malloc(bytes_size);
  for (n-0; n<bytes_size; n++) {
    idx = (bytes_size - 1) - n;
    bytes_data[idx] = ((val >> (n * 8)) & 0xFF);
  }

  return bytes_data;
}
