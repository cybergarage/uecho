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

#include <uecho/typedef.h>
#include <uecho/util/string.h>

#if defined(CG_USE_OPENSSL)
#include <openssl/ssl.h>
#endif

#ifdef  __cplusplus
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

#if defined(BTRON) || defined(TENGINE)
typedef W SOCKET;
#elif defined(ITRON)
typedef ER SOCKET;
#elif !defined(WIN32) && !defined(__CYGWIN__)
typedef int SOCKET;
#endif

#define CG_SOCKET_LF '\n'

#define UECHO_NET_SOCKET_DGRAM_RECV_BUFSIZE 512
#define UECHO_NET_SOCKET_DGRAM_ANCILLARY_BUFSIZE 512
#define UECHO_NET_SOCKET_MULTICAST_DEFAULT_TTL 4
#define UECHO_NET_SOCKET_AUTO_IP_NET 0xa9fe0000
#define UECHO_NET_SOCKET_AUTO_IP_MASK 0xffff0000 

#if defined(ITRON)
#define UECHO_NET_SOCKET_WINDOW_BUFSIZE 4096
#endif

/****************************************
* Define (SocketList)
****************************************/

#if defined(ITRON)
#define UECHO_NET_USE_SOCKET_LIST 1
#endif

/****************************************
* Data Type
****************************************/

#if defined(UECHO_NET_USE_SOCKET_LIST)
#include <uecho/util/list.h>
#endif

typedef struct _uEchoSocket {
#if defined(UECHO_NET_USE_SOCKET_LIST)
	bool headFlag;
	struct _uEchoSocket *prev;
	struct _uEchoSocket *next;
#endif
	SOCKET id;
	int type;
	int direction;
	uEchoString *ipaddr;
	int port;
#if defined(ITRON)
	UH *sendWinBuf;
	UH *recvWinBuf;
#endif
#if defined(CG_USE_OPENSSL)
	SSL_CTX* ctx;
	SSL* ssl;
#endif
} uEchoSocket, uEchoSocketList;

typedef struct _uEchoDatagramPacket {
	uEchoString *data;
	uEchoString *localAddress;
	int localPort;
	uEchoString *remoteAddress;
	int remotePort;
} uEchoDatagramPacket;

/****************************************
* Function (Socket)
****************************************/

void uecho_socket_startup();
void uecho_socket_cleanup();

uEchoSocket *uecho_socket_new(int type);
#define uecho_socket_stream_new() uecho_socket_new(UECHO_NET_SOCKET_STREAM)
#define uecho_socket_dgram_new() uecho_socket_new(UECHO_NET_SOCKET_DGRAM)
bool uecho_socket_delete(uEchoSocket *socket);

void uecho_socket_setid(uEchoSocket *socket, SOCKET value);
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
#define uecho_socket_setport(socket, value) (socket->port = value)
#define uecho_socket_getaddress(socket) uecho_string_getvalue(socket->ipaddr)
#define uecho_socket_getport(socket) (socket->port)

bool uecho_socket_isbound(uEchoSocket *socket);
bool uecho_socket_close(uEchoSocket *socket);

bool uecho_socket_listen(uEchoSocket *socket);

bool uecho_socket_bind(uEchoSocket *sock, int bindPort, const char *bindAddr, bool bindFlag, bool reuseFlag);
bool uecho_socket_accept(uEchoSocket *sock, uEchoSocket *clientSock);
bool uecho_socket_connect(uEchoSocket *sock, const char *addr, int port);
ssize_t uecho_socket_read(uEchoSocket *sock, char *buffer, size_t bufferLen);
size_t uecho_socket_write(uEchoSocket *sock, const char *buffer, size_t bufferLen);
ssize_t uecho_socket_readline(uEchoSocket *sock, char *buffer, size_t bufferLen);
size_t uecho_socket_skip(uEchoSocket *sock, size_t skipLen);

