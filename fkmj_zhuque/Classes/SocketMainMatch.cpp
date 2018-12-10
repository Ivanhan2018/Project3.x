#include "CServerItem.h"
//比赛消息
bool CServerItem::OnSocketMainMatch(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	//费用查询
	case SUB_GR_MATCH_FEE:		 return OnSocketSubMatchFee(data,dataSize);
	case SUB_GR_MATCH_NUM:		 return OnSocketSubMatchNum(data,dataSize);
	case SUB_GR_MATCH_INFO:		 return OnSocketSubMatchInfo(data, dataSize);
	case SUB_GR_MATCH_WAIT_TIP:	 return OnSocketSubWaitTip(data, dataSize);
	case SUB_GR_MATCH_RESULT:	 return OnSocketSubMatchResult(data, dataSize);
	case SUB_GR_MATCH_STATUS:	 return OnSocketSubMatchStatus(data, dataSize);
	case SUB_GR_MATCH_GOLDUPDATE:return OnSocketSubMatchGoldUpdate(data, dataSize);
	case SUB_GR_MATCH_ELIMINATE: return OnSocketSubMatchEliminate(data, dataSize);
	case SUB_GR_MATCH_JOIN_RESOULT: return OnSocketSubMatchJoinResoult(data, dataSize);
	}
	return true;
}

void CServerItem::sendMacthFree()
{
	SendSocketData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&m_kMacthCost.lMatchFee,sizeof(m_kMacthCost.lMatchFee));
}
void CServerItem::sendExitMacth()
{
	SendSocketData(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH);
}
//比赛费用
bool CServerItem::OnSocketSubMatchFee(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchFee\n");

	CMD_GR_Match_Fee *pNetInfo = (CMD_GR_Match_Fee*)data;

	m_kMacthCost = *pNetInfo;

	if(pNetInfo->lMatchFee>0)
	{
		if (mIServerMatchSink)
		{
			mIServerMatchSink->OnSocketSubMatchFee(pNetInfo);
		}
	}

	return true;
}

//参赛人数
bool CServerItem::OnSocketSubMatchNum(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchNum\n");

	ASSERT(dataSize==sizeof(CMD_GR_Match_Num));
	if(dataSize!=sizeof(CMD_GR_Match_Num)) return false;

	CMD_GR_Match_Num *pNetInfo = (CMD_GR_Match_Num*)data;

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchNum(pNetInfo);
	}

	return true;
}
bool CServerItem::OnSocketSubMatchInfo(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchInfo\n");

	ASSERT(dataSize==sizeof(CMD_GR_Match_Info));
	if(dataSize!=sizeof(CMD_GR_Match_Info)) return false;

	CMD_GR_Match_Info* pNetInfo = (CMD_GR_Match_Info*)data;

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchInfo(pNetInfo);
	}

	return true;
}
bool CServerItem::OnSocketSubWaitTip(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubWaitTip\n");

	ASSERT(dataSize==sizeof(CMD_GR_Match_Wait_Tip) || dataSize==0);
	if(dataSize!=sizeof(CMD_GR_Match_Wait_Tip) && dataSize!=0) return false;

	if (mIServerMatchSink)
	{
		if (dataSize != 0)
		{
			CMD_GR_Match_Wait_Tip* pNetInfo = (CMD_GR_Match_Wait_Tip*)data;
			mIServerMatchSink->OnSocketSubMatchWaitTip(true,pNetInfo);
		}
		else
		{
			mIServerMatchSink->OnSocketSubMatchWaitTip(true,NULL);
		}
	}

	return true;
}
bool CServerItem::OnSocketSubMatchResult(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchResult\n");

	ASSERT(dataSize==sizeof(CMD_GR_MatchResult));
	if(dataSize!=sizeof(CMD_GR_MatchResult)) return false;

	CMD_GR_MatchResult *pNetInfo = (CMD_GR_MatchResult*)data;

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchResult(pNetInfo);
	}

	return true;
}
bool CServerItem::OnSocketSubMatchStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchStatus\n");

	ASSERT(dataSize==sizeof(byte));
	if(dataSize!=sizeof(byte)) return false;

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchStatus(*(byte*)data);
	}

	return true;
}
bool CServerItem::OnSocketSubMatchGoldUpdate(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchGoldUpdate\n");

	ASSERT(dataSize==sizeof(CMD_GR_MatchGoldUpdate));
	if(dataSize!=sizeof(CMD_GR_MatchGoldUpdate)) return false;

	CMD_GR_MatchGoldUpdate *pNetInfo = (CMD_GR_MatchGoldUpdate*)data;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	pGlobalUserData->lUserScore = pNetInfo->lCurrGold;

	pGlobalUserInfo->upPlayerInfo();

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchGoldUpdate(pNetInfo);
	}

	return true;
}
bool CServerItem::OnSocketSubMatchEliminate(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchEliminate\n");

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchEliminate();
	}

	return true;
}
bool CServerItem::OnSocketSubMatchJoinResoult(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubMatchJoinResoult\n");

	ASSERT(dataSize==sizeof(CMD_GR_Match_JoinResoult));
	if(dataSize!=sizeof(CMD_GR_Match_JoinResoult)) return false;

	CMD_GR_Match_JoinResoult *pNetInfo = (CMD_GR_Match_JoinResoult*)data;

	if (mIServerMatchSink)
	{
		mIServerMatchSink->OnSocketSubMatchJoinResolt((bool)pNetInfo->wSucess);
	}
	return true;
}
