//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName     :   GC_Socket
//  Version      :   1.0
//  Creater      :   michaelzhou
//  Date         :   2013-7-15   18:09:29
//  Comment    	 :   
//
//////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32

#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#endif

#include "GC_Socket.h"
#include <assert.h>
/**
 * IGC_SocketStream接口的实现
 *
 */
class GC_SocketStream : public IGC_SocketStream
{
public:
	GC_SocketStream(int nSocket, struct sockaddr_in *pRemoteAddr);

	virtual int SetTimeout(const timeval *pTimeout);

	virtual int CheckCanSend(const timeval *pTimeout);

	virtual int Send(char *piBuffer, int nSize);

	virtual int CheckCanRecv(const timeval *pTimeout);

	virtual int Recv(char **ppiRetBuffer, int *pRetSize);

	virtual int IsAlive();

	virtual int GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort);

	virtual int SetUserData(void *pvUserData) { m_pvUserData = pvUserData; return true; }

	virtual void *GetUserData() { return m_pvUserData; }

	virtual int GetLastError() { return m_nErrorCode; }

    virtual void CloseSocket();
protected:
	int                 m_nSocketHandle;
	timeval             m_Timeout;
	struct sockaddr_in  m_RemoteAddress;
	void               *m_pvUserData;
	int                 m_nErrorCode;
};

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanRecv(int nSocket, const timeval *pcTimeout)
{
	if (nSocket < 0)
		return -1;

#ifdef WIN32

	struct fd_set fds;
	//struct timeval timeout={3,0};		//select等待3秒，3秒轮询，要非阻塞就置0 

	FD_ZERO(&fds);						//每次循环都要清空集合，否则不能检测描述符变化
	FD_SET(nSocket,&fds);				//添加描述符
	
	int ret = select(nSocket + 1,&fds,NULL,NULL,pcTimeout);   //select使用 
	if (ret == -1)
	{
		return -1;			//select错误，退出程序 
	}

	if (ret == 0)
	{
		return 0;			//没有数据到达
	}
	if(FD_ISSET(nSocket,&fds)) //测试sock是否可读，即是否网络上有数据
	{
		return 1;
	}

	return -1;
#else

	int nTimeout = -1;
	if (pcTimeout)
	{
		nTimeout = (pcTimeout->tv_sec * 1000) + (pcTimeout->tv_usec / 1000);
	}

	struct pollfd PollFD;
	PollFD.fd = nSocket;
	PollFD.events  = POLLIN;
	PollFD.revents = 0;

	int nRetCode = poll(&PollFD, 1, nTimeout);

	if (nRetCode == 0)
		return 0;

	if (nRetCode > 0)
	{
		assert(
			(PollFD.revents & POLLIN ) ||
			(PollFD.revents & POLLERR) ||
			(PollFD.revents & POLLHUP)
			);
		return 1;
	}
#endif    
}

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanSend(int nSocket, const timeval *pcTimeout)
{
#ifdef WIN32
	struct fd_set fds;
	//struct timeval timeout={3,0};		//select等待3秒，3秒轮询，要非阻塞就置0 

	FD_ZERO(&fds);						//每次循环都要清空集合，否则不能检测描述符变化
	FD_SET(nSocket,&fds);				//添加描述符

	int ret = select(nSocket + 1,NULL,&fds,NULL,pcTimeout);   //select使用 
	if (ret == -1)
	{
		return -1;					//select错误，退出程序 
	}

	if (ret == 0)
	{
		return 0;					//没有数据到达
	}
	if(FD_ISSET(nSocket,&fds))		//测试sock是否可读，即是否网络上有数据
	{
		return 1;
	}

	return ret;
#else 
	if (nSocket < 0)
		return -1;

	int nTimeout = -1;
	if (pcTimeout)
	{
		nTimeout = (pcTimeout->tv_sec * 1000) + (pcTimeout->tv_usec / 1000);
	}

	struct pollfd PollFD;
	PollFD.fd = nSocket;
	PollFD.events  = POLLOUT;
	PollFD.revents = 0;

	int nRetCode = poll(&PollFD, 1, nTimeout);

	if (nRetCode == 0)
		return 0;

	if (nRetCode > 0)
	{
		assert(
			(PollFD.revents & POLLOUT) ||
			(PollFD.revents & POLLERR) ||
			(PollFD.revents & POLLHUP)
			);
		return 1;
	}
#endif
}

