//
//  UserItem.cpp
//  Game
//
//  Created by zhouwei on 13-6-21.
//
//

#include "UserItem.h"

CUserItem::CUserItem()
{

}

CUserItem::CUserItem(tagUserData* tag)
{
	assert(tag);
	memcpy(&m_UserData,tag,sizeof(m_UserData));
}

CUserItem::~CUserItem()
{

}

//???????
unsigned int CUserItem::getUserPlayCount()
{
	return m_UserData.lWinCount+m_UserData.lLostCount+m_UserData.lDrawCount+m_UserData.lFleeCount;
}

void CUserItem::setUserScore(const tagMobileUserScore * pUserScore)
{
	m_UserData.lDrawCount  = pUserScore->dwDrawCount;
	m_UserData.lWinCount   = pUserScore->dwWinCount;
	m_UserData.lFleeCount  = pUserScore->dwFleeCount;
	m_UserData.lLostCount  = pUserScore->dwLostCount;

	m_UserData.lScore      = pUserScore->lScore;
	m_UserData.lExperience      = pUserScore->dwExperience;
}

//??????? ???????????
void  CUserItem::setUserScore(const tagUserScore * pUserScore)
{
	//งน?????
	//ASSERT(m_bActive==true);
	assert(pUserScore != NULL);

	//???????
	/*m_UserData.wSortID     = pUserScore->wSortID;
	m_UserData.lScore      = pUserScore->lScore;
	m_UserData.lWinCount   = pUserScore->lWinCount;
	m_UserData.lLostCount  = pUserScore->lLostCount;
	m_UserData.lDrawCount  = pUserScore->lDrawCount;
	m_UserData.lFleeCount  = pUserScore->lFleeCount;
	m_UserData.lExperience =pUserScore->lExperience;
	m_UserData.lGoldEggs   =pUserScore->lGoldEggs;*/
	return;
}
//???y??
void  CUserItem::setUserGold(const tagUserGold * pUserGold)
{
	assert(pUserGold!=NULL);

	//???????
	m_UserData.lScore=pUserGold->lScore;
	m_UserData.lGoldEggs=pUserGold->lGoldEggs;
	m_UserData.lInsureScore=pUserGold->lStorageGold;
	m_UserData.lExperience=pUserGold->lExperience;

	return;
}
//????????
void  CUserItem::setUserSort(const unsigned int nSortID, const EN_MatchStatus enMatchStatus)
{
	m_UserData.wSortID = nSortID;
	return;
}

//???????????
void  CUserItem::setUserMatchScore(const unsigned int nMatchScore)
{
	m_UserData.lMatchScore=nMatchScore;
	return;
}

//??????
void  CUserItem::setUserStatus(const tagUserStatus* pUserStatus)
{
	//งน?????
	//ASSERT(m_bActive==true);
	assert(pUserStatus!=NULL);

	//???????
	//m_UserData.wTeamID=pUserStatus->wTeamID;
	m_UserData.wTableID=pUserStatus->wTableID;
	m_UserData.wChairID=pUserStatus->wChairID;
	m_UserData.cbUserStatus=pUserStatus->cbUserStatus;

	return;
}

//???????????
void CUserItem::setUserDate(tagUserData* tag)
{
	if (tag->lScore == 0)
	{
		CCLOG("0");
	}
	memcpy(&m_UserData, tag, sizeof(m_UserData));
}

//????????
void CUserItem::cleanData()
{
	//m_bActive=false;
	memset(&m_UserData,0,sizeof(m_UserData));
	//m_UserData.wTeamID=INVALID_TEAM; 2010.9.17 by gaoshan
	m_UserData.wTableID=INVALID_TABLE;
	m_UserData.wChairID=INVALID_CHAIR;
}