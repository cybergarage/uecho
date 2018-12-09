/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/net/interface.h>
#include <uecho/util/list.h>

/****************************************
* uecho_net_interfacelist_new
****************************************/

uEchoNetworkInterfaceList* uecho_net_interfacelist_new(void)
{
  uEchoNetworkInterfaceList* netIfList;

  netIfList = (uEchoNetworkInterfaceList*)malloc(sizeof(uEchoNetworkInterfaceList));
  if (!netIfList)
    return NULL;

  uecho_list_header_init((uEchoList*)netIfList);
  netIfList->name = NULL;
  netIfList->ipaddr = NULL;

  return netIfList;
}

/****************************************
* uecho_net_interfacelist_delete
****************************************/

void uecho_net_interfacelist_delete(uEchoNetworkInterfaceList* netIfList)
{
  uecho_net_interfacelist_clear(netIfList);
  free(netIfList);
}
