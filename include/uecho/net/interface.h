/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
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

#define UECHO_NET_IPV4_ADDRSTRING_MAXSIZE ((3*4)+(1*3)+1)
#define UECHO_NET_IPV6_ADDRSTRING_MAXSIZE (1+(8*4)+(1*7)+1+1)

#define UECHO_NET_IPV4_LOOPBACK "127.0.0.1"
#define UECHO_NET_IPV6_LOOPBACK "fixmelater"
#define UECHO_NET_MACADDR_SIZE 6

/****************************************
* Data Type
****************************************/

typedef struct _uEchoNetworkInterface {
	bool headFlag;
	struct _uEchoNetworkInterface *prev;
	struct _uEchoNetworkInterface *next;
	uEchoString *name;
	uEchoString *ipaddr;
	uEchoString *netmask;
	byte macaddr[UECHO_NET_MACADDR_SIZE];
	int index;
} uEchoNetworkInterface, uEchoNetworkInterfaceList;

/****************************************
* Function (NetworkInterface)
****************************************/

uEchoNetworkInterface *uecho_net_interface_new();
void uecho_net_interface_delete(uEchoNetworkInterface *netIf);
uEchoNetworkInterface* uecho_net_interface_getany();

#define uecho_net_interface_next(netIf) (uEchoNetworkInterface *)uecho_list_next((uEchoList *)netIf)
#define uecho_net_interface_remove(netIf) uecho_list_remove((uEchoList *)netIf)

void uecho_net_interface_setname(uEchoNetworkInterface *netIf, char *name);
char *uecho_net_interface_getname(uEchoNetworkInterface *netIf);
void uecho_net_interface_setaddress(uEchoNetworkInterface *netIf, char *ipaddr);
char *uecho_net_interface_getaddress(uEchoNetworkInterface *netIf);
void uecho_net_interface_setnetmask(uEchoNetworkInterface *netIf, char *ipaddr);
char *uecho_net_interface_getnetmask(uEchoNetworkInterface *netIf);
char *uecho_net_selectaddr(struct sockaddr *remoteaddr);

#define uecho_net_interface_setmacaddress(netIf, value) memcpy(netIf->macaddr, value, UECHO_NET_MACADDR_SIZE)
#define uecho_net_interface_getmacaddress(netIf, buf) memcpy(buf, netIf->macaddr, UECHO_NET_MACADDR_SIZE)

#define uecho_net_interface_setindex(netIf, value) (netIf->index = value)
#define uecho_net_interface_getindex(netIf, buf) (netIf->index)

int uecho_net_interface_cmp(uEchoNetworkInterface *netIfA, uEchoNetworkInterface *netIfB);

/****************************************
* Function (NetworkInterfaceList)
****************************************/

uEchoNetworkInterfaceList *uecho_net_interfacelist_new();
void uecho_net_interfacelist_delete(uEchoNetworkInterfaceList *netIfList);

#define uecho_net_interfacelist_clear(netIfList) uecho_list_clear((uEchoList *)netIfList, (UECHO_LIST_DESTRUCTORFUNC)uecho_net_interface_delete)
#define uecho_net_interfacelist_size(netIfList) uecho_list_size((uEchoList *)netIfList)
#define uecho_net_interfacelist_gets(netIfList) (uEchoNetworkInterface *)uecho_list_next((uEchoList *)netIfList)
#define uecho_net_interfacelist_add(netIfList,netIf) uecho_list_add((uEchoList *)netIfList, (uEchoList *)netIf)

/****************************************
* Function
****************************************/

int uecho_net_gethostinterfaces(uEchoNetworkInterfaceList *netIfList);

bool uecho_net_isipv6address(const char *addr);
int uecho_net_getipv6scopeid(const char *addr);

#ifdef  __cplusplus
}
#endif

#endif

