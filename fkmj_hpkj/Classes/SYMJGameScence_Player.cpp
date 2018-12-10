#include "SYMJGameScence.h"
#include "GameLib.h"
#include "SYMJPlayer.h"

SYMJPlayer* SYMJGameScence::getPlayerByChairID(int iChairID)
{
	if (!m_pLocal->getUserItem(false))
	{
		return m_pLocal;
	}
	int iIdex = (m_pLocal->GetChairID()-iChairID+MAX_PLAYER)%MAX_PLAYER;
	return m_pPlayer[iIdex];
}
GamePlayer* SYMJGameScence::CreatePlayer( IClientUserItem * pIClientUserItem )
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
void SYMJGameScence::DeletePlayer(GamePlayer* pPlayer)
{
}
void SYMJGameScence::upSelfPlayerInfo()
{
	WidgetFun::getChildWidget(this,"TimeNodeChild")->setRotation(-90*(m_pLocal->GetChairID()));
	//WidgetFun::setText(this,"BJLSelfGold",getSelfGamePlayer()->GetUserScore());
}
