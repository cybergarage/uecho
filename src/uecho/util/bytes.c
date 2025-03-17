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

int uecho_bytes_toint(byte* byteData, size_t byteSize)
{
  int idx;
  int n;
  int val = 0;

  for (n = 0; n < (int)byteSize; n++) {
    idx = ((int)byteSize - 1) - n;
    val += byteData[idx] << (n * 8);
  }

  return val;
}

/****************************************
 * uecho_bytes_fromint
 ****************************************/

byte* uecho_bytes_fromint(int val, size_t bytesSize)
{
  byte* bytesData;
  int idx;
  int n;

  bytesData = malloc(bytesSize);
  for (n = 0; n < (int)bytesSize; n++) {
    idx = ((int)bytesSize - 1) - n;
    bytesData[idx] = ((val >> (n * 8)) & 0xFF);
  }

  return bytesData;
}
