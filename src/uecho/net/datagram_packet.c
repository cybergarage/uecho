/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/net/socket.h>

/****************************************
 * uecho_socket_datagram_packet_new
 ****************************************/

uEchoDatagramPacket* uecho_socket_datagram_packet_new(void)
{
  uEchoDatagramPacket* dgm_pkt;

  dgm_pkt = (uEchoDatagramPacket*)malloc(sizeof(uEchoDatagramPacket));

  if (!dgm_pkt)
    return NULL;

  dgm_pkt->data = NULL;
  dgm_pkt->data_len = 0;

  dgm_pkt->local_address = uecho_string_new();
  uecho_socket_datagram_packet_setlocalport(dgm_pkt, 0);

  dgm_pkt->remote_address = uecho_string_new();
  uecho_socket_datagram_packet_setremoteport(dgm_pkt, 0);

  return dgm_pkt;
}

/****************************************
 * uecho_socket_datagram_packet_delete
 ****************************************/

void uecho_socket_datagram_packet_delete(uEchoDatagramPacket* dgm_pkt)
{
  if (!dgm_pkt)
    return;

  uecho_socket_datagram_packet_clear(dgm_pkt);

  uecho_string_delete(dgm_pkt->local_address);
  uecho_string_delete(dgm_pkt->remote_address);

  free(dgm_pkt);
}

/****************************************
 * uecho_socket_datagram_packet_setdata
 ****************************************/

bool uecho_socket_datagram_packet_setdata(uEchoDatagramPacket* dgm_pkt, const byte* data, size_t data_len)
{
  if (!dgm_pkt)
    return false;

  uecho_socket_datagram_packet_clear(dgm_pkt);

  if (!data || (data_len <= 0))
    return true;

  dgm_pkt->data = malloc(data_len);
  if (!dgm_pkt->data)
    return false;

  memcpy(dgm_pkt->data, data, data_len);
  dgm_pkt->data_len = data_len;

  return true;
}

/****************************************
 * uecho_socket_datagram_packet_clear
 ****************************************/

bool uecho_socket_datagram_packet_clear(uEchoDatagramPacket* dgm_pkt)
{
  if (!dgm_pkt)
    return false;

  if (dgm_pkt->data) {
    free(dgm_pkt->data);
    dgm_pkt->data = NULL;
  }
  dgm_pkt->data_len = 0;

  return true;
}

/****************************************
 * uecho_socket_datagram_packet_copy
 ****************************************/

bool uecho_socket_datagram_packet_copy(uEchoDatagramPacket* dst_dgm_pkt, uEchoDatagramPacket* src_dgm_pkt)
{
  if (!dst_dgm_pkt || !src_dgm_pkt)
    return false;

  uecho_socket_datagram_packet_setdata(dst_dgm_pkt, uecho_socket_datagram_packet_getdata(src_dgm_pkt), uecho_socket_datagram_packet_getlength(src_dgm_pkt));
  uecho_socket_datagram_packet_setlocaladdress(dst_dgm_pkt, uecho_socket_datagram_packet_getlocaladdress(src_dgm_pkt));
  uecho_socket_datagram_packet_setlocalport(dst_dgm_pkt, uecho_socket_datagram_packet_getlocalport(src_dgm_pkt));
  uecho_socket_datagram_packet_setremoteaddress(dst_dgm_pkt, uecho_socket_datagram_packet_getremoteaddress(src_dgm_pkt));
  uecho_socket_datagram_packet_setremoteport(dst_dgm_pkt, uecho_socket_datagram_packet_getremoteport(src_dgm_pkt));

  return true;
}
