#include <iostream>
#include "CSocketMission.h"

CSocketMission::CSocketMission(const char* url, int port)
{
	mSocketEngine = ISocketEngine::create();
	mSocketEngine->setTCPValidate(true);///
	mSocketEngine->setSocketEngineSink(this);

	setUrl(url, port);
}

CSocketMission::~CSocketMission()
{
	mSocketEngine->setSocketEngineSink(0);
	ISocketEngine::destory(mSocketEngine);
	mSocketEngine = 0;
}

void CSocketMission::setUrl(const char* url, int port)
{
	if (url != 0)
		sprintf(mUrl, "%s", url);
	else
		mUrl[0]=0;
	mPort = port;
}

bool CSocketMission::start()
{
	if (isAlive())
	{
		onEventTCPSocketLink();
		return true;
	}
	else
	{
		return mSocketEngine->connect(mUrl, mPort);
	}
	return false;
}


void CSocketMission::stop()
{
	mSocketEngine->disconnect();
}

bool CSocketMission::isAlive() const
{
	return mSocketEngine->isAlive();
}

bool CSocketMission::send(int main, int sub)
{
	return send(main, sub, 0, 0);
}

bool CSocketMission::send(int main, int sub, void* data, int size)
{
	return mSocketEngine->send(main, sub, data, size);
}