size_t uecho_socket_sendto(uEchoSocket *sock, const char *addr, int port, const char *data, size_t dataeLen);
ssize_t uecho_socket_recv(uEchoSocket *sock, uEchoDatagramPacket *dgmPkt);

int uecho_socket_getlasterror();

/****************************************
* Function (Multicast)
****************************************/

bool uecho_socket_joingroup(uEchoSocket *sock, const char *mcastAddr, const char *ifAddr);

/****************************************
* Function (Option)
****************************************/

bool uecho_socket_setreuseaddress(uEchoSocket *socket, bool flag);
bool uecho_socket_setmulticastttl(uEchoSocket *sock,  int ttl);
bool uecho_socket_settimeout(uEchoSocket *sock, int sec);

/****************************************
* Function (DatagramPacket)
****************************************/

uEchoDatagramPacket *uecho_socket_datagram_packet_new();
void uecho_socket_datagram_packet_delete(uEchoDatagramPacket *dgmPkt);

#define uecho_socket_datagram_packet_setdata(dgmPkt, value) uecho_string_setvalue(dgmPkt->data, value)
#define uecho_socket_datagram_packet_getdata(dgmPkt) uecho_string_getvalue(dgmPkt->data)
#define uecho_socket_datagram_packet_getlength(dgmPkt) uecho_string_length(dgmPkt->data)

#define uecho_socket_datagram_packet_setlocaladdress(dgmPkt, addr) uecho_string_setvalue(dgmPkt->localAddress, addr)
#define uecho_socket_datagram_packet_getlocaladdress(dgmPkt) uecho_string_getvalue(dgmPkt->localAddress)
#define uecho_socket_datagram_packet_setlocalport(dgmPkt, port) (dgmPkt->localPort = port)
#define uecho_socket_datagram_packet_getlocalport(dgmPkt) (dgmPkt->localPort)
#define uecho_socket_datagram_packet_setremoteaddress(dgmPkt, addr) uecho_string_setvalue(dgmPkt->remoteAddress, addr)
#define uecho_socket_datagram_packet_getremoteaddress(dgmPkt) uecho_string_getvalue(dgmPkt->remoteAddress)
#define uecho_socket_datagram_packet_setremoteport(dgmPkt, port) (dgmPkt->remotePort = port)
#define uecho_socket_datagram_packet_getremoteport(dgmPkt) (dgmPkt->remotePort)

void uecho_socket_datagram_packet_copy(uEchoDatagramPacket *dstDgmPkt, uEchoDatagramPacket *srcDgmPkt);

/****************************************
* Function (SSLSocket)
****************************************/

#if defined(CG_USE_OPENSSL)
#define UECHO_NET_SOCKET_SSL 0x0100
#define uecho_socket_ssl_new() uecho_socket_new(UECHO_NET_SOCKET_STREAM | UECHO_NET_SOCKET_SSL)
#define uecho_socket_isssl(socket) ((socket->type & UECHO_NET_SOCKET_SSL) ? true : false)
#endif

/****************************************
* Function (SocketList)
****************************************/

#if defined(UECHO_NET_USE_SOCKET_LIST)

#define uecho_socket_next(sock) (uEchoSocket *)uecho_list_next((uEchoList *)sock)

uEchoSocketList *uecho_socketlist_new();
void uecho_socketlist_delete(uEchoSocketList *sockList);

#define uecho_socketlist_clear(sockList) uecho_list_clear((uEchoList *)sockList, (CG_LIST_DESTRUCTORFUNC)uecho_socket_delete)
#define uecho_socketlist_size(sockList) uecho_list_size((uEchoList *)sockList)
#define uecho_socketlist_gets(sockList) (uEchoSocket *)uecho_list_next((uEchoList *)sockList)
#define uecho_socketlist_add(sockList, sock) uecho_list_add((uEchoList *)sockList, (uEchoList *)sock)

#endif

#ifdef  __cplusplus
}
#endif

#endif
