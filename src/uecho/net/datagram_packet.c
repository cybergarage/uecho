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

uEchoDatagramPacket *uecho_socket_datagram_packet_new(void)
{
	uEchoDatagramPacket *dgmPkt;

	dgmPkt = (uEchoDatagramPacket *)malloc(sizeof(uEchoDatagramPacket));

	if (!dgmPkt)
    return NULL;

  dgmPkt->data = uecho_string_new();
  dgmPkt->localAddress = uecho_string_new();
  dgmPkt->remoteAddress = uecho_string_new();

  uecho_socket_datagram_packet_setlocalport(dgmPkt, 0);
  uecho_socket_datagram_packet_setremoteport(dgmPkt, 0);
	
	return dgmPkt;
}

/****************************************
* uecho_socket_datagram_packet_delete
****************************************/

void uecho_socket_datagram_packet_delete(uEchoDatagramPacket *dgmPkt)
{
	uecho_string_delete(dgmPkt->data);
	uecho_string_delete(dgmPkt->localAddress);
	uecho_string_delete(dgmPkt->remoteAddress);

	free(dgmPkt);
}

/****************************************
* uecho_socket_datagram_packet_copy
****************************************/

void uecho_socket_datagram_packet_copy(uEchoDatagramPacket *dstDgmPkt, uEchoDatagramPacket *srcDgmPkt)
{
	uecho_socket_datagram_packet_setdata(dstDgmPkt, uecho_socket_datagram_packet_getdata(srcDgmPkt));
	uecho_socket_datagram_packet_setlocaladdress(dstDgmPkt, uecho_socket_datagram_packet_getlocaladdress(srcDgmPkt));
	uecho_socket_datagram_packet_setlocalport(dstDgmPkt, uecho_socket_datagram_packet_getlocalport(srcDgmPkt));
	uecho_socket_datagram_packet_setremoteaddress(dstDgmPkt, uecho_socket_datagram_packet_getremoteaddress(srcDgmPkt));
	uecho_socket_datagram_packet_setremoteport(dstDgmPkt, uecho_socket_datagram_packet_getremoteport(srcDgmPkt));
}
