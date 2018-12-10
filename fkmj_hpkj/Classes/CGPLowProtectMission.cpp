#include "CGPLowProtectMission.h"

CGPLowProtectMission::CGPLowProtectMission(const char * url, int port)
: CCallMission("CGPLowProtectMission",url, port)
{
	mIGPLowProtectMissionSink = 0;
	addNetCall(CC_CALLBACK_2(CGPLowProtectMission::Net_requickLowProtectInfo,this),SUB_GP_BASEENSURE_PARAMETER);
	addNetCall(CC_CALLBACK_2(CGPLowProtectMission::Net_onObtainLowProtect,this),SUB_GP_BASEENSURE_RESULT);
}

void CGPLowProtectMission::setMissionSink(IGPLowProtectMissionLink* pIGPLowProtectMissionSink)
{
	mIGPLowProtectMissionSink = pIGPLowProtectMissionSink;
}

void CGPLowProtectMission::requickLowProtectInfo()
{
	addLinkCallFun(CC_CALLBACK_0(CGPLowProtectMission::CB_requickLowProtectInfo,this));
	start();
}
void CGPLowProtectMission::CB_requickLowProtectInfo()
{
	send(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_LOAD, NULL, 0);
}

void CGPLowProtectMission::obtainLowProtect()
{
	addLinkCallFun(CC_CALLBACK_0(CGPLowProtectMission::CB_obtainLowProtect,this));
	start();
}
void CGPLowProtectMission::CB_obtainLowProtect()
{
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
	CMD_GP_BaseEnsureTake kNetInfo;
	kNetInfo.dwUserID = pGlobalUserData->dwUserID;
	strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
	strncpy(kNetInfo.szMachineID,DF::shared()->GetMachineID(),countarray(kNetInfo.szMachineID));
	send(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_TAKE, &kNetInfo, sizeof(kNetInfo));
}


void CGPLowProtectMission::Net_requickLowProtectInfo(void * data, int size)
{
	PLAZZ_PRINTF("CGPLowProtectMission::onRequickLowProtectInfo");
	if (size != sizeof(CMD_GP_BaseEnsureParamter))
	{
		ASSERT(false);
		return ;
	}
	CMD_GP_BaseEnsureParamter* pNetInfo = (CMD_GP_BaseEnsureParamter*)data;

	stop();

	if (mIGPLowProtectMissionSink)
		mIGPLowProtectMissionSink->onLowProtectQueryInfoResult(pNetInfo->lScoreCondition, pNetInfo->lScoreAmount, pNetInfo->cbTakeTimes);

}

void CGPLowProtectMission::Net_onObtainLowProtect(void * data, int size)
{
	PLAZZ_PRINTF("CGPLowProtectMission::Net_onObtainLowProtect");

	CMD_GP_BaseEnsureResult* pNetInfo = (CMD_GP_BaseEnsureResult*)data;

	CGlobalUserInfo * pGlobalUserInfo   = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pUserData=pGlobalUserInfo->GetGlobalUserData();

	//更新
	if (pNetInfo->bSuccessed)
	{
		pUserData->lUserScore = pNetInfo->lGameScore;
		pGlobalUserInfo->upPlayerInfo();
	}

	if (mIGPLowProtectMissionSink)
		mIGPLowProtectMissionSink->onLowProtectDoneResult(pNetInfo->bSuccessed, pNetInfo->lGameScore, pNetInfo->szNotifyContent);

}
