#include "SYMJGameScence.h"
#include "GameLib.h"

#include "CMD_SYMJ.h"
#include "SYMJGameLogic.h"
#include "SYMJPlayer.h"

bool SYMJGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_MJ_FREE)
	{
		OnFreeScence(data,dataSize);
		setGameState(HNMJ_STATE_NULL);
	}

	if (cbGameStatus == GS_MJ_PLAY)
	{
		OnPlayScence(data,dataSize);
		setGameState(HNMJ_STATE_PLAYING);
	}
	return true;
}
void SYMJGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	upSelfFreeReadState();
}
void SYMJGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;

	defaultState();

	m_iBankerUser=pNetInfo->wBankerUser;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		SYMJPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[i],pNetInfo->cbWeaveCount[i]);
		pPlayer->setHandCard(pNetInfo->cbCardData,13 - pNetInfo->cbWeaveCount[i]*3);
		for (BYTE m = 0;m<pNetInfo->cbDiscardCount[i];m++)
		{
			pPlayer->addHandOutCard(pNetInfo->cbDiscardCard[i][m]);
		}
		if (pPlayer == m_pLocal)
		{
//			m_pLocal->setHasCSGang(pNetInfo->bHasCSGang[i]);
		}

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
		setCurrentPlayer(pNetInfo->wCurrentUser,pNetInfo->cbActionMask,pNetInfo->cbActionCard);

		SYMJPlayer* pPlayer = getPlayerByChairID(m_iCurrentUser);
		if (pPlayer != m_pLocal && pPlayer)
		{
			pPlayer->addNewInCard(0);
		}
	}
	else if (pNetInfo->cbActionMask != WIK_NULL)
	{
		setCurrentPlayer(m_pLocal->GetChairID(),pNetInfo->cbActionMask,pNetInfo->cbActionCard);
	}
	if (pNetInfo->wOutCardUser != INVALID_WORD && pNetInfo->cbOutCardData)
	{
		SYMJPlayer* pPlayer = getPlayerByChairID(pNetInfo->wOutCardUser);
		pPlayer->showMJPoint();
	}

	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);
}
void SYMJGameScence::OnGFGameClose(int iExitCode)
{
	ExitGame();
}
void SYMJGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	if (pJieSuanNode->getActionByTag(1) == NULL)
	{
		upSelfFreeReadState();
	}
}
void SYMJGameScence::upSelfFreeReadState()
{
	if (!m_pLocal)
	{
		return;
	}
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}