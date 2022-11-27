/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_NET_CSOCKET_H_
#define _UECHO_NET_CSOCKET_H_

#if defined(UECHO_USE_OPENSSL)
#include <openssl/ssl.h>
#endif

#include <uecho/net/socket_opt.h>
#include <uecho/typedef.h>
#include <uecho/util/strings.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define UECHO_NET_SOCKET_NONE 0

#define UECHO_NET_SOCKET_STREAM 0x01
#define UECHO_NET_SOCKET_DGRAM 0x02

#define UECHO_NET_SOCKET_CLIENT 1
#define UECHO_NET_SOCKET_SERVER 2

#define UECHO_NET_SOCKET_MAXHOST 32
#define UECHO_NET_SOCKET_MAXSERV 32

#if !defined(WIN32)
typedef int SOCKET;
#endif

#define UECHO_SOCKET_LF '\n'

#define UECHO_NET_SOCKET_DGRAM_RECV_BUFSIZE 512
#define UECHO_NET_SOCKET_DGRAM_ANCILLARY_BUFSIZE 512
#define UECHO_NET_SOCKET_MULTICAST_DEFAULT_TTL 4
#define UECHO_NET_SOCKET_AUTO_IP_NET 0xa9fe0000
#define UECHO_NET_SOCKET_AUTO_IP_MASK 0xffff0000

/****************************************
 * Data Type
 ****************************************/

#include <uecho/util/list.h>

typedef struct UEchoSocket {
  SOCKET id;
  int type;
  int direction;
  uEchoString* ipaddr;
  int port;
#if defined(UECHO_USE_OPENSSL)
  SSL_CTX* ctx;
  SSL* ssl;
#endif
} uEchoSocket;

typedef struct UEchoDatagramPacket {
  byte* data;
  size_t data_len;

  uEchoString* local_address;
  int local_port;

  uEchoString* remote_address;
  int remote_port;
} uEchoDatagramPacket;

/****************************************
 * Function (Socket)
 ****************************************/

void uecho_socket_startup(void);
void uecho_socket_cleanup(void);

uEchoSocket* uecho_socket_new(int type);
#define uecho_socket_stream_new() uecho_socket_new(UECHO_NET_SOCKET_STREAM)
#define uecho_socket_dgram_new() uecho_socket_new(UECHO_NET_SOCKET_DGRAM)
bool uecho_socket_delete(uEchoSocket* socket);

void uecho_socket_setid(uEchoSocket* socket, SOCKET value);
#define uecho_socket_getid(socket) (socket->id)

#define uecho_socket_settype(socket, value) (socket->type = value)
#define uecho_socket_gettype(socket) (socket->type)
#define uecho_socket_issocketstream(socket) ((socket->type & UECHO_NET_SOCKET_STREAM) ? true : false)
#define uecho_socket_isdatagramstream(socket) ((socket->type & UECHO_NET_SOCKET_DGRAM) ? true : false)

#define uecho_socket_setdirection(socket, value) (socket->direction = value)
#define uecho_socket_getdirection(socket) (socket->direction)
#define uecho_socket_isclient(socket) ((socket->direction == UECHO_NET_SOCKET_CLIENT) ? true : false)
#define uecho_socket_isserver(socket) ((socket->direction == UECHO_NET_SOCKET_SERVER) ? true : false)

#define uecho_socket_setaddress(socket, value) uecho_string_setvalue(socket->ipaddr, value)
#define uecho_socket_getaddress(socket) uecho_string_getvalue(socket->ipaddr)
bool uecho_socket_isboundaddress(uEchoSocket* socket, const char* addr);

#define uecho_socket_setport(socket, value) (socket->port = value)
#define uecho_socket_getport(socket) (socket->port)

bool uecho_socket_isbound(uEchoSocket* socket);
bool uecho_socket_close(uEchoSocket* socket);

bool uecho_socket_listen(uEchoSocket* socket);

