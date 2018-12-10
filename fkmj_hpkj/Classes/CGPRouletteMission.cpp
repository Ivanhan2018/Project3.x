#include "CGPRouletteMission.h"


CGPRouletteMission::CGPRouletteMission(const char* url, int port)
: CCallMission("CGUserInGameServerInfo",url, port)
,m_pRouletteMissionSink(NULL)
{
	addNetCall(CC_CALLBACK_2(CGPRouletteMission::Net_RouletteUeserInfo,this),SUB_GP_ROULETTE_USERINFO);
	addNetCall(CC_CALLBACK_2(CGPRouletteMission::Net_RouletteDone,this),SUB_GP_ROULETTE_DONE);
}

CGPRouletteMission::~CGPRouletteMission()
{

}
// 设置回调接口
void CGPRouletteMission::setMissionSink(ICGPRouletteMissionSink* pSink)
{
	m_pRouletteMissionSink = pSink;
}

void CGPRouletteMission::PerformRouletteUeserInfo(int iUserID,int iVaildDay)
{
	addLinkCallFun(CC_CALLBACK_0(CGPRouletteMission::CB_RouletteUeserInfo,this,iUserID,iVaildDay));
	start();
}
void CGPRouletteMission::CB_RouletteUeserInfo(int iUserID,int iVaildDay)
{
	CMD_GP_ReqRouletteUeserInfo kNetInfo;
	kNetInfo.dwUserID = iUserID;
	kNetInfo.dwVaildDay = iVaildDay;
	send(MDM_GP_USER_SERVICE,SUB_GP_ROULETTE_USERINFO,&kNetInfo,sizeof(kNetInfo));
}
void CGPRouletteMission::Net_RouletteUeserInfo(void* data, int dataSize)
{
	ASSERT(dataSize==sizeof(CMD_GP_BackRouletteUeserInfo));
	if (dataSize!=sizeof(CMD_GP_BackRouletteUeserInfo)) return ;
	
	CMD_GP_BackRouletteUeserInfo* pNetInfo = (CMD_GP_BackRouletteUeserInfo*)data;

	if (m_pRouletteMissionSink)
	{
		m_pRouletteMissionSink->onRouletteUeserInfo(pNetInfo);
	}
	stop();
}

void CGPRouletteMission::PerformRouletteDone(int iUserID)
{
	addLinkCallFun(CC_CALLBACK_0(CGPRouletteMission::CB_RouletteDone,this,iUserID));
	start();
}
void CGPRouletteMission::CB_RouletteDone(int iUserID)
{
	cocos2d::log("CGPRouletteMission::CB_RouletteDone");

	CMD_GP_ReqRouletteDone kNetInfo;
	kNetInfo.dwUserID = iUserID;
	send(MDM_GP_USER_SERVICE,SUB_GP_ROULETTE_DONE,&kNetInfo,sizeof(kNetInfo));
}
void CGPRouletteMission::Net_RouletteDone(void* data, int dataSize)
{
	cocos2d::log("CGPRouletteMission::Net_RouletteDone");
	ASSERT(dataSize==sizeof(CMD_GP_BackRouletteDone));
	if (dataSize!=sizeof(CMD_GP_BackRouletteDone)) return ;

	CMD_GP_BackRouletteDone* pNetInfo = (CMD_GP_BackRouletteDone*)data;

	if (m_pRouletteMissionSink)
	{
		m_pRouletteMissionSink->onRouletteDone(pNetInfo);
	}
	stop();
}