#include "GamePlayer.h"

#include "GameLib.h"

GamePlayer::GamePlayer(IClientUserItem* pUserItem)
	:m_pUserItem(pUserItem)
{
}
GamePlayer::~GamePlayer()
{

}
void GamePlayer::setUserItem(IClientUserItem* pItem)
{
	m_pUserItem = pItem;
	upPlayerInfo();
}
IClientUserItem* GamePlayer::getUserItem(bool bAssert)
{
	CCAssert(m_pUserItem || !bAssert,"");
	return m_pUserItem;
}
word GamePlayer::GetTableID()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetTableID();
}
word GamePlayer::GetChairID()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetChairID();
}
byte GamePlayer::GetUserStatus()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetUserStatus();
}
SCORE GamePlayer::GetUserScore()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetUserScore();
}
word GamePlayer::GetFaceID()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetFaceID();
}
byte GamePlayer::GetGender()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetGender();
}
dword GamePlayer::GetUserID()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetUserID();
}
dword GamePlayer::GetGameID()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	return m_pUserItem->GetGameID();
}
dword GamePlayer::GetUserStarValue()
{
	if (!m_pUserItem)
	{
		return 0;
	}
	if (GetUserInfo()->iStartCout == 0)
	{
		return 0;
	}
	return ((float)GetUserInfo()->iStarValue/(float)GetUserInfo()->iStartCout+0.5f);
}
std::string GamePlayer::GetNickName()
{
	if (!m_pUserItem)
	{
		return "";
	}
	return utility::a_u8(m_pUserItem->GetNickName());
}
std::string GamePlayer::GetGPS()
{
	if (!m_pUserItem)
	{
		return "";
	}
	return m_pUserItem->GetUserInfo()->szGPS;
}
std::string GamePlayer::GetHeadHttp()
{
	if (!m_pUserItem)
	{
		return "";
	}
	return m_pUserItem->GetUserInfo()->szHeadHttp;
}

tagUserInfo * GamePlayer::GetUserInfo()
{
	if (!m_pUserItem)
	{
		return NULL;
	}
	return m_pUserItem->GetUserInfo();
}
