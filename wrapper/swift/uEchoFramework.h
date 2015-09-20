/******************************************************************
 *
 * uEcho for Swift
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_FRAMEWORK_H_
#define _UECHO_FRAMEWORK_H_

#include <uecho/typedef.h>

#ifdef  __cplusplus
extern "C" {
#endif

void uEchoFrameworkControllerMessageListener(uEchoController *ctrl, uEchoMessage *msg);

#ifdef  __cplusplus
}
#endif

#endif
