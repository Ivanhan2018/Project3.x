#include "CGPKefuMission.h"

CGPKefuMission::CGPKefuMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPKefuMissionSink = 0;
}

// 设置回调接口
void CGPKefuMission::setMissionSink(IGPKefuMissionSink* pIGPKefuMissionSink)
{
	mIGPKefuMissionSink = pIGPKefuMissionSink;
}

void CGPKefuMission::Kefu()
{
	start();
}

void CGPKefuMission::onEventTCPSocketLink()
{
	//发送数据
	send(MDM_GP_USER_SERVICE,SUB_GP_KEFU, 0, 0);
}

void CGPKefuMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPKefuMission::onEventTCPSocketShut\n");
}

void CGPKefuMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPKefuMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPKefuMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPKefuMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	switch (sub)
	{
		//兑换结果
	case SUB_GP_KEFU_RESULT:	return onSubKefuResult(data, size);
	}

	return false;
}

// 兑换结果
bool CGPKefuMission::onSubKefuResult(void* data, int size)
{
	//关闭连接
	stop();

	return true;
}
