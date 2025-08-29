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
#include <uecho/net/socket.h>
#include <uecho/util/strings.h>

#include "esp_log.h"
#include "esp_netif.h"

static const char* TAG = "uecho_esp32";

size_t uecho_net_gethostinterfaces(uEchoNetworkInterfaceList* netIfList)
{
  uecho_net_interfacelist_clear(netIfList);

  esp_netif_t* netif = NULL;
  for (int i = 0; i < esp_netif_get_nr_of_ifs(); ++i) {
    netif = esp_netif_next(netif);
    if (!netif) {
      break;
    }

    esp_netif_ip_info_t ip_info;
    if (esp_netif_get_ip_info(netif, &ip_info) != ESP_OK) {
      continue;
    }

    // Ignore loopback and down interfaces
    if (esp_netif_is_netif_up(netif) == false || (ip_info.ip.addr == IPADDR_LOOPBACK)) {
      continue;
    }

    uEchoNetworkInterface* netIf = uecho_net_interface_new();
    if (!netIf) {
      continue;
    }

    char ip_str[IP4ADDR_STRLEN_MAX];
    char netmask_str[IP4ADDR_STRLEN_MAX];

    ip4addr_ntoa_r(&ip_info.ip, ip_str, sizeof(ip_str));
    ip4addr_ntoa_r(&ip_info.netmask, netmask_str, sizeof(netmask_str));

    uecho_net_interface_setaddress(netIf, ip_str);
    uecho_net_interface_setnetmask(netIf, netmask_str);

    const char* if_key = esp_netif_get_ifkey(netif);
    if (if_key) {
      uecho_net_interface_setname(netIf, (char*)if_key);
    }

    uint8_t mac[6];
    if (esp_netif_get_mac(netif, mac) == ESP_OK) {
      uecho_net_interface_setmacaddress(netIf, (const char*)mac);
    }

    uecho_net_interfacelist_add(netIfList, netIf);
  }

  return uecho_net_interfacelist_size(netIfList);
}

char* uecho_net_selectaddr(struct sockaddr* remoteaddr)
{
  // For now, we just return the first available interface address.
  // A more sophisticated implementation could select the best interface
  // based on the remote address.
  uEchoNetworkInterfaceList* netIfList = uecho_net_interfacelist_new();
  if (!netIfList) {
    return uecho_strdup("127.0.0.1");
  }

  if (uecho_net_gethostinterfaces(netIfList) <= 0) {
    uecho_net_interfacelist_delete(netIfList);
    return uecho_strdup("127.0.0.1");
  }

  uEchoNetworkInterface* netIf = uecho_net_interfacelist_gets(netIfList);
  if (!netIf) {
    uecho_net_interfacelist_delete(netIfList);
    return uecho_strdup("127.0.0.1");
  }

  char* addr = uecho_strdup(uecho_net_interface_getaddress(netIf));
  uecho_net_interfacelist_delete(netIfList);
  return addr;
}
