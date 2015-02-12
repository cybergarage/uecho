/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <uecho/net/interface.h>
#include <string.h>

/****************************************
* uecho_net_interface_new
****************************************/

mUpnpNetworkInterface *uecho_net_interface_new()
{
	mUpnpNetworkInterface *netIf;

	netIf = (mUpnpNetworkInterface *)malloc(sizeof(mUpnpNetworkInterface));

	if ( NULL != netIf )
	{
		uecho_list_node_init((mUpnpList *)netIf);
		netIf->name = uecho_string_new();
		netIf->ipaddr = uecho_string_new();
		netIf->netmask = uecho_string_new();
		uecho_net_interface_setindex(netIf, 0);
		memset(netIf->macaddr, 0, (size_t)CG_NET_MACADDR_SIZE);
	}

	return netIf;
}

/****************************************
* uecho_net_interface_delete
****************************************/

void uecho_net_interface_delete(mUpnpNetworkInterface *netIf)
{
	uecho_list_remove((mUpnpList *)netIf);
	uecho_string_delete(netIf->name);
	uecho_string_delete(netIf->ipaddr);
	uecho_string_delete(netIf->netmask);
	free(netIf);
}

/****************************************
* uecho_net_interface_getany
****************************************/

mUpnpNetworkInterface *uecho_net_interface_getany()
{
	mUpnpNetworkInterface *netIf;

	netIf = uecho_net_interface_new();
	uecho_net_interface_setname(netIf, "INADDR_ANY");
	uecho_net_interface_setaddress(netIf, "0.0.0.0");
	
	return netIf;
}

/****************************************
* uecho_net_interface_setname
****************************************/

void uecho_net_interface_setname(mUpnpNetworkInterface *netIf, char *name)
{
	uecho_string_setvalue(netIf->name, name);
}

/****************************************
* uecho_net_interface_getname
****************************************/

char *uecho_net_interface_getname(mUpnpNetworkInterface *netIf)
{
	return uecho_string_getvalue(netIf->name);
}

/****************************************
* uecho_net_interface_setaddress
****************************************/

void uecho_net_interface_setaddress(mUpnpNetworkInterface *netIf, char *value)
{
	uecho_string_setvalue(netIf->ipaddr, value);
}

/****************************************
* uecho_net_interface_getaddress
****************************************/

char *uecho_net_interface_getaddress(mUpnpNetworkInterface *netIf)
{
	return uecho_string_getvalue(netIf->ipaddr);
}

/****************************************
* uecho_net_interface_setnetmask
****************************************/

void uecho_net_interface_setnetmask(mUpnpNetworkInterface *netIf, char *value)
{
	uecho_string_setvalue(netIf->netmask, value);
}

/****************************************
* uecho_net_interface_getnetmask
****************************************/

char *uecho_net_interface_getnetmask(mUpnpNetworkInterface *netIf)
{
	return uecho_string_getvalue(netIf->netmask);
}

/****************************************
* uecho_net_interface_cmp
****************************************/

int uecho_net_interface_cmp(mUpnpNetworkInterface *netIfA, 
			 mUpnpNetworkInterface *netIfB)
{
	if (netIfA == NULL && netIfB == NULL) return 0;
	if (netIfA == NULL && netIfB != NULL) return 1;
	if (netIfA != NULL && netIfB == NULL) return -1;
	
	return uecho_strcmp(uecho_net_interface_getaddress(netIfA), 
			 uecho_net_interface_getaddress(netIfB));
}
