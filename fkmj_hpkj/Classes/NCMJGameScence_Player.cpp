#include "NCMJGameScence.h"
#include "GameLib.h"
#include "NCMJPlayer.h"

NCMJPlayer* NCMJGameScence::getPlayerByChairID(int iChairID)
{
	if (!m_pLocal->getUserItem(false))
	{
		return m_pLocal;
	}
	int iIdex = (m_pLocal->GetChairID()-iChairID+MAX_PLAYER)%MAX_PLAYER;
	return m_pPlayer[iIdex];
}
GamePlayer* NCMJGameScence::CreatePlayer( IClientUserItem * pIClientUserItem )
{
	if (pIClientUserItem->GetUserID() == UserInfo::Instance().getUserID())
	{
		m_pLocal->setUserItem(pIClientUserItem);
		return m_pLocal;
	}
	else
	{
		GamePlayer* pPlayer = getPlayerByChairID(pIClientUserItem->GetChairID());
		pPlayer->setUserItem(pIClientUserItem);
		return pPlayer;
	}
	return NULL;
}
void NCMJGameScence::DeletePlayer(GamePlayer* pPlayer)
{
}
void NCMJGameScence::upSelfPlayerInfo()
{
	//WidgetFun::setText(this,"BJLSelfGold",getSelfGamePlayer()->GetUserScore());
}