static inline int _IsSocketCanRestore()
{
	return (errno == EINTR);
}

static inline int _GetSocketErrorCode()
{
	return errno;
}
static inline int _CloseSocket(int nSocket)
{
#ifdef WIN32
	return closesocket(nSocket);
#else
	struct linger lingerStruct;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;

	setsockopt(
		nSocket,
		SOL_SOCKET, SO_LINGER, 
		(char *)&lingerStruct, sizeof(lingerStruct)
		);

	return close(nSocket);
#endif
}

// if nSendFlag == false, then RecvBuffer
// if nSendFlag == true,  then SendBuffer   00002746 
// return -1: error, 0: timeout, 1: success
static inline int _SendOrRecvBuffer(
									const int nSendFlag,
									int nSocket, 
									unsigned char *pbyBuffer, int *nSize, 
									const timeval &ProcessTimeout
									)
{
	int nResult  = -1;      // error
	int nRetCode = false;

	const timeval *pcTimeout = NULL;
	if (ProcessTimeout.tv_sec != -1)
		pcTimeout = &ProcessTimeout;

	KG_PROCESS_ERROR(nSocket != -1);
	KG_PROCESS_ERROR(pbyBuffer);
	KG_PROCESS_ERROR(*nSize > 0);

	while (nSize > 0)
	{
		if (nSendFlag)
		{
			//when timeout disabled,pTimeout was set to NULL.
			nRetCode = _CheckCanSend(nSocket, pcTimeout);
			KG_PROCESS_ERROR_RET_CODE(nRetCode != 0, nRetCode); // if timeout
			if (nRetCode < 0)  //error
			{
				nRetCode = _IsSocketCanRestore();
				if (nRetCode)   // if can restore then continue
					continue;

				goto Exit0;
			}
			nRetCode = send(nSocket, (char *)pbyBuffer, *nSize, 0);
		}
		else    // recv
		{
			//when timeout disabled,pTimeout was set to NULL.
			nRetCode = _CheckCanRecv(nSocket, pcTimeout);
			KG_PROCESS_ERROR_RET_CODE(nRetCode != 0, nRetCode); // if timeout
			if (nRetCode < 0)   // error
			{
				nRetCode = _IsSocketCanRestore();
				if (nRetCode)   // if can restore then continue
					continue;

				goto Exit0;
			}
			nRetCode = recv(nSocket, (char *)pbyBuffer, *nSize, 0);
			*nSize = nRetCode;
		}

		if (nRetCode == 0)  // Disconnected
			goto Exit0;

		if (nRetCode < 0)   // Error!
		{
			nRetCode = _IsSocketCanRestore();
			if (nRetCode)   // if can restore then continue
				continue;

			goto Exit0;
		}

		//pbyBuffer   += nRetCode;
		//nSize       = nRetCode;
		break; //break here?
	}

	nResult = 1;    // success
Exit0:
	return nResult;   
}

///////////////////////////////////////////////////////////////////////////////
// class GC_SocketStream
///////////////////////////////////////////////////////////////////////////////
GC_SocketStream::GC_SocketStream(int nSocket,  struct sockaddr_in *pRemoteAddr)
{
	assert(nSocket != -1);
	assert(pRemoteAddr);

	m_RemoteAddress     = *pRemoteAddr;
	m_nSocketHandle     = nSocket;
	m_Timeout.tv_sec    = -1;
	m_Timeout.tv_usec   = -1;
	m_pvUserData        = NULL;
	m_nErrorCode        = ERROR_CODE_SUCCESS;
}

