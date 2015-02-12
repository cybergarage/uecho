/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/list.h>
#include <uecho/net/interface.h>

/****************************************
* uecho_net_interfacelist_new
****************************************/

uEchoNetworkInterfaceList *uecho_net_interfacelist_new()
{
	uEchoNetworkInterfaceList *netIfList;

	netIfList = (uEchoNetworkInterfaceList *)malloc(sizeof(uEchoNetworkInterfaceList));

	if ( NULL != netIfList )
	{
		uecho_list_header_init((uEchoList *)netIfList);
		netIfList->name = NULL;
		netIfList->ipaddr = NULL;
	}

	return netIfList;
}

/****************************************
* uecho_net_interfacelist_delete
****************************************/

void uecho_net_interfacelist_delete(uEchoNetworkInterfaceList *netIfList)
{
	uecho_net_interfacelist_clear(netIfList);
	free(netIfList);
}

/****************************************
* uecho_net_interfacelist_getinterface
****************************************/

uEchoNetworkInterface *uecho_net_interfacelist_get(uEchoNetworkInterfaceList *netIfList, char *name)
{
	uEchoNetworkInterface *netIf;
	char *ifName;

	if (name == NULL)
		return NULL;
		
	for (netIf = uecho_net_interfacelist_gets(netIfList); netIf != NULL; netIf = uecho_net_interface_next(netIf)) {
		ifName = uecho_net_interface_getname(netIf);
		if (ifName == NULL)
			continue;
		if (uecho_strcasecmp(ifName, name) == 0)
			return netIf;
	}
	
	return NULL;
}

/****************************************
* uecho_net_interfacelist_getchanges
****************************************/

void uecho_net_interfacelist_getchanges(uEchoNetworkInterfaceList *netIfListOld,
				     uEchoNetworkInterfaceList *netIfListNew,
				     uEchoNetworkInterfaceList *netIfListAdded,
				     uEchoNetworkInterfaceList *netIfListRemoved)
{
	uEchoNetworkInterface *netIfOld, *netIfNew, *tmp;
	BOOL found;
	
	/* Browse through old interfaces and check, if they are in the new */
	tmp = uecho_net_interfacelist_gets(netIfListOld);
	while (tmp != NULL)
	{
		netIfOld = tmp; tmp = uecho_net_interface_next(netIfOld);
		
		found = FALSE;
		for (netIfNew = uecho_net_interfacelist_gets(netIfListNew); netIfNew != NULL;
		     netIfNew = uecho_net_interface_next(netIfNew))
		{
			if (uecho_net_interface_cmp(netIfOld, netIfNew) == 0)
			{
				found = TRUE;
				break;
			}
		}
		
		/* Old interface was not found in new ones, so it's removed */
		if (found == FALSE)
		{
			uecho_net_interface_remove(netIfOld);
			if (netIfListRemoved != NULL)
				uecho_net_interfacelist_add(netIfListRemoved, 
							 netIfOld);
		}
	}
	
	/* Browse through new interfaces and check, if they are in the 
	   remaining old interfaces */
	tmp = uecho_net_interfacelist_gets(netIfListNew);
	while (tmp != NULL)
	{
		netIfNew = tmp; tmp = uecho_net_interface_next(netIfNew);
		
		found = FALSE;
		for (netIfOld = uecho_net_interfacelist_gets(netIfListOld); netIfOld != NULL;
		     netIfOld = uecho_net_interface_next(netIfOld))
		{
			if (uecho_net_interface_cmp(netIfOld, netIfNew) == 0)
			{
				found = TRUE;
				break;
			}
		}
		
		/* New interface was not found in old ones, so it's added */
		if (found == FALSE)
		{
			uecho_net_interface_remove(netIfNew);
			if (netIfListAdded != NULL)
				uecho_net_interfacelist_add(netIfListAdded, 
							 netIfNew);
		}
	}
}
