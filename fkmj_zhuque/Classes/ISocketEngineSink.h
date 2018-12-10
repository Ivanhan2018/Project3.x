#ifndef _ISocketEngineSink_
#define _ISocketEngineSink_

struct ISocketEngineSink
{
	virtual ~ISocketEngineSink(){}
	virtual void onEventTCPSocketLink() = 0;
	virtual void onEventTCPSocketShut() = 0;
	virtual void onEventTCPSocketError(int errorCode) = 0;
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize) = 0;
	virtual bool onEventTCPHeartTick(){return true;};
}; // ISocketEngineSink

#endif // _ISocketEngineSink_