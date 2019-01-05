//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName     :   GC_Socket
//  Version      :   1.0
//  Creater      :   michaelzhou
//  Date         :   2013-7-15   18:09:23
//  Comment    	 :   socket client package
//
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#ifdef WIN32
#include <winsock2.h>
#include <ws2def.h>
#include <windows.h>
#pragma comment(lib, "WS2_32.lib")
#else
#include <netdb.h>
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
#include <sys/select.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <time.h>
//#include <sys/epoll.h>
#endif

#define ERROR_CODE_SUCCESS 0


#define KG_PROCESS_ERROR(Condition) \
	do  \
	{   \
	if (!(Condition))   \
	goto Exit0;     \
	} while (false)


#define KG_PROCESS_SUCCESS(Condition) \
	do  \
	{   \
	if (Condition)      \
	goto Exit1;     \
	} while (false)


#define KG_PROCESS_ERROR_RET_CODE(Condition, Code) \
	do  \
	{   \
	if (!(Condition))       \
		{                       \
		nResult = Code;     \
		goto Exit0;         \
		}                       \
	} while (false)

/**
 * 网络数据流接口
 *
 */
struct IGC_SocketStream 
{
	virtual int SetTimeout(const timeval *pTimeout)  = 0;

	// return -1: error, 0: timeout, 1: success
	virtual int CheckCanSend(const timeval *pTimeout) = 0;

	// return -1: error, 0: timeout, 1: success, don't send a package > 65500 bytes
	virtual int Send(char *piBuffer, int nSize) = 0;

	// return -1: error, 0: timeout, 1: success
	virtual int CheckCanRecv(const timeval *pTimeout) = 0;

	// return -2: again, -1: error, 0: timeout, 1: success
	virtual int Recv(char **piRetBuffer, int *pRetSize)  = 0;

	virtual int IsAlive() = 0;

	virtual int GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort) = 0;

	virtual int SetUserData(void *pvUserData)      = 0;

	virtual void *GetUserData() = 0;

	virtual int GetLastError() = 0;
    
    virtual void CloseSocket() = 0;
};

/**
 * 网络数据流的生成器(创建连接)
 *
 */
class GC_SocketConnector
{
public:
	GC_SocketConnector();

	virtual IGC_SocketStream *Connect(
		const char cszIPAddress[],     int nPort
		);

protected:
	struct sockaddr_in m_BindLoaclAddr;
};