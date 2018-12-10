#include "CGPublicNoticMission.h"


CGPublicNoticMission::CGPublicNoticMission(const char* url, int port)
: CCallMission("CGPublicNoticMission",url, port)
{
	mIGPKefuMissionSink = 0;
	addNetCall(CC_CALLBACK_2(CGPublicNoticMission::Net_PublicNotice,this),SUB_GP_PUBLIC_NOTICE);
	addNetCall(CC_CALLBACK_2(CGPublicNoticMission::Net_OperateFailure,this),SUB_GP_OPERATE_FAILURE);
}

// 设置回调接口
void CGPublicNoticMission::setMissionSink(CGPublicNoticMissionSink* pIGPKefuMissionSink)
{
	mIGPKefuMissionSink = pIGPKefuMissionSink;
}

void CGPublicNoticMission::PublicNotice(std::string kKey)
{
	addLinkCallFun(CC_CALLBACK_0(CGPublicNoticMission::CB_PublicNotice,this,kKey));

	start();
}

void CGPublicNoticMission::CB_PublicNotice(std::string kKey)
{
	CMD_GP_QueryNotice kNetInfo;
	strncpy(kNetInfo.szKeyName,kKey.c_str(),countarray(kNetInfo.szKeyName));
	//发送数据
	m_kKeyName = kKey;
	send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_PUBLIC_NOTICE, &kNetInfo,sizeof(kNetInfo));
}

void CGPublicNoticMission::Net_PublicNotice(void* data, int dataSize)
{

	CMD_GP_PublicNotice* pNetInfo = (CMD_GP_PublicNotice*)data;
	if (mIGPKefuMissionSink)
	{
		mIGPKefuMissionSink->onGPPublicNoticeResult(m_kKeyName,pNetInfo->lResultCode,pNetInfo->szDescribeString);
	}
	stop();
}

void CGPublicNoticMission::Net_OperateFailure(void* data, int dataSize)
{
	CMD_GP_OperateFailure* pNetInfo = (CMD_GP_OperateFailure*)data;
	if (mIGPKefuMissionSink)
	{
		mIGPKefuMissionSink->onGPPublicNoticeResult(m_kKeyName,pNetInfo->lResultCode,pNetInfo->szDescribeString);
	}
	stop();
}

