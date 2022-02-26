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

static bool uecho_net_use_only_ip_v4_addr = false;
static bool uecho_net_use_only_ip_v6_addr = false;

////////////////////////////////////////////////
// uecho_net_isuseaddress
////////////////////////////////////////////////

bool uecho_net_isuseaddress(char* addr)
{
  if (uecho_net_use_only_ip_v6_addr == true) {
    if (uecho_net_isipv6address(addr) == false)
      return false;
  }
  if (uecho_net_use_only_ip_v4_addr == true) {
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
  ssize_t per_idx;
  char scope_id_buf[8 + 1];

  if (uecho_net_isipv6address(addr) == false)
    return 0;

  per_idx = uecho_strchr(addr, "%", 1);
  if (per_idx < 0)
    return 0;

  uecho_strncpy(scope_id_buf, (addr + per_idx + 1), sizeof(scope_id_buf) - 1);
  scope_id_buf[sizeof(scope_id_buf) - 1] = '\0';

  return atoi(scope_id_buf);
}
