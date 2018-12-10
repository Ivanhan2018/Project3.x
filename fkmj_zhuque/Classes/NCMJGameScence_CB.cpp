#include "NCMJGameScence.h"
#include "GameLib.h"

#include "CMD_NCMJ.h"
#include "NCMJGameLogic.h"
#include "NCMJPlayer.h"

bool NCMJGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_MJ_FREE)
	{
		OnFreeScence(data,dataSize);
		setGameState(NCMJ_STATE_NULL);
	}

	if (cbGameStatus == GS_MJ_PLAY)
	{
		OnPlayScence(data,dataSize);
		setGameState(NCMJ_STATE_PLAYING);
	}
	return true;
}
void NCMJGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() == US_FREE);
}
void NCMJGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;

	defaultState();

	m_iBankerUser=pNetInfo->wBankerUser;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		NCMJPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[i],pNetInfo->cbWeaveCount[i]);
		pPlayer->setHandCard(pNetInfo->cbCardData,13 - pNetInfo->cbWeaveCount[i]*3);
		for (BYTE m = 0;m<pNetInfo->cbDiscardCount[i];m++)
		{
			pPlayer->addHandOutCard(pNetInfo->cbDiscardCard[i][m]);
		}

	}
	if (pNetInfo->wOutCardUser != INVALID_WORD && pNetInfo->cbOutCardData)
	{
		NCMJPlayer* pPlayer = getPlayerByChairID(pNetInfo->wOutCardUser);
		pPlayer->showMJPoint();
	}

	m_pLocal->setHandCard(pNetInfo->cbCardData,pNetInfo->cbCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGame();
	}

	defaultPlayerActionState();
	if (pNetInfo->wCurrentUser != INVALID_WORD)
	{
		setCurrentPlayer(pNetInfo->wCurrentUser,pNetInfo->cbActionMask);

		NCMJPlayer* pPlayer = getPlayerByChairID(m_iCurrentUser);
		if (pPlayer != m_pLocal && pPlayer)
		{
			pPlayer->addNewInCard(0);
		}
	}
	else if (pNetInfo->cbActionMask != WIK_NULL)
	{
		setCurrentPlayer(m_pLocal->GetChairID(),pNetInfo->cbActionMask);
	}
	if (pNetInfo->wOutCardUser != INVALID_WORD && pNetInfo->cbOutCardData)
	{
		NCMJPlayer* pPlayer = getPlayerByChairID(pNetInfo->wOutCardUser);
		pPlayer->addHandOutCard(pNetInfo->cbOutCardData);
		pPlayer->setActOutCard(pNetInfo->cbOutCardData,false);
	}
	if (pNetInfo->cbActionCard != 0)
	{
		WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pNetInfo->cbActionCard));
	}

	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);
}
void NCMJGameScence::OnGFGameClose(int iExitCode)
{
	ExitGame();
}
void NCMJGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}