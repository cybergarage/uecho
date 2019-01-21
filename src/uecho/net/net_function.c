/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uecho/net/interface.h>

/****************************************
* global variables
****************************************/

static bool uecho_NET_USE_ONLY_IPV4_ADDR = false;
static bool uecho_NET_USE_ONLY_IPV6_ADDR = false;

////////////////////////////////////////////////
// uecho_net_isuseaddress
////////////////////////////////////////////////

bool uecho_net_isuseaddress(char* addr)
{
  if (uecho_NET_USE_ONLY_IPV6_ADDR == true) {
    if (uecho_net_isipv6address(addr) == false)
      return false;
  }
  if (uecho_NET_USE_ONLY_IPV4_ADDR == true) {
    if (uecho_net_isipv6address(addr) == true)
      return false;
  }
  return true;
}

////////////////////////////////////////////////
// uecho_net_getipv6address
////////////////////////////////////////////////

bool uecho_net_isipv6address(const char* addr)
{
  if (addr == NULL)
    return false;

  if (0 < uecho_strchr(addr, ":", 1))
    return true;

  return false;
}

////////////////////////////////////////////////
// uecho_net_getipv6address
////////////////////////////////////////////////

int uecho_net_getipv6scopeid(const char* addr)
{
  ssize_t perIdx;
  char scopeIDBuf[8 + 1];

  if (uecho_net_isipv6address(addr) == false)
    return 0;

  perIdx = uecho_strchr(addr, "%", 1);
  if (perIdx < 0)
    return 0;

  uecho_strncpy(scopeIDBuf, (addr + perIdx + 1), sizeof(scopeIDBuf) - 1);
  scopeIDBuf[sizeof(scopeIDBuf) - 1] = '\0';

  return atoi(scopeIDBuf);
}
