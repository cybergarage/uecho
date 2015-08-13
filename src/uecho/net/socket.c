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
#include <uecho/net/interface.h>
#include <uecho/util/timer.h>

#include <string.h>

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#if defined(UECHO_USE_OPENSSL)
#pragma comment(lib, "libeay32MD.lib")
#pragma comment(lib, "ssleay32MD.lib")
#endif
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#endif

/****************************************
* static variable
****************************************/

static int socketCnt = 0;

/****************************************
* prototype
****************************************/

bool uecho_socket_tosockaddrin(const char *addr, int port, struct sockaddr_in *sockaddr, bool isBindAddr);
bool uecho_socket_tosockaddrinfo(int sockType, const char *addr, int port, struct addrinfo **addrInfo, bool isBindAddr);

#define uecho_socket_getrawtype(socket) (((socket->type & UECHO_NET_SOCKET_STREAM) == UECHO_NET_SOCKET_STREAM) ? SOCK_STREAM : SOCK_DGRAM)

/****************************************
*
* Socket
*
****************************************/

/****************************************
* uecho_socket_startup
****************************************/

void uecho_socket_startup(void)
{
	if (socketCnt == 0)
{
#if defined(WIN32)
		WSADATA wsaData;
		int err;

    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    
#if !defined(WIN32)
    // Thanks for Brent Hills (10/26/04)
		signal(SIGPIPE,SIG_IGN);
#endif

#if defined(UECHO_USE_OPENSSL)
		SSL_library_init(); 
#endif
	}
	socketCnt++;
}

/****************************************
* uecho_socket_cleanup
****************************************/

void uecho_socket_cleanup(void)
{
	socketCnt--;
	if (socketCnt <= 0)
{
#if defined(WIN32)
		WSACleanup( );
#endif

#if !defined(WIN32)
		// Thanks for Brent Hills (10/26/04)
		signal(SIGPIPE,SIG_DFL);
#endif
	}
}

/****************************************
* uecho_socket_new
****************************************/

uEchoSocket *uecho_socket_new(int type)
{
	uEchoSocket *sock;

	uecho_socket_startup();

	sock = (uEchoSocket *)malloc(sizeof(uEchoSocket));
  if (!sock)
    return NULL;

#if defined(WIN32)
  sock->id = INVALID_SOCKET;
#else
  sock->id = -1;
#endif

  uecho_socket_settype(sock, type);
  uecho_socket_setdirection(sock, UECHO_NET_SOCKET_NONE);

  sock->ipaddr = uecho_string_new();

  uecho_socket_setaddress(sock, "");
  uecho_socket_setport(sock, -1);

#if defined(UECHO_USE_OPENSSL)
  sock->ctx = NULL;
  sock->ssl = NULL;
#endif

	return sock;
}

/****************************************
* uecho_socket_delete
****************************************/

bool uecho_socket_delete(uEchoSocket *sock)
{
  if (!sock)
    return true;
  
	uecho_socket_close(sock);
	uecho_string_delete(sock->ipaddr);
	free(sock);
  
	uecho_socket_cleanup();

	return true;
}

/****************************************
* uecho_socket_isbound
****************************************/

bool uecho_socket_isbound(uEchoSocket *sock)
{
  if (!sock)
    return false;
  
#if defined(WIN32)
	return (sock->id != INVALID_SOCKET) ? true: false;
#else
	return (0 < sock->id) ? true : false;
#endif
}

/****************************************
* uecho_socket_setid
****************************************/

void uecho_socket_setid(uEchoSocket *sock, SOCKET value)
{
#if defined(WIN32) || defined(HAVE_IP_PKTINFO) || (!defined(WIN32) && defined(HAVE_SO_NOSIGPIPE))
	int on=1;
#endif

  if (!sock)
    return;
  
	sock->id=value;

#if defined(WIN32) || defined(HAVE_IP_PKTINFO)
	if ( UECHO_NET_SOCKET_DGRAM == uecho_socket_gettype(socket) ) 
		setsockopt(sock->id, IPPROTO_IP, IP_PKTINFO,  &on, sizeof(on));
#endif

#if !defined(WIN32) && defined(HAVE_SO_NOSIGPIPE)
	setsockopt(sock->id, SOL_SOCKET, SO_NOSIGPIPE,  &on, sizeof(on));
#endif
}

/****************************************
* uecho_socket_close
****************************************/