int GC_SocketStream::IsAlive()
{
	int nRetsult = false;
	int nRetCode = false;
	volatile int nData = false; // no use


	KG_PROCESS_ERROR(m_nSocketHandle != -1);

	nRetCode = send(m_nSocketHandle, (char *)&nData, 0, 0);
	KG_PROCESS_ERROR(nRetCode != -1);

	nRetsult = true;
Exit0:
	return nRetsult;
}

int GC_SocketStream::GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort)
{
	int nResult  = false;

	KG_PROCESS_ERROR(m_nSocketHandle != -1);

	if (pRemoteIP)
		*pRemoteIP  = m_RemoteAddress.sin_addr;

	if (pusRemotePort)
		*pusRemotePort = m_RemoteAddress.sin_port;

	nResult = true;
Exit0:    
	return nResult;
}


int GC_SocketStream::SetTimeout(const timeval *pTimeout)
{
	if (!pTimeout)
	{
		m_Timeout.tv_sec  = -1;
		m_Timeout.tv_usec = -1;
		return true;
	}

	m_Timeout = *pTimeout;
	return true;
}

// return -1: error, 0: timeout, 1: success
int GC_SocketStream::CheckCanSend(const timeval *pTimeout)
{
	int nRetCode = _CheckCanSend(m_nSocketHandle, pTimeout);
	if(nRetCode ==  -1)
	{
		m_nErrorCode = _GetSocketErrorCode();
	}
	return nRetCode;
}


// return -1: error, 0: timeout, 1: success
int GC_SocketStream::Send(char *piBuffer, int nSize)
{
	int nResult  = -1;  // error
	int nRetCode = false;
	unsigned char *pbyBuffer	= NULL;
//	unsigned char *pPosBuffer	= NULL;
	unsigned uBufferSize = 0;

	KG_PROCESS_ERROR(piBuffer); 
	KG_PROCESS_ERROR(m_nSocketHandle != -1);

	uBufferSize = nSize;
	KG_PROCESS_ERROR(uBufferSize <= 65500);

	nRetCode = _SendOrRecvBuffer(
		true,   // SendFlag = true
		m_nSocketHandle, 
		(unsigned char *)piBuffer, 
		(int*)&uBufferSize, 
		m_Timeout      
		);
	if(nRetCode == -1)
	{
		m_nErrorCode = _GetSocketErrorCode();
	}
	KG_PROCESS_ERROR_RET_CODE(nRetCode > 0, nRetCode);

	nResult = 1;    // success
Exit0:
	if (pbyBuffer != NULL)
	{
		free(pbyBuffer);
		pbyBuffer = NULL;
	}
	return nResult;
}

// return -1: error, 0: timeout, 1: success
int GC_SocketStream::CheckCanRecv(const timeval *pTimeout)
{
	int nRetCode = _CheckCanRecv(m_nSocketHandle, pTimeout);
	if(nRetCode ==  -1)
	{
		m_nErrorCode = _GetSocketErrorCode();
	}
	return nRetCode;
}

// return -1: error, 0: timeout, 1: success
int GC_SocketStream::Recv(char **ppiRetBuffer, int *pRetSize)  
{
	int nResult     = -1;   // error
	int nRetCode    = false;

	char		*piBuffer = NULL;
	int        wDataLen = 0;

	KG_PROCESS_ERROR(m_nSocketHandle != -1);
	KG_PROCESS_ERROR(ppiRetBuffer);

	wDataLen = *pRetSize;

	piBuffer = (char *)malloc((unsigned)wDataLen);
	KG_PROCESS_ERROR(piBuffer);

	nRetCode = _SendOrRecvBuffer(
		false,  // SendFlag = false
		m_nSocketHandle, 
		(unsigned char *)piBuffer, 
		(int*)&wDataLen, 
		m_Timeout
		);
	if(nRetCode == -1)
	{
		m_nErrorCode = _GetSocketErrorCode();
	}
	KG_PROCESS_ERROR_RET_CODE(nRetCode > 0, nRetCode);

	memcpy(ppiRetBuffer, piBuffer, wDataLen);
	*pRetSize = wDataLen;
	
	nResult = 1;    // success

Exit0:
	if (piBuffer != NULL)
	{
		free(piBuffer);
		piBuffer = NULL;
	}
	return nResult;
}