bool uecho_socket_bind(uEchoSocket* sock, int bind_port, const char* bind_addr, uEchoSocketOption* opt);
bool uecho_socket_accept(uEchoSocket* sock, uEchoSocket* client_sock);
bool uecho_socket_connect(uEchoSocket* sock, const char* addr, int port);
ssize_t uecho_socket_read(uEchoSocket* sock, char* buffer, size_t buffer_len);
size_t uecho_socket_write(uEchoSocket* sock, const char* buffer, size_t buffer_len);
ssize_t uecho_socket_readline(uEchoSocket* sock, char* buffer, size_t buffer_len);
size_t uecho_socket_skip(uEchoSocket* sock, size_t skip_len);

size_t uecho_socket_sendto(uEchoSocket* sock, const char* addr, int port, const byte* data, size_t datae_len);
ssize_t uecho_socket_recv(uEchoSocket* sock, uEchoDatagramPacket* dgm_pkt);

/****************************************
 * Function (Multicast)
 ****************************************/

bool uecho_socket_joingroup(uEchoSocket* sock, const char* mcast_addr, const char* if_addr);

/****************************************
 * Function (Option)
 ****************************************/

bool uecho_socket_setreuseaddress(uEchoSocket* socket, bool flag);
bool uecho_socket_setmulticastloop(uEchoSocket* sock, bool flag);
bool uecho_socket_setmulticastttl(uEchoSocket* sock, int ttl);
bool uecho_socket_settimeout(uEchoSocket* sock, int sec);

/****************************************
 * Function (DatagramPacket)
 ****************************************/

uEchoDatagramPacket* uecho_socket_datagram_packet_new(void);
void uecho_socket_datagram_packet_delete(uEchoDatagramPacket* dgm_pkt);
bool uecho_socket_datagram_packet_setdata(uEchoDatagramPacket* dgm_pkt, const byte* data, size_t data_len);
bool uecho_socket_datagram_packet_clear(uEchoDatagramPacket* dgm_pkt);

#define uecho_socket_datagram_packet_getdata(dgmPkt) (dgmPkt->data)
#define uecho_socket_datagram_packet_getlength(dgmPkt) (dgmPkt->data_len)

#define uecho_socket_datagram_packet_setlocaladdress(dgmPkt, addr) uecho_string_setvalue(dgmPkt->local_address, addr)
#define uecho_socket_datagram_packet_getlocaladdress(dgmPkt) uecho_string_getvalue(dgmPkt->local_address)
#define uecho_socket_datagram_packet_setlocalport(dgmPkt, port) (dgmPkt->local_port = port)
#define uecho_socket_datagram_packet_getlocalport(dgmPkt) (dgmPkt->local_port)
#define uecho_socket_datagram_packet_setremoteaddress(dgmPkt, addr) uecho_string_setvalue(dgmPkt->remote_address, addr)
#define uecho_socket_datagram_packet_getremoteaddress(dgmPkt) uecho_string_getvalue(dgmPkt->remote_address)
#define uecho_socket_datagram_packet_setremoteport(dgmPkt, port) (dgmPkt->remote_port = port)
#define uecho_socket_datagram_packet_getremoteport(dgmPkt) (dgmPkt->remote_port)

bool uecho_socket_datagram_packet_copy(uEchoDatagramPacket* dst_dgm_pkt, uEchoDatagramPacket* src_dgm_pkt);

/****************************************
 * Function (SSLSocket)
 ****************************************/

#if defined(UECHO_USE_OPENSSL)
#define UECHO_NET_SOCKET_SSL 0x0100
#define uecho_socket_ssl_new() uecho_socket_new(UECHO_NET_SOCKET_STREAM | UECHO_NET_SOCKET_SSL)
#define uecho_socket_isssl(socket) ((socket->type & UECHO_NET_SOCKET_SSL) ? true : false)
#endif

#ifdef __cplusplus
}
#endif

#endif