bool uecho_socket_close(uEchoSocket *sock)
{
  if (!sock)
    return true;
  
	if (uecho_socket_isbound(sock) == false)
		return true;

#if defined(UECHO_USE_OPENSSL)
	if (uecho_socket_isssl(sock) == true)
{
		if (sock->ctx)
{
			SSL_shutdown(sock->ssl); 
			SSL_free(sock->ssl);
			sock->ssl = NULL;
		}
		if (sock->ctx)
{
			SSL_CTX_free(sock->ctx);
			sock->ctx = NULL;
		}
	}
#endif

#if defined(WIN32)
	WSAAsyncSelect(sock->id, NULL, 0, FD_CLOSE);
	shutdown(sock->id, SD_BOTH );
  closesocket(sock->id);
  
  sock->id = INVALID_SOCKET;
#else
	int flag = fcntl(sock->id, F_GETFL, 0);
	if (0 <= flag)
		fcntl(sock->id, F_SETFL, flag | O_NONBLOCK);
	shutdown(sock->id, 2);
	close(sock->id);

  sock->id = -1;
#endif

	uecho_socket_setaddress(sock, "");
	uecho_socket_setport(sock, -1);

	return true;
}

/****************************************
* uecho_socket_listen
****************************************/

bool uecho_socket_listen(uEchoSocket *sock)
{
  if (!sock)
    return false;
  
	int ret = listen(sock->id, SOMAXCONN);
	return (ret == 0) ? true: false;
}

/****************************************
* uecho_socket_bind
****************************************/

bool uecho_socket_bind(uEchoSocket *sock, int bindPort, const char *bindAddr, bool bindFlag, bool reuseFlag)
{
	struct addrinfo *addrInfo;
	int ret;

  if (!sock)
    return false;
  
	if (bindPort <= 0 /* || bindAddr == NULL*/)
		return false;

	if (uecho_socket_tosockaddrinfo(uecho_socket_getrawtype(sock), bindAddr, bindPort, &addrInfo, bindFlag) == false)
		return false;
	uecho_socket_setid(sock, socket(addrInfo->ai_family, addrInfo->ai_socktype, 0));
	if (sock->id== -1)
{
		uecho_socket_close(sock);
		return false;
	}
	if (reuseFlag == true)
{
		if (uecho_socket_setreuseaddress(sock, true) == false)
{
			uecho_socket_close(sock);
			return false;
		}
	}
	ret = bind(sock->id, addrInfo->ai_addr, addrInfo->ai_addrlen);
	freeaddrinfo(addrInfo);

	if (ret != 0)
		return false;

	uecho_socket_setdirection(sock, UECHO_NET_SOCKET_SERVER);
	uecho_socket_setaddress(sock, bindAddr);
	uecho_socket_setport(sock, bindPort);

	return true;
}

/****************************************
* uecho_socket_accept
****************************************/

bool uecho_socket_accept(uEchoSocket *serverSock, uEchoSocket *clientSock)
{
	struct sockaddr_in sockaddr;
	socklen_t socklen;
	char localAddr[UECHO_NET_SOCKET_MAXHOST];
	char localPort[UECHO_NET_SOCKET_MAXSERV];
	struct sockaddr_storage sockClientAddr;
	socklen_t nLength = sizeof(sockClientAddr);

  uecho_socket_setid(clientSock, accept(serverSock->id, (struct sockaddr *)&sockClientAddr, &nLength));

#if defined (WIN32)
	if (clientSock->id == INVALID_SOCKET)
		return false;
#else
	if (clientSock->id < 0)
		return false;
#endif
	
	uecho_socket_setaddress(clientSock, uecho_socket_getaddress(serverSock));
	uecho_socket_setport(clientSock, uecho_socket_getport(serverSock));
	socklen = sizeof(struct sockaddr_in);
	
	if (getsockname(clientSock->id, (struct sockaddr *)&sockaddr, &socklen) == 0 &&
	    getnameinfo((struct sockaddr *)&sockaddr, socklen, localAddr, sizeof(localAddr), 
			localPort, sizeof(localPort), NI_NUMERICHOST | NI_NUMERICSERV) == 0) 
	{
		/* Set address for the sockaddr to real addr */
		uecho_socket_setaddress(clientSock, localAddr);
	}
	
	return true;
}

/****************************************
* uecho_socket_connect
****************************************/