void GC_SocketStream::CloseSocket()
{
    _CloseSocket(m_nSocketHandle);
    m_nSocketHandle = -1;
    
}
// =================================================================================================
// class KG_SocketConnector begin

GC_SocketConnector::GC_SocketConnector()
{
	memset((void *)&m_BindLoaclAddr, 0, sizeof(sockaddr_in));
	m_BindLoaclAddr.sin_family       = AF_INET;
	m_BindLoaclAddr.sin_addr.s_addr  = INADDR_ANY;
	m_BindLoaclAddr.sin_port         = htons(0);
}

IGC_SocketStream *GC_SocketConnector::Connect(const char cszIPAddress[], int nPort)
{
	int                 nResult         = false;
	int                 nRetCode        = false;
	IGC_SocketStream   *piResult        = NULL;
	int                 nSocket         = -1; 
	struct hostent     *pHost           = NULL;
	struct sockaddr_in  ServerAddr;
	timeval             TimeOutConnect  = {2, 0}; // 设置超时时间
	KG_PROCESS_ERROR(cszIPAddress);

#ifdef WIN32

	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if ( ret )
	{
		return NULL;
	}

	nSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	KG_PROCESS_ERROR(nSocket != -1);

	pHost = gethostbyname(cszIPAddress);
	if (NULL == pHost)
	{
		return NULL;
	}

	//struct sockaddr_in svraddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = *(unsigned long *)pHost->h_addr_list[0];//inet_addr(cszIPAddress);
	ServerAddr.sin_port = htons(nPort);
	ret = connect(nSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	
	if ( ret == SOCKET_ERROR ) 
	{
		return NULL;
	}
#else
	
	
	pHost = gethostbyname(cszIPAddress);
	KG_PROCESS_ERROR(pHost);

	memset((void *)&ServerAddr, 0, sizeof(sockaddr_in));
	ServerAddr.sin_family       = AF_INET;
	ServerAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
	ServerAddr.sin_port         = htons(nPort);

	nSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	KG_PROCESS_ERROR(nSocket != -1);

	nRetCode = bind(nSocket, (struct sockaddr *)&m_BindLoaclAddr, sizeof(sockaddr_in));
	KG_PROCESS_ERROR(nRetCode != -1);

	{
		// 屏蔽SIGPIPE信号。其实不需要每次都做这个操作，但是Connector没有Init函数，所以只能暂时先放这里了。
		typedef void (*SignalHandlerPointer)(int);
		SignalHandlerPointer pSignalHandler = SIG_ERR;

		pSignalHandler = signal(SIGPIPE, SIG_IGN);
		KG_PROCESS_ERROR(pSignalHandler != SIG_ERR);  // write when connection reset.
	}


	nRetCode = setsockopt(nSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)(&TimeOutConnect), sizeof(TimeOutConnect));
	KG_PROCESS_ERROR(nRetCode ==0 );

	nRetCode = connect(nSocket, (struct sockaddr *)&ServerAddr, sizeof(sockaddr_in));
	KG_PROCESS_ERROR(nRetCode != -1);

	TimeOutConnect.tv_sec = 75;
	TimeOutConnect.tv_usec = 0;
	nRetCode = setsockopt(nSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)(&TimeOutConnect), sizeof(TimeOutConnect));
	KG_PROCESS_ERROR(nRetCode ==0 );

#endif
	piResult = new GC_SocketStream(nSocket, &ServerAddr);
	KG_PROCESS_ERROR(piResult);

	nSocket = -1;

	nResult = true;

Exit0:
	if (!nResult)
	{
		if (nSocket != -1)
		{
			_CloseSocket(nSocket);
			nSocket = -1;
		}
	}
	return piResult;
}


