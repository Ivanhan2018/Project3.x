#include "CGPAddRankMission.h"


CGPAddRankMission::CGPAddRankMission(const char* url, int port)
: CCallMission("CGPAddRankMission",url, port)
{
	mIGPAddRankMissionSink = 0;

	addNetCall(CC_CALLBACK_2(CGPAddRankMission::Net_RankAwardConfig,this),SUB_GP_ADDRANK_BACK_AWARD_INFO);
	addNetCall(CC_CALLBACK_2(CGPAddRankMission::Net_AddBank,this),SUB_GP_ADDRANK_BACK_RANK);
}

// 设置回调接口
void CGPAddRankMission::setMissionSink(CGAddRankMissionSink* pIGPKefuMissionSink)
{
	mIGPAddRankMissionSink = pIGPKefuMissionSink;
}

void CGPAddRankMission::GetRankAwardConfig()
{
	PLAZZ_PRINTF("CGPAddRankMission::GetRankAwardConfig\n");

	addLinkCallFun(CC_CALLBACK_0(CGPAddRankMission::CB_GetRankAwardConfig,this));

	start();
}
void CGPAddRankMission::CB_GetRankAwardConfig()
{
	PLAZZ_PRINTF("CGPAddRankMission::CB_GetRankAwardConfig\n");


	send(MDM_GP_USER_SERVICE,SUB_GP_ADDRANK_GET_AWARD_INFO,NULL,0);
}
void CGPAddRankMission::Net_RankAwardConfig(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPAddRankMission::Net_RankAwardConfig\n");

	ASSERT(dataSize == sizeof(CMD_GP_BackAddBankAwardInfo));
	CMD_GP_BackAddBankAwardInfo* pNetInfo = (CMD_GP_BackAddBankAwardInfo*)data;

	stop();

	if (mIGPAddRankMissionSink)
	{
		mIGPAddRankMissionSink->onGPAwardInfoResult(pNetInfo);
	}
}
void CGPAddRankMission::GetAddBank(int iRankIdex)
{
	PLAZZ_PRINTF("CGPAddRankMission::GetAddBank\n");

	addLinkCallStruct<int>(CC_CALLBACK_1(CGPAddRankMission::CB_GetAddBank,this),iRankIdex);

	start();
}
void CGPAddRankMission::CB_GetAddBank(int iRandIdex)
{
	CMD_GP_GetAddBank kNetInfo;
	kNetInfo.iRankIdex = iRandIdex;
	kNetInfo.dwUserID = CGlobalUserInfo::getUserID();
	strncpy(kNetInfo.szPassword,CGlobalUserInfo::getPassword(),countarray(kNetInfo.szPassword));
	send(MDM_GP_USER_SERVICE, SUB_GP_ADDRANK_GET_RANK,&kNetInfo,sizeof(CMD_GP_GetAddBank));

}
void CGPAddRankMission::Net_AddBank(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPAddRankMission::Net_AddBank\n");

	datastream kStream(data,dataSize);
	CMD_GP_BackAddBank kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	stop();

	if (mIGPAddRankMissionSink)
	{
		mIGPAddRankMissionSink->onGPBackAddBankResult(&kNetInfo);
	}
}
