#include "NNGameScence.h"
#include "GameLib.h"

#include "CMD_Ox.h"
#include "NNGameLogic.h"
#include "NNPlayer.h"

bool NNGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_TK_FREE)
	{
		OnFreeScence(data,dataSize);
	}
	if (cbGameStatus == GS_TK_CALL)
	{
		OnCallBankerScence(data,dataSize);
	}
	if (cbGameStatus == GS_TK_SCORE)
	{
		OnBetScoreScence(data,dataSize);
	}
	if (cbGameStatus == GS_TK_PLAYING)
	{
		OnPlayScence(data,dataSize);
	}
	return true;
}
void NNGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);

}
void NNGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;
	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;
	defaultState();
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_cbPlayStatus[i] = pNetInfo->cbPlayStatus[i];
		if (m_cbPlayStatus[i]==0)
		{
			continue;
		}
		NNPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbHandCardData[i],pPlayer == m_pLocal);
		if (i != pNetInfo->wBankerUser)
		{
			pPlayer->setJiaoZhuangFen((int)pNetInfo->lTableScore[i],false);
		}
		else
		{
			pPlayer->setZhuang(false);
		}
	}
	WidgetFun::setVisible(this,"BetStateNode",false);
	if (pNetInfo->bOxCard[m_pLocal->GetChairID()] != 0xff)
	{
		m_pLocal->showNiuCard(pNetInfo->bOxCard[m_pLocal->GetChairID()]);
		showBoxNumNode(false);
	}
	else
	{
		showBoxNumNode(true);
	}
	if (isSelfInGame())
	{
		m_pLocal->showClock(15);
	}
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}
void NNGameScence::OnGFGameClose(int iExitCode)
{
	ExitGame();
}
void NNGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}
void NNGameScence::LocalPlayerEnter()
{
}

void NNGameScence::OnCallBankerScence(void* data, int dataSize)
{
	ASSERT(dataSize==sizeof(CMD_S_StatusCall));
	if (dataSize!=sizeof(CMD_S_StatusCall)) return;
	CMD_S_StatusCall* pNetInfo = (CMD_S_StatusCall*)data;
	defaultState();

	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_cbPlayStatus[i] = pNetInfo->cbPlayStatus[i];
		if (m_cbPlayStatus[i]==0)
		{
			continue;
		}
		NNPlayer* pPlayer = getPlayerByChairID(i);
		if (pPlayer == m_pLocal)
		{
			pPlayer->showStartCard(pNetInfo->cbHandCardData[i]);
			if (pNetInfo->bCallStatus[i] != INVALID_BYTE)
			{
				WidgetFun::setVisible(this,"QiangZhuangStateNode",false);
				pPlayer->setJiaoZhuangFen(pNetInfo->bCallStatus[i]);
			}
			else
			{
				WidgetFun::setVisible(this,"QiangZhuangStateNode",true);
			}
		}
		else
		{
			pPlayer->showBackCard();
			if (pNetInfo->bCallStatus[i] != INVALID_BYTE)
			{
				pPlayer->setJiaoZhuangFen(pNetInfo->bCallStatus[i]);
			}
		}
	}
	if (isSelfInGame())
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"NNQiangZhuangWait");
		pNode->setVisible(true);
	}
}
void NNGameScence::OnBetScoreScence(void* data, int dataSize)
{
	ASSERT(dataSize==sizeof(CMD_S_StatusScore));
	if (dataSize!=sizeof(CMD_S_StatusScore)) return;
	CMD_S_StatusScore* pNetInfo = (CMD_S_StatusScore*)data;
	defaultState();

	if (m_pLocal->GetChairID() != pNetInfo->wBankerUser
		&& pNetInfo->lTableScore[m_pLocal->GetChairID()] == 0)
	{
		WidgetFun::setVisible(this,"BetStateNode",true);
		m_pLocal->showStartCard(pNetInfo->cbHandCardData[m_pLocal->GetChairID()]);
	}
	else
	{
		WidgetFun::setVisible(this,"BetStateNode",false);
		m_pLocal->showStartCard(pNetInfo->cbHandCardData[m_pLocal->GetChairID()]);
	}
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_cbPlayStatus[i] = pNetInfo->cbPlayStatus[i];
		if (m_cbPlayStatus[i]==0)
		{
			continue;
		}
		NNPlayer* pPlayer = getPlayerByChairID(i);
		if (pPlayer != m_pLocal)
		{
			pPlayer->setHandCard(pNetInfo->cbHandCardData[i],false);
		}
		if (i != pNetInfo->wBankerUser)
		{
			pPlayer->setJiaoZhuangFen((int)pNetInfo->lTableScore[i],false);
		}
		else
		{
			pPlayer->setZhuang(false);
		}
	}
	if (isSelfInGame())
	{
		m_pLocal->showClock(15);
	}
}