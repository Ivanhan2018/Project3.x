#include "CGMatchMission.h"


CGMatchMission::CGMatchMission(const char* url, int port)
: CCallMission("CGMatchMission",url, port)
{
	m_pMatchSink = 0;

	addNetCall(CC_CALLBACK_2(CGMatchMission::Net_SignupResult,this),SUB_GP_MATCH_SIGNUP_RESULT);
	addNetCall(CC_CALLBACK_2(CGMatchMission::Net_MatchAward,this),SUB_GP_MATCH_AWARD_LIST);
}

CGMatchMission::~CGMatchMission()
{

}
// 设置回调接口
void CGMatchMission::setMissionSink(ICGMatchMissionSink* pSink)
{
	m_pMatchSink = pSink;
}

void CGMatchMission::PerformSignupMatch(int iGamerServerID)
{
	PLAZZ_PRINTF("CGMatchMission::PerformSignupMatch\n");

	addLinkCallFun(CC_CALLBACK_0(CGMatchMission::CB_SignupMatch,this));

	//设置变量
	CServerListData * pServerListData=CServerListData::shared();
	CGameServerItem * pServerItem = pServerListData->SearchGameServer(iGamerServerID);
	if (pServerItem)
	{
		//设置变量
		m_bSignup=true;

		m_pGameMatchInfo=pServerItem->m_GameMatch;

		start();
	}
	else
	{
		CCASSERT(false,"CGMatchMission::PerformSignupMatch");
	}
}
void CGMatchMission::PerformUnSignupMatch(int iGamerServerID)
{
	PLAZZ_PRINTF("CGMatchMission::PerformUnSignupMatch\n");

	addLinkCallFun(CC_CALLBACK_0(CGMatchMission::CB_SignupMatch,this));

	CServerListData * pServerListData=CServerListData::shared();
	CGameServerItem * pServerItem = pServerListData->SearchGameServer(iGamerServerID);
	if (pServerItem)
	{
		//设置变量
		m_bSignup=false;

		m_pGameMatchInfo=pServerItem->m_GameMatch;

		start();
	}
	else
	{
		CCASSERT(false,"CGMatchMission::PerformSignupMatch");
	}
}
void CGMatchMission::CB_SignupMatch()
{
	PLAZZ_PRINTF("CGMatchMission::CB_SignupMatch\n");
	//报名比赛
	if(m_bSignup==true)
	{
		//获取对象
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		//构造结果
		CMD_GP_MatchSignup MatchSignup;

		//比赛信息
		MatchSignup.wServerID = m_pGameMatchInfo.wServerID;
		MatchSignup.dwMatchID = m_pGameMatchInfo.dwMatchID;
		MatchSignup.dwMatchNO = m_pGameMatchInfo.dwMatchNO;

		//个人信息
		MatchSignup.dwUserID = pGlobalUserData->dwUserID;
		strcpy(MatchSignup.szPassword,pGlobalUserData->szPassword);

		send(MDM_GP_USER_SERVICE,SUB_GP_MATCH_SIGNUP,&MatchSignup,sizeof(MatchSignup));

		return;
	}

	//取消比赛
	if(m_bSignup==false)
	{
		//获取对象
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		//构造结果
		CMD_GP_MatchUnSignup MatchUnSignup;

		//比赛信息
		MatchUnSignup.wServerID = m_pGameMatchInfo.wServerID;
		MatchUnSignup.dwMatchID = m_pGameMatchInfo.dwMatchID;
		MatchUnSignup.dwMatchNO = m_pGameMatchInfo.dwMatchNO;

		//个人信息
		MatchUnSignup.dwUserID = pGlobalUserData->dwUserID;
		strcpy(MatchUnSignup.szPassword,pGlobalUserData->szPassword);

		send(MDM_GP_USER_SERVICE,SUB_GP_MATCH_UNSIGNUP,&MatchUnSignup,sizeof(MatchUnSignup));

		return;
	}
}
void CGMatchMission::Net_SignupResult(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGMatchMission::Net_MatchAward");

	//提取数据
	CMD_GP_MatchSignupResult * pMatchSignupResult = (CMD_GP_MatchSignupResult *)data;

	//获取对象
	CServerListData * pServerListData=CServerListData::shared();

	//查找房间
	CGameServerItem * pGameServerItem = pServerListData->SearchGameServer(m_pGameMatchInfo.wServerID);
	if(pGameServerItem==NULL) return ;

	////设置标识
	if(pMatchSignupResult->bSuccessed==true)
	{
		pGameServerItem->m_bSignuped = pMatchSignupResult->bSignup;
	}
	if (m_pMatchSink)
	{
		m_pMatchSink->onSignUpResoult(m_pGameMatchInfo,pMatchSignupResult->bSuccessed,pMatchSignupResult->bSignup,pMatchSignupResult->szDescribeString);
	}

	stop();
}

//取消报名
void CGMatchMission::PerformMatchAward(int iGamerServerID)
{
	addLinkCallFun(CC_CALLBACK_0(CGMatchMission::CB_MatchAward,this));

	CServerListData * pServerListData=CServerListData::shared();
	CGameServerItem * pServerItem = pServerListData->SearchGameServer(iGamerServerID);
	if (pServerItem)
	{

		m_pGameMatchInfo=pServerItem->m_GameMatch;

		start();
	}
	else
	{
		CCASSERT(false,"CGMatchMission::PerformMatchAward");
	}
}
void CGMatchMission::CB_MatchAward()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	CMD_GP_MatchGetAward MatchGetAward;

	//比赛信息
	MatchGetAward.dwUserID = pGlobalUserData->dwUserID;
	MatchGetAward.dwMatchID = m_pGameMatchInfo.dwMatchID;
	MatchGetAward.dwMatchNO = m_pGameMatchInfo.dwMatchNO;

	send(MDM_GP_USER_SERVICE,SUB_GP_MATCH_AWARD_LIST,&MatchGetAward,sizeof(MatchGetAward));
}
void CGMatchMission::Net_MatchAward(void* data, int dataSize)
{
	datastream kDataStream(data,dataSize);
	CMD_GR_MatchAwardList kNetInfo;
	kNetInfo.StreamValue(kDataStream,false);

	if (m_pMatchSink)
	{
		m_pMatchSink->onMatchAwardList(&kNetInfo);
	}
}
