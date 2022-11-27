/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
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
  uEchoNetworkInterfaceList* net_if_list;

  net_if_list = (uEchoNetworkInterfaceList*)malloc(sizeof(uEchoNetworkInterfaceList));
  if (!net_if_list)
    return NULL;

  uecho_list_header_init((uEchoList*)net_if_list);
  net_if_list->name = NULL;
  net_if_list->ipaddr = NULL;

  return net_if_list;
}

/****************************************
 * uecho_net_interfacelist_delete
 ****************************************/

void uecho_net_interfacelist_delete(uEchoNetworkInterfaceList* net_if_list)
{
  uecho_net_interfacelist_clear(net_if_list);
  free(net_if_list);
}
