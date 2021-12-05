#ifndef __unp_h
#define __unp_h

#ifdef _WIN32
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bzero(ptr,n) memset(ptr,0,n)
#define MAXLINE 4096
#define BUFFSIZE 8192
#define LISTENQ 1024
#define SA struct sockaddr
#endif /*__unp_h*/