bool uecho_socket_connect(uEchoSocket *sock, const char *addr, int port)
{
	struct addrinfo *toaddrInfo;
	int ret;

  if (!sock)
    return false;
  
  if (uecho_socket_tosockaddrinfo(uecho_socket_getrawtype(sock), addr, port, &toaddrInfo, true) == false)
		return false;
	
  if (uecho_socket_isbound(sock) == false)
{
		uecho_socket_setid(sock, socket(toaddrInfo->ai_family, toaddrInfo->ai_socktype, 0));
  }
  
  ret = connect(sock->id, toaddrInfo->ai_addr, toaddrInfo->ai_addrlen);
	freeaddrinfo(toaddrInfo);

	uecho_socket_setdirection(sock, UECHO_NET_SOCKET_CLIENT);

#if defined(UECHO_USE_OPENSSL)
	if (uecho_socket_isssl(sock) == true)
{
		sock->ctx = SSL_CTX_new( SSLv23_client_method());
		sock->ssl = SSL_new(sock->ctx);
		if (SSL_set_fd(sock->ssl, uecho_socket_getid(sock)) == 0)
{
			uecho_socket_close(sock);
			return false;
		}
		if (SSL_connect(sock->ssl) < 1)
{
			uecho_socket_close(sock);
			return false;
		}
	}
#endif

	return (ret == 0) ? true : false;
}

/****************************************
* uecho_socket_read
****************************************/

ssize_t uecho_socket_read(uEchoSocket *sock, char *buffer, size_t bufferLen)
{
	ssize_t recvLen;

  if (!sock)
    return -1;
  
#if defined(UECHO_USE_OPENSSL)
	if (uecho_socket_isssl(sock) == false)
{
#endif

	recvLen = recv(sock->id, buffer, bufferLen, 0);

#if defined(UECHO_USE_OPENSSL)
	}
	else {
		recvLen = SSL_read(sock->ssl, buffer, bufferLen);
	}
#endif

	return recvLen;
}

/****************************************
* uecho_socket_write
****************************************/

#define UECHO_NET_SOCKET_SEND_RETRY_CNT 10
#define UECHO_NET_SOCKET_SEND_RETRY_WAIT_MSEC 20

size_t uecho_socket_write(uEchoSocket *sock, const char *cmd, size_t cmdLen)
{
	ssize_t nSent;
	size_t nTotalSent = 0;
	size_t cmdPos = 0;
	int retryCnt = 0;

  if (!sock)
    return 0;
  
	if (cmdLen <= 0)
		return 0;

	do {
#if defined(UECHO_USE_OPENSSL)
		if (uecho_socket_isssl(sock) == false)
{
#endif

		nSent = send(sock->id, cmd + cmdPos, cmdLen, 0);
			
#if defined(UECHO_USE_OPENSSL)
		}
		else {
			nSent = SSL_write(sock->ssl, cmd + cmdPos, cmdLen);
		}
#endif

		if (nSent <= 0)
{
			retryCnt++;
			if (UECHO_NET_SOCKET_SEND_RETRY_CNT < retryCnt)
			{
				nTotalSent = 0;
				break;
			}

			uecho_wait(UECHO_NET_SOCKET_SEND_RETRY_WAIT_MSEC);
		}
		else
		{
			nTotalSent += nSent;
			cmdPos += nSent;
			cmdLen -= nSent;
			retryCnt = 0;
		}

	} while (0 < cmdLen);

	return nTotalSent;
}
/****************************************
* uecho_socket_readline
****************************************/

ssize_t uecho_socket_readline(uEchoSocket *sock, char *buffer, size_t bufferLen)
{
	ssize_t readCnt;
	ssize_t readLen;
	char c;

  if (!sock)
    return -1;
  
	readCnt = 0;
	while (readCnt < (bufferLen-1))
{
		readLen = uecho_socket_read(sock, &buffer[readCnt], sizeof(char));
		if (readLen <= 0)
			return -1;
		readCnt++;
		if (buffer[readCnt-1] == UECHO_SOCKET_LF)
			break;
	}
	buffer[readCnt] = '\0';
	if (buffer[readCnt-1] != UECHO_SOCKET_LF)
{
		do {
			readLen = uecho_socket_read(sock, &c, sizeof(char));
			if (readLen <= 0)
				break;
		} while (c != UECHO_SOCKET_LF);
	}

	return readCnt;	
}

/****************************************
* uecho_socket_skip
****************************************/

size_t uecho_socket_skip(uEchoSocket *sock, size_t skipLen)
{
	ssize_t readCnt;
	ssize_t readLen;
	char c;

  if (!sock)
    return 0;
  
	readCnt = 0;
	while (readCnt < skipLen)
{
		readLen = uecho_socket_read(sock, &c, sizeof(char));
		if (readLen <= 0)
			break;
		readCnt++;
	}

	return readCnt;
}

/****************************************
* uecho_socket_sendto
****************************************/

