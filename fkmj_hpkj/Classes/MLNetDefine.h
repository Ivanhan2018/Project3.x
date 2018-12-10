/****************************************************************************
Copyright (c) 2014 Lijunlin - Jason lee

Created by Lijunlin - Jason lee on 2014

jason.lee.c@foxmail.com
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCNET_MACROS_H__
#define __CCNET_MACROS_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef unsigned int          SOCKET;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#include <WS2tcpip.h>
typedef int socklen_t;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#endif

#ifndef DEBUG_SOCKET_DATA_INFO 
#define DEBUG_SOCKET_DATA_INFO 0
#endif // DEBUG_SOCKET_DATA_INFO 

#ifndef SOCKET_SOTIMEOUT
#define SOCKET_SOTIMEOUT 5.0f
#endif
#ifndef SOCKET_READ_BUFFER_SIZE
#define SOCKET_READ_BUFFER_SIZE 20480//16384//
#endif
#ifndef MEMORYTYPE_REVERSE
#define MEMORYTYPE_REVERSE 0
#endif
#ifndef HANDLE_ON_SINGLE_FRAME
#define HANDLE_ON_SINGLE_FRAME 1
#endif
#ifndef CCBUFFER_ORGINAL_SIZE
#define CCBUFFER_ORGINAL_SIZE 512
#endif

#ifndef SOCKET_HOSTPORT
#define SOCKET_HOSTPORT 3193
#endif

#ifndef SOCKET_GAMEPORT
#define SOCKET_GAMEPORT 3194
#endif

/**
 * enum   : CCSocketStatus
 * author : Jason lee
 * email  : jason.lee.c@foxmail.com
 * descpt : socket status define
 */
enum CCSocketStatus
{
	eSocketConnected			=  1,
	eSocketConnecting			=  2,
	eSocketDisconnected			=  3,
	eSocketConnectFailed		=  4,
	eSocketConnectTimeout		=  5,
	eSocketIoClosed				=  0,
	eSocketIoError				= -1,
	eSocketCreateFailed			= -2,
};

static bool isSocketVaild(const SOCKET& iSocket,bool bRecive)
{

	if (iSocket == INVALID_SOCKET)
	{
		return false;
	}
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(iSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;


	if(!bRecive && select((int)(iSocket + 1), NULL, &fd, NULL, &tv) > 0 )
	{
		if( FD_ISSET(iSocket, &fd) )
		{
			return true;
		}
	}
	if(bRecive &&  select((int)(iSocket + 1), &fd, NULL, NULL, &tv) > 0 )
	{
		if( FD_ISSET(iSocket, &fd) )
		{
			return true;
		}
	}

	return false;
}

#endif //__CCNET_MACROS_H__