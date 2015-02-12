/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_NET_CINTERFACE_H_
#define _UECHO_NET_CINTERFACE_H_

#include <uecho/typedef.h>
#include <uecho/util/string.h>
#include <uecho/util/list.h>

#if !defined(WIN32)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_NET_IPV4_ADDRSTRING_MAXSIZE ((3*4)+(1*3)+1)
#define CG_NET_IPV6_ADDRSTRING_MAXSIZE (1+(8*4)+(1*7)+1+1)

#define CG_NET_IPV4_LOOPBACK "127.0.0.1"
#define CG_NET_IPV6_LOOPBACK "fixmelater"
#define CG_NET_MACADDR_SIZE 6

#if defined(BTRON) || defined(TENGINE)
#define CG_NET_DEFAULT_IFNAME "Neta"
#endif

/****************************************
* Data Type
****************************************/

typedef struct _uEchoNetworkInterface {
	BOOL headFlag;
	struct _uEchoNetworkInterface *prev;
	struct _uEchoNetworkInterface *next;
	mUpnpString *name;
	mUpnpString *ipaddr;
	mUpnpString *netmask;
	mUpnpByte macaddr[CG_NET_MACADDR_SIZE];
	int index;
} mUpnpNetworkInterface, mUpnpNetworkInterfaceList;

/****************************************
* Function (NetworkInterface)
****************************************/

mUpnpNetworkInterface *uecho_net_interface_new();
void uecho_net_interface_delete(mUpnpNetworkInterface *netIf);
mUpnpNetworkInterface* uecho_net_interface_getany();

#define uecho_net_interface_next(netIf) (mUpnpNetworkInterface *)uecho_list_next((mUpnpList *)netIf)
#define uecho_net_interface_remove(netIf) uecho_list_remove((mUpnpList *)netIf)

void uecho_net_interface_setname(mUpnpNetworkInterface *netIf, char *name);
char *uecho_net_interface_getname(mUpnpNetworkInterface *netIf);
void uecho_net_interface_setaddress(mUpnpNetworkInterface *netIf, char *ipaddr);
char *uecho_net_interface_getaddress(mUpnpNetworkInterface *netIf);
void uecho_net_interface_setnetmask(mUpnpNetworkInterface *netIf, char *ipaddr);
char *uecho_net_interface_getnetmask(mUpnpNetworkInterface *netIf);
char *uecho_net_selectaddr(struct sockaddr *remoteaddr);

#define uecho_net_interface_setmacaddress(netIf, value) memcpy(netIf->macaddr, value, CG_NET_MACADDR_SIZE)
#define uecho_net_interface_getmacaddress(netIf, buf) memcpy(buf, netIf->macaddr, CG_NET_MACADDR_SIZE)

#define uecho_net_interface_setindex(netIf, value) (netIf->index = value)
#define uecho_net_interface_getindex(netIf, buf) (netIf->index)

/**
 * Compares two interfaces based on IP-address.
 */
int uecho_net_interface_cmp(mUpnpNetworkInterface *netIfA, 
			 mUpnpNetworkInterface *netIfB);

/****************************************
* Function (NetworkInterfaceList)
****************************************/

mUpnpNetworkInterfaceList *uecho_net_interfacelist_new();
void uecho_net_interfacelist_delete(mUpnpNetworkInterfaceList *netIfList);

#define uecho_net_interfacelist_clear(netIfList) uecho_list_clear((mUpnpList *)netIfList, (CG_LIST_DESTRUCTORFUNC)uecho_net_interface_delete)
#define uecho_net_interfacelist_size(netIfList) uecho_list_size((mUpnpList *)netIfList)
#define uecho_net_interfacelist_gets(netIfList) (mUpnpNetworkInterface *)uecho_list_next((mUpnpList *)netIfList)
#define uecho_net_interfacelist_add(netIfList,netIf) uecho_list_add((mUpnpList *)netIfList, (mUpnpList *)netIf)

mUpnpNetworkInterface *uecho_net_interfacelist_get(mUpnpNetworkInterfaceList *netIfList, char *name);

/**
 * Gets changes in the two given (aka old and new) interface lists. Changes
 * are resolved based on IP-addresses.
 * 
 * @param netIfListOld List of the old interfaces. It is changed!
 * @param netIfListNew List of the new interfaces. It is changed!
 * @param netIfListAdded List to store interfaces, which were in netIfListNew,
 *			 but were not in netIfListOld. May be NULL.
 * @param netIfListRemoved List to store interfaces, which were in 
 *			   netIfListOld, but were not in netIfListNew.
 */
void uecho_net_interfacelist_getchanges(mUpnpNetworkInterfaceList *netIfListOld,
				     mUpnpNetworkInterfaceList *netIfListNew,
				     mUpnpNetworkInterfaceList *netIfListAdded,
				     mUpnpNetworkInterfaceList *netIfListRemoved);

/****************************************
* Function
****************************************/

#if defined(ITRON)
void uecho_net_setinterface(const char *ifaddr);
#endif

int uecho_net_gethostinterfaces(mUpnpNetworkInterfaceList *netIfList);

BOOL uecho_net_isipv6address(const char *addr);
int uecho_net_getipv6scopeid(const char *addr);

#ifdef  __cplusplus
}
#endif

#endif

