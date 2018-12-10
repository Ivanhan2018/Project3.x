#include "CGPShopInfoMission.h"
#include "NoticeMsg.h"
#include "UserInfo.h"

CGPShopInfoMission::CGPShopInfoMission(const char* url, int port)
: CCallMission("CGPShopInfoMission",url, port)
{
	mIGPShopInfoMissionSink = 0;

	addNetCall(CC_CALLBACK_2(CGPShopInfoMission::Net_ShopInfoList,this),SUB_GP_SHOPINFO_BACK);
	addNetCall(CC_CALLBACK_2(CGPShopInfoMission::Net_OperateSucess,this),SUB_GP_OPERATE_SUCCESS);
	addNetCall(CC_CALLBACK_2(CGPShopInfoMission::Net_OperateFailure,this),SUB_GP_OPERATE_FAILURE);
}

// 设置回调接口
void CGPShopInfoMission::setMissionSink(CGShopInfoMissionSink* pIGPKefuMissionSink)
{
	mIGPShopInfoMissionSink = pIGPKefuMissionSink;
}

void CGPShopInfoMission::GetShopInfoList()
{
	addLinkCallFun(CC_CALLBACK_0(CGPShopInfoMission::CB_GetShopInfoList,this));
	start();
}

void CGPShopInfoMission::CB_GetShopInfoList()
{
	CMD_GP_GetShopInfo kNetInfo;
	kNetInfo.dwUserID = CGlobalUserInfo::getUserID();
	strncpy(kNetInfo.szPassword,CGlobalUserInfo::getPassword(),countarray(kNetInfo.szPassword));
	send(MDM_GP_USER_SERVICE, SUB_GP_SHOPINFO_GET_LIST_INFO,&kNetInfo,sizeof(CMD_GP_GetShopInfo));
}

void CGPShopInfoMission::Net_ShopInfoList( void* data, int dataSize )
{
	PLAZZ_PRINTF("CGPShopInfoMission::Net_ShopInfoList\n");

	datastream kStream(data,dataSize);
	CMD_GP_BackShopInfo kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	stop();

	if (mIGPShopInfoMissionSink)
	{
		mIGPShopInfoMissionSink->onGPShopInfoListResult(&kNetInfo);
	}
}

void CGPShopInfoMission::UpShopOder(CMD_GP_UpShopOder kOder)
{
	m_kShopUpOder = kOder;

	addLinkCallFun(CC_CALLBACK_0(CGPShopInfoMission::CB_UpShopOder,this));
	start();
}
void CGPShopInfoMission::CB_UpShopOder()
{
	datastream kData;
	m_kShopUpOder.StreamValue(kData,true);
	send(MDM_GP_USER_SERVICE, SUB_GP_SHOPINFO_BUYODER,&kData[0],kData.size());
}
void CGPShopInfoMission::Net_OperateSucess(void* data, int dataSize)
{
	CMD_GP_OperateSuccess* pNetInfo = (CMD_GP_OperateSuccess*)data;
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pNetInfo->szDescribeString));

	UserInfo::Instance().reqAccountInfo();
	stop();
}
void CGPShopInfoMission::Net_OperateFailure(void* data, int dataSize)
{
	CMD_GP_OperateFailure* pNetInfo = (CMD_GP_OperateFailure*)data;
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pNetInfo->szDescribeString));

	stop();
}