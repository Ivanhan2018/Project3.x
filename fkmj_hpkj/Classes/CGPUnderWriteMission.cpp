#include "CGPUnderWriteMission.h"

CGPUnderWriteMission::CGPUnderWriteMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPUnderWriteMissionSink = 0;
}

// 设置回调接口
void CGPUnderWriteMission::setMissionSink(IGPUnderWriteMissionSink* pIGPUnderWriteMissionSink)
{
	mIGPUnderWriteMissionSink = pIGPUnderWriteMissionSink;
}

// 个性签名
void CGPUnderWriteMission::modifyUnderWrite(char szUnderWrite[LEN_UNDER_WRITE])
{
	memcpy(mUnderWrite, szUnderWrite, sizeof(mUnderWrite));
	start();
}

void CGPUnderWriteMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPUnderWriteMission::onEventTCPSocketLink\n");

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

}

void CGPUnderWriteMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPUnderWriteMission::onEventTCPSocketShut\n");
}

void CGPUnderWriteMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPUnderWriteMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPUnderWriteMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPUnderWriteMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	switch (sub)
	{
		//操作成功
	case SUB_GP_OPERATE_SUCCESS:	return onSubOperateSuccess(data, size);
		//操作失败
	case SUB_GP_OPERATE_FAILURE:	return onSubOperateFailure(data, size);
	}

	return false;
}

// 操作成功
bool CGPUnderWriteMission::onSubOperateSuccess(void* data, int size)
{
	
	//关闭连接
	stop();

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置信息
	strncpy(pGlobalUserData->szUnderWrite,mUnderWrite,countarray(pGlobalUserData->szUnderWrite));

	//通知
	if (mIGPUnderWriteMissionSink)
		mIGPUnderWriteMissionSink->onGPUnderWriteSuccess("");
	return true;
}

// 操作失败
bool CGPUnderWriteMission::onSubOperateFailure(void* data, int size)
{
	//关闭连接
	stop();
	return true;
}