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

uEchoObject* uecho_light_new(void);
bool uecho_light_delete(uEchoObject* obj);
void uecho_light_printrequest(uEchoMessage* msg);
