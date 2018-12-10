#include "GameBase.h"
#include "GamePlayer.h"

#include "GameLib.h"

#include "IServerItem.h"

GamePlayer* GameBase::getPoolPlayer(IClientUserItem * pIClientUserItem)
{
	GamePlayer* pPlayer = NULL;
	if (m_kPoolPlayer.size())
	{
		pPlayer = m_kPoolPlayer.back();
		m_kPoolPlayer.pop_back();
		pPlayer->setUserItem(pIClientUserItem);
	}
	return pPlayer;
}
GamePlayer* GameBase::CreatePlayer(IClientUserItem * pIClientUserItem)
{
	GamePlayer* pPlayer = getPoolPlayer(pIClientUserItem);
	if (pPlayer)
	{
		return pPlayer;
	}
	return new GamePlayer(pIClientUserItem);
}
void GameBase::DeletePlayer(GamePlayer* pPlayer)
{
	if (utility::haveInVector(m_kPoolPlayer,pPlayer))
	{
		return;
	}
	m_kPoolPlayer.push_back(pPlayer);
}
void GameBase::LocalPlayerEnter()
{
	CCAssert(m_pSelfPlayer,"");
	if (!m_pSelfPlayer)
	{
		return;
	}
	if (m_pSelfPlayer->GetUserStatus() < US_READY)
	{
		sendReady();
	}
}
void GameBase::upSelfPlayerInfo()
{

}
GamePlayer* GameBase::getBasePlayerByUserItem(IClientUserItem * pIClientUserItem)
{
	std::vector<GamePlayer*>::iterator itor = m_kPlayers.begin();
	for (;itor != m_kPlayers.end();itor++)
	{
		if ((*itor)->getUserItem() == pIClientUserItem)
		{
			return (*itor);
		}
	}
	return NULL;
}
GamePlayer* GameBase::getBasePlayerByChairID( word wChairID )
{
	std::vector<GamePlayer*>::iterator itor = m_kPlayers.begin();
	for (;itor != m_kPlayers.end();itor++)
	{
		if ((*itor)->GetChairID() == wChairID )
		{
			return (*itor);
		}
	}
	return NULL;
}
GamePlayer* GameBase::getBasePlayerByUserID( dword wUserID )
{
	std::vector<GamePlayer*>::iterator itor = m_kPlayers.begin();
	for (;itor != m_kPlayers.end();itor++)
	{
		if ((*itor)->GetUserID() == wUserID )
		{
			return (*itor);
		}
	}
	return NULL;
}

void GameBase::addGamePlayerToList(GamePlayer* pPlayer)
{
	CCAssert(pPlayer->getUserItem(),"");
	if (!haveInVectorByValue(m_kPlayers,pPlayer))
	{
		m_kPlayers.push_back(pPlayer);
	}
	pPlayer->PlayerEnter();
	pPlayer->upPlayerInfo();
	pPlayer->upPlayerState();
	m_kReqPlayerInfo.query(pPlayer->GetUserID());
}
void GameBase::removeGamePlayerToList(GamePlayer* pPlayer)
{
	if (m_pSelfPlayer == pPlayer)
	{
		m_pSelfPlayer = NULL;
	}
	removeVectorByValue(m_kPlayers,pPlayer);
	pPlayer->PlayerLeave();
	pPlayer->setUserItem(NULL);
}
GamePlayer* GameBase::getSelfGamePlayer()
{
	return m_pSelfPlayer;
}
word GameBase::getSelfChairID()
{
	if (m_pSelfPlayer == NULL)
	{
		return 0;
	}
	return m_pSelfPlayer->GetChairID();
}
bool GameBase::IsPrivateGame()
{
	return IServerItem::get()->GetServerAttribute().wServerType == GAME_GENRE_EDUCATE;
}
//用户进入
void GameBase::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getBasePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		m_kReqPlayerInfo.query(pIClientUserItem->GetUserID());
		pPlayer->upPlayerInfo();
		return;
	}
	IClientUserItem * pMeItem = IServerItem::get()->GetMeUserItem();
	if (!pMeItem)
	{
		return;
	}
	if (pMeItem->GetTableID() != pIClientUserItem->GetTableID())
	{
		return;
	}
	if (pIClientUserItem->GetUserStatus() == US_LOOKON)
	{
		return;
	}
	if (IServerItem::get()->GetMeUserItem() == pIClientUserItem)
	{
		CCAssert(m_pSelfPlayer == NULL,"");
		m_pSelfPlayer = CreatePlayer(pIClientUserItem);
		addGamePlayerToList(m_pSelfPlayer);
		LocalPlayerEnter();
		upSelfPlayerInfo();

		int iIdex = 0;
		while(true)
		{
			IClientUserItem* pTempUserItem = IServerItem::get()->GetTableUserItem(iIdex);
			iIdex++;
			if (!pTempUserItem)
			{
				break;
			}
			if (pTempUserItem->GetTableID() != pMeItem->GetTableID())
			{
				continue;
			}
			if (pTempUserItem->GetUserStatus() == US_LOOKON)
			{
				continue;
			}
			
			if (pTempUserItem == pIClientUserItem)
			{
				continue;
			}
			GamePlayer* pTempPlayer = CreatePlayer(pTempUserItem);
			addGamePlayerToList(pTempPlayer);
		}
	}
	else
	{
		if (m_pSelfPlayer)
		{
			GamePlayer* pTempPlayer = CreatePlayer(pIClientUserItem);
			addGamePlayerToList(pTempPlayer);
		}
	}
}
//用户离开
void GameBase::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getBasePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		removeGamePlayerToList(pPlayer);
		DeletePlayer(pPlayer);
	}
}
//用户积分
void GameBase::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getBasePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerInfo();
	}
	if (pPlayer == m_pSelfPlayer)
	{
		upSelfPlayerInfo();
	}
}
//用户状态
void GameBase::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getBasePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerState();
		OnEventUserStatus(pPlayer);
	}
}
//用户属性
void GameBase::OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getBasePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerInfo();
	}
	if (pPlayer == m_pSelfPlayer)
	{
		upSelfPlayerInfo();
	}
}
//用户头像
void GameBase::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getBasePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerInfo();
	}
}
void GameBase::onGPAccountInfo(CMD_GP_UserAccountInfo* pNetInfo)
{

}
void GameBase::onGPAccountInfoHttpIP(TAG_UserIndividual* pNetInfo)
{

	GamePlayer * pPlayer = getBasePlayerByUserID(pNetInfo->dwUserID);
	if (!pPlayer)
	{
		return;
	}
	IClientUserItem * pIClientUserItem = pPlayer->getUserItem(false);
	if (!pIClientUserItem)
	{
		return;
	}

	//获取用户
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();
	pUserInfo->iStarValue = pNetInfo->dwStarValue;
	pUserInfo->iStartCout = pNetInfo->dwStarCout;
	utility::StringToChar(pNetInfo->kIP,pUserInfo->szLogonIP,countarray(pUserInfo->szLogonIP));
	utility::StringToChar(pNetInfo->kHttp,pUserInfo->szHeadHttp,countarray(pUserInfo->szHeadHttp));
	utility::StringToChar(pNetInfo->kGPS,pUserInfo->szGPS,countarray(pUserInfo->szGPS));
	
	pPlayer->upPlayerInfo();
}