size_t uecho_socket_sendto(uEchoSocket *sock, const char *addr, int port, byte *data, size_t dataLen)
{
	struct addrinfo *addrInfo;
	ssize_t sentLen;
	bool isBoundFlag;

  if (!sock)
    return 0;
  
	if (!data && (dataLen <= 0))
		return 0;
  
	isBoundFlag = uecho_socket_isbound(sock);
	sentLen = -1;

  if (uecho_socket_tosockaddrinfo(uecho_socket_getrawtype(sock), addr, port, &addrInfo, true) == false)
		return -1;
	if (isBoundFlag == false)
		uecho_socket_setid(sock, socket(addrInfo->ai_family, addrInfo->ai_socktype, 0));
	
	/* Setting multicast time to live in any case to default */
	uecho_socket_setmulticastttl(sock, UECHO_NET_SOCKET_MULTICAST_DEFAULT_TTL);
	
	if (0 <= sock->id)
		sentLen = sendto(sock->id, data, dataLen, 0, addrInfo->ai_addr, addrInfo->ai_addrlen);
	freeaddrinfo(addrInfo);

	if (isBoundFlag == false)
		uecho_socket_close(sock);

  return sentLen;
}

/****************************************
* uecho_socket_recv
****************************************/

ssize_t uecho_socket_recv(uEchoSocket *sock, uEchoDatagramPacket *dgmPkt)
{
	ssize_t recvLen = 0;
	byte recvBuf[UECHO_NET_SOCKET_DGRAM_RECV_BUFSIZE+1];
	char remoteAddr[UECHO_NET_SOCKET_MAXHOST];
	char remotePort[UECHO_NET_SOCKET_MAXSERV];
	char *localAddr;
  struct sockaddr_storage from;
  socklen_t fromLen;
  
  if (!sock)
    return -1;
  
  fromLen = sizeof(from);
	recvLen = recvfrom(sock->id, recvBuf, sizeof(recvBuf)-1, 0, (struct sockaddr *)&from, &fromLen);

	if (recvLen <= 0)
		return 0;

	uecho_socket_datagram_packet_setdata(dgmPkt, recvBuf, recvLen);

	uecho_socket_datagram_packet_setlocalport(dgmPkt, uecho_socket_getport(sock));
	uecho_socket_datagram_packet_setremoteaddress(dgmPkt, "");
	uecho_socket_datagram_packet_setremoteport(dgmPkt, 0);

	if (getnameinfo((struct sockaddr *)&from, fromLen, remoteAddr, sizeof(remoteAddr), remotePort, sizeof(remotePort), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
{
		uecho_socket_datagram_packet_setremoteaddress(dgmPkt, remoteAddr);
		uecho_socket_datagram_packet_setremoteport(dgmPkt, uecho_str2int(remotePort));
	}

	localAddr = uecho_net_selectaddr((struct sockaddr *)&from);
	uecho_socket_datagram_packet_setlocaladdress(dgmPkt, localAddr);
	free(localAddr);

	return recvLen;
}

/****************************************
* uecho_socket_setreuseaddress
****************************************/

bool uecho_socket_setreuseaddress(uEchoSocket *sock, bool flag)
{
	int sockOptRet;
#if defined (WIN32)
	bool optval;
#else
	int optval;
#endif

  if (!sock)
    return false;
  
#if defined (WIN32)
	optval = (flag == true) ? 1 : 0;
	sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
#else
	optval = (flag == true) ? 1 : 0;
	sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
	#if defined(USE_SO_REUSEPORT) || defined(TARGET_OS_MAC) || defined(TARGET_OS_IPHONE)
	if (sockOptRet == 0)
{
    sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_REUSEPORT, (const char *)&optval, sizeof(optval));
  }
	#endif
#endif

	return (sockOptRet == 0) ? true : false;
}

/****************************************
* uecho_socket_setmulticastttl
****************************************/

bool uecho_socket_setmulticastttl(uEchoSocket *sock, int ttl)
{
	int sockOptRet;
	int ttl_;
	unsigned int len=0;

  if (!sock)
    return false;

#if defined (WIN32)
	sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char *)&ttl, sizeof(ttl));
#else
	sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const unsigned char *)&ttl, sizeof(ttl));
	if (sockOptRet == 0)
{
		len = sizeof(ttl_);
		getsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, &ttl_, (socklen_t*)&len);
	}
#endif

	return (sockOptRet == 0) ? true : false;
}

/****************************************
* uecho_socket_settimeout
****************************************/

bool uecho_socket_settimeout(uEchoSocket *sock, int sec)
{
	int sockOptRet;

#if defined (WIN32)
	struct timeval timeout;
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

  if (!sock)
    return false;
  
	sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
	if (sockOptRet == 0)
		sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
#else
	struct timeval timeout;
	timeout.tv_sec = (clock_t)sec;
	timeout.tv_usec = 0;

  if (!sock)
    return false;
  
	sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
	if (sockOptRet == 0)
		sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
#endif

	return (sockOptRet == 0) ? true : false;
}

