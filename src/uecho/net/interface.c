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
  uEchoNetworkInterface* netIf;

  netIf = (uEchoNetworkInterface*)malloc(sizeof(uEchoNetworkInterface));

  if (!netIf)
    return NULL;

  uecho_list_node_init((uEchoList*)netIf);
  netIf->name = uecho_string_new();
  netIf->ipaddr = uecho_string_new();
  netIf->netmask = uecho_string_new();
  uecho_net_interface_setindex(netIf, 0);
  memset(netIf->macaddr, 0, (size_t)UECHO_NET_MACADDR_SIZE);

  return netIf;
}

/****************************************
 * uecho_net_interface_delete
 ****************************************/

void uecho_net_interface_delete(uEchoNetworkInterface* netIf)
{
  if (!netIf)
    return;

  uecho_list_remove((uEchoList*)netIf);
  uecho_string_delete(netIf->name);
  uecho_string_delete(netIf->ipaddr);
  uecho_string_delete(netIf->netmask);
  free(netIf);
}

/****************************************
 * uecho_net_interface_getany
 ****************************************/

uEchoNetworkInterface* uecho_net_interface_getany(void)
{
  uEchoNetworkInterface* netIf;

  netIf = uecho_net_interface_new();
  uecho_net_interface_setname(netIf, "INADDR_ANY");
  uecho_net_interface_setaddress(netIf, "0.0.0.0");

  return netIf;
}

/****************************************
 * uecho_net_interface_setname
 ****************************************/

void uecho_net_interface_setname(uEchoNetworkInterface* netIf, char* name)
{
  if (!netIf)
    return;

  uecho_string_setvalue(netIf->name, name);
}

/****************************************
 * uecho_net_interface_getname
 ****************************************/

char* uecho_net_interface_getname(uEchoNetworkInterface* netIf)
{
  if (!netIf)
    return NULL;

  return uecho_string_getvalue(netIf->name);
}

/****************************************
 * uecho_net_interface_setaddress
 ****************************************/

void uecho_net_interface_setaddress(uEchoNetworkInterface* netIf, char* value)
{
  if (!netIf)
    return;

  uecho_string_setvalue(netIf->ipaddr, value);
}

/****************************************
 * uecho_net_interface_getaddress
 ****************************************/

char* uecho_net_interface_getaddress(uEchoNetworkInterface* netIf)
{
  if (!netIf)
    return NULL;

  return uecho_string_getvalue(netIf->ipaddr);
}

/****************************************
 * uecho_net_interface_setnetmask
 ****************************************/

void uecho_net_interface_setnetmask(uEchoNetworkInterface* netIf, char* value)
{
  if (!netIf)
    return;

  uecho_string_setvalue(netIf->netmask, value);
}

/****************************************
 * uecho_net_interface_getnetmask
 ****************************************/

char* uecho_net_interface_getnetmask(uEchoNetworkInterface* netIf)
{
  if (!netIf)
    return NULL;

  return uecho_string_getvalue(netIf->netmask);
}
