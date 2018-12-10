#ifndef _Socket_H_
#define _Socket_H_

#include <list>
#include "MLClientNetDelegate.h"

struct ISocketSink
{
	virtual ~ISocketSink(){}
	virtual void onSocketLink() = 0;
	virtual void onSocketShut() = 0;
	virtual void onSocketTimeOut() = 0;
	virtual void onSocketError(int errorCode) = 0;
	virtual void onSocketData(void* data, int dataSize) = 0;
	
};

class CSocket
	:public cocos2d::MLClientNetDelegate
{
public:
	CSocket();
	~CSocket();
	void setSocketSink(ISocketSink* pISocketSink);
	bool isAlive();
	bool connect(const char* url, int port);
	void disconnect();
	int send(const char* data, int size);
	//////////////////////////////////////////////////////////////////////////
	// 辅助函数
private:
	virtual void onConnected();
	virtual void onConnectTimeout();
	virtual void onDisconnected();
	virtual void onExceptionCaught(CCSocketStatus eStatus);
	virtual void onMessageReceived(char* pData,unsigned int iLenght);


	ISocketSink*	mSocketSink;
};

#endif // _Socket_H_