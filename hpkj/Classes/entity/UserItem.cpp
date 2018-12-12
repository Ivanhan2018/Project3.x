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

void  CUserItem::setUserScore(const tagUserScore * pUserScore)
{
	assert(pUserScore != NULL);

	return;
}

void  CUserItem::setUserGold(const tagUserGold * pUserGold)
{
	assert(pUserGold!=NULL);

	m_UserData.lScore=pUserGold->lScore;
	m_UserData.lGoldEggs=pUserGold->lGoldEggs;
	m_UserData.lInsureScore=pUserGold->lStorageGold;
	m_UserData.lExperience=pUserGold->lExperience;

	return;
}

void  CUserItem::setUserSort(const unsigned int nSortID, const EN_MatchStatus enMatchStatus)
{
	m_UserData.wSortID = nSortID;
	return;
}

void  CUserItem::setUserMatchScore(const unsigned int nMatchScore)
{
	m_UserData.lMatchScore=nMatchScore;
	return;
}

void  CUserItem::setUserStatus(const tagUserStatus* pUserStatus)
{
	assert(pUserStatus!=NULL);

	m_UserData.wTableID=pUserStatus->wTableID;
	m_UserData.wChairID=pUserStatus->wChairID;
	m_UserData.cbUserStatus=pUserStatus->cbUserStatus;

	return;
}

void CUserItem::setUserDate(tagUserData* tag)
{
	memcpy(&m_UserData, tag, sizeof(m_UserData));
}

void CUserItem::cleanData()
{
	memset(&m_UserData,0,sizeof(m_UserData));
	m_UserData.wTableID=INVALID_TABLE;
	m_UserData.wChairID=INVALID_CHAIR;
}