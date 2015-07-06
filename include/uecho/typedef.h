/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_TYPEDEF_H_
#define _UECHO_TYPEDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(WINCE) && !defined(WIN32)
#define WIN32 1
#endif

#if (defined(WIN32) || defined(__CYGWIN__)) && !defined (ITRON)
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(BTRON) || defined(TENGINE)
#include <typedef.h>
#elif defined(ITRON)
#include <kernel.h>
#endif

#if defined(__APPLE_CPP__) || defined(__APPLE_CC__)
#include <TargetConditionals.h>
#endif
	
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Data Type
****************************************/

#if !defined(bool) && !defined(BTRON) && !defined(TENGINE) && !defined(__OBJC__)
typedef int bool;
#endif

#if !defined(true)
#if defined(__OBJC__)
#define true YES
#else
#define true (1)
#endif
#endif

#if !defined(false)
#if defined(__OBJC__)
#define false NO
#else
#define false (0)
#endif
#endif
	
typedef unsigned char uEchoByte;

#ifdef  __cplusplus
}
#endif

#endif
