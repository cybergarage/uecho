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
* uecho_classcode_to_classcode
****************************************/

uEchoClassCode uecho_classcode_to_classcode(uEchoObjectCode objCode)
{
  return ((objCode >> 8) & 0xFFFF);
}
