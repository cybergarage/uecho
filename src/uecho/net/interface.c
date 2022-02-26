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

#include <string.h>
#include <uecho/net/interface.h>

/****************************************
* uecho_net_interface_new
****************************************/

uEchoNetworkInterface* uecho_net_interface_new(void)
{
  uEchoNetworkInterface* net_if;

  net_if = (uEchoNetworkInterface*)malloc(sizeof(uEchoNetworkInterface));

  if (!net_if)
    return NULL;

  uecho_list_node_init((uEchoList*)net_if);
  net_if->name = uecho_string_new();
  net_if->ipaddr = uecho_string_new();
  net_if->netmask = uecho_string_new();
  uecho_net_interface_setindex(net_if, 0);
  memset(net_if->macaddr, 0, (size_t)UECHO_NET_MACADDR_SIZE);

  return net_if;
}

/****************************************
* uecho_net_interface_delete
****************************************/

void uecho_net_interface_delete(uEchoNetworkInterface* net_if)
{
  if (!net_if)
    return;

  uecho_list_remove((uEchoList*)net_if);
  uecho_string_delete(net_if->name);
  uecho_string_delete(net_if->ipaddr);
  uecho_string_delete(net_if->netmask);
  free(net_if);
}

/****************************************
* uecho_net_interface_getany
****************************************/

uEchoNetworkInterface* uecho_net_interface_getany(void)
{
  uEchoNetworkInterface* net_if;

  net_if = uecho_net_interface_new();
  uecho_net_interface_setname(net_if, "INADDR_ANY");
  uecho_net_interface_setaddress(net_if, "0.0.0.0");

  return net_if;
}

/****************************************
* uecho_net_interface_setname
****************************************/

void uecho_net_interface_setname(uEchoNetworkInterface* net_if, char* name)
{
  if (!net_if)
    return;

  uecho_string_setvalue(net_if->name, name);
}

/****************************************
* uecho_net_interface_getname
****************************************/

char* uecho_net_interface_getname(uEchoNetworkInterface* net_if)
{
  if (!net_if)
    return NULL;

  return uecho_string_getvalue(net_if->name);
}

/****************************************
* uecho_net_interface_setaddress
****************************************/

void uecho_net_interface_setaddress(uEchoNetworkInterface* net_if, char* value)
{
  if (!net_if)
    return;

  uecho_string_setvalue(net_if->ipaddr, value);
}

/****************************************
* uecho_net_interface_getaddress
****************************************/

char* uecho_net_interface_getaddress(uEchoNetworkInterface* net_if)
{
  if (!net_if)
    return NULL;

  return uecho_string_getvalue(net_if->ipaddr);
}

/****************************************
* uecho_net_interface_setnetmask
****************************************/

void uecho_net_interface_setnetmask(uEchoNetworkInterface* net_if, char* value)
{
  if (!net_if)
    return;

  uecho_string_setvalue(net_if->netmask, value);
}

/****************************************
* uecho_net_interface_getnetmask
****************************************/

char* uecho_net_interface_getnetmask(uEchoNetworkInterface* net_if)
{
  if (!net_if)
    return NULL;

  return uecho_string_getvalue(net_if->netmask);
}