/****************************************
* uecho_socket_joingroup
****************************************/

bool uecho_socket_joingroup(uEchoSocket *sock, const char *mcastAddr, const char *ifAddr)
{
	struct addrinfo hints;
	struct addrinfo *mcastAddrInfo, *ifAddrInfo;
	
	/**** for IPv6 ****/
	struct ipv6_mreq ipv6mr;
	struct sockaddr_in6 toaddr6, ifaddr6;
	int scopeID;
	
	/**** for IPv4 ****/	
	struct ip_mreq ipmr;
	struct sockaddr_in toaddr, ifaddr;
	
	bool joinSuccess;
	int sockOptRetCode;

  if (!sock)
    return false;
  
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags= AI_NUMERICHOST | AI_PASSIVE;

	if (getaddrinfo(mcastAddr, NULL, &hints, &mcastAddrInfo) != 0) 
		return false;

	if (getaddrinfo(ifAddr, NULL, &hints, &ifAddrInfo) != 0)
{
		freeaddrinfo(mcastAddrInfo);
		return false;
	}

	joinSuccess = true;
	
	if (uecho_net_isipv6address(mcastAddr) == true)
{
		memcpy(&toaddr6, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
		memcpy(&ifaddr6, ifAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
		ipv6mr.ipv6mr_multiaddr = toaddr6.sin6_addr;	
		scopeID = uecho_net_getipv6scopeid(ifAddr);
		ipv6mr.ipv6mr_interface = scopeID /*if_nametoindex*/;
		
		sockOptRetCode = setsockopt(sock->id, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&scopeID, sizeof(scopeID));

		if (sockOptRetCode != 0)
			joinSuccess = false;

		sockOptRetCode = setsockopt(sock->id, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&ipv6mr, sizeof(ipv6mr));

		if (sockOptRetCode != 0)
			joinSuccess = false;

	}
	else {
		memcpy(&toaddr, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in));
		memcpy(&ifaddr, ifAddrInfo->ai_addr, sizeof(struct sockaddr_in));
		memcpy(&ipmr.imr_multiaddr.s_addr, &toaddr.sin_addr, sizeof(struct in_addr));
		memcpy(&ipmr.imr_interface.s_addr, &ifaddr.sin_addr, sizeof(struct in_addr));
		sockOptRetCode = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ipmr.imr_interface.s_addr, sizeof(struct in_addr));
		if (sockOptRetCode != 0)
			joinSuccess = false;
		sockOptRetCode = setsockopt(sock->id, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipmr, sizeof(ipmr));
		
		if (sockOptRetCode != 0)
			joinSuccess = false;
	}

	freeaddrinfo(mcastAddrInfo);
	freeaddrinfo(ifAddrInfo);

	return joinSuccess;
}

/****************************************
* uecho_socket_tosockaddrin
****************************************/

bool uecho_socket_tosockaddrin(const char *addr, int port, struct sockaddr_in *sockaddr, bool isBindAddr)
{
	uecho_socket_startup();

	memset(sockaddr, 0, sizeof(struct sockaddr_in));

	sockaddr->sin_family = AF_INET;
	sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr->sin_port = htons((unsigned short)port);

	if (isBindAddr == true)
{
		sockaddr->sin_addr.s_addr = inet_addr(addr);
		if (sockaddr->sin_addr.s_addr == INADDR_NONE)
{
			struct hostent *hent = gethostbyname(addr);
			if (hent == NULL)
				return false;
			memcpy(&(sockaddr->sin_addr), hent->h_addr, hent->h_length);
		}
	}

	return true;
}

/****************************************
* uecho_socket_tosockaddrinfo
****************************************/

bool uecho_socket_tosockaddrinfo(int sockType, const char *addr, int port, struct addrinfo **addrInfo, bool isBindAddr)
{
	struct addrinfo hints;
	char portStr[32];
	int errorn;

	uecho_socket_startup();

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = sockType;
	hints.ai_flags= /*AI_NUMERICHOST | */AI_PASSIVE;
	sprintf(portStr, "%d", port);
	if ( (errorn = getaddrinfo(addr, portStr, &hints, addrInfo)) != 0)
		return false;
	if (isBindAddr == true)
		return true;
	hints.ai_family = (*addrInfo)->ai_family;
	freeaddrinfo(*addrInfo);
	if ((errorn = getaddrinfo(NULL, portStr, &hints, addrInfo)) != 0)
		return false;
	return true;
}
