#include "CGPExchangeMission.h"


CGPExchangeMission::CGPExchangeMission(const char* url, int port)
: CCallMission("CGPExchangeHuaFeiMission",url, port)
{
	mIGPExchangeHuaFeiMissionSink = 0;

	addNetCall(CC_CALLBACK_2(CGPExchangeMission::Net_ExchangeHuaFeiList,this),SUB_GP_EXCHANGEHUAFEI_BACK);
	addNetCall(CC_CALLBACK_2(CGPExchangeMission::Net_ExchangeItemSucess,this),SUB_GP_OPERATE_SUCCESS);
	addNetCall(CC_CALLBACK_2(CGPExchangeMission::Net_ExchangeItemFail,this),SUB_GP_OPERATE_FAILURE);
}

// 设置回调接口
void CGPExchangeMission::setMissionSink(CGExchangeMissionSink* pIGPKefuMissionSink)
{
	mIGPExchangeHuaFeiMissionSink = pIGPKefuMissionSink;
}

void CGPExchangeMission::GetExchangeList()
{
	addLinkCallFun(CC_CALLBACK_0(CGPExchangeMission::CB_ExchangeList,this));
	start();
}

void CGPExchangeMission::CB_ExchangeList()
{
	CMD_GP_GetExchangeHuaFei kNetInfo;
	kNetInfo.dwUserID = CGlobalUserInfo::getUserID();
	strncpy(kNetInfo.szPassword,CGlobalUserInfo::getPassword(),countarray(kNetInfo.szPassword));
	send(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO,&kNetInfo,sizeof(CMD_GP_GetExchangeHuaFei));
}

void CGPExchangeMission::Net_ExchangeHuaFeiList( void* data, int dataSize )
{
	PLAZZ_PRINTF("CGPExchangeHuaFeiMission::Net_ExchangeHuaFeiList\n");

	datastream kStream(data,dataSize);
	CMD_GP_BackExchangeHuaFei kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	stop();

	if (mIGPExchangeHuaFeiMissionSink)
	{
		mIGPExchangeHuaFeiMissionSink->onGPExchangeHuaFeiListResult(&kNetInfo);
	}
}

void CGPExchangeMission::ExchangeItem(int iExchangeID)
{
	addLinkCallFun(CC_CALLBACK_0(CGPExchangeMission::CB_ExchangeItem,this,iExchangeID));
	start();
}
void CGPExchangeMission::CB_ExchangeItem(int iExchangeID)
{

	CMD_GP_ExchangeDone kNetInfo;
	kNetInfo.dwUserID = CGlobalUserInfo::getUserID();
	kNetInfo.dwExchangeID = iExchangeID;
	strncpy(kNetInfo.szPassword,CGlobalUserInfo::getPassword(),countarray(kNetInfo.szPassword));
	send(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_DONE,&kNetInfo,sizeof(CMD_GP_ExchangeDone));
}
void CGPExchangeMission::Net_ExchangeItemSucess(void* data, int dataSize)
{
	//变量定义
	CMD_GP_OperateSuccess * pNetInfo=(CMD_GP_OperateSuccess *)data;

	if (mIGPExchangeHuaFeiMissionSink)
	{
		mIGPExchangeHuaFeiMissionSink->onGPExchangeDoneResult(true,pNetInfo->szDescribeString);
	}
}
void CGPExchangeMission::Net_ExchangeItemFail(void* data, int dataSize)
{
	//变量定义
	CMD_GP_OperateFailure * pNetInfo=(CMD_GP_OperateFailure *)data;

	if (mIGPExchangeHuaFeiMissionSink)
	{
		mIGPExchangeHuaFeiMissionSink->onGPExchangeDoneResult(false,pNetInfo->szDescribeString);
	}
}