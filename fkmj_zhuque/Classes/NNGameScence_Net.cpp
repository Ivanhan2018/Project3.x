#include "NNGameScence.h"
#include "GameLib.h"

#include "CMD_Ox.h"
#include "NNPlayer.h"
#include "NNGameLogic.h"
#include "NNSoundFun.h"

void NNGameScence::initNet()
{
	addNetCBDefine(SUB_S_ALL_CARD,this,NNGameScence::OnSubAllCard);
	addNetCBDefine(SUB_S_CALL_BANKER,this,NNGameScence::OnSubCallBanker);
	addNetCBDefine(SUB_S_GAME_START,this,NNGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_ADD_SCORE,this,NNGameScence::OnSubAddScore);
	addNetCBDefine(SUB_S_SEND_CARD,this,NNGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPEN_CARD,this,NNGameScence::OnSubOpenCard);
	addNetCBDefine(SUB_S_PLAYER_EXIT,this,NNGameScence::OnSubPlayerExit);
	addNetCBDefine(SUB_S_GAME_END,this,NNGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_AMDIN_COMMAND,this,NNGameScence::OnSubAdminControl);
}
//发牌
void NNGameScence::OnSubAllCard(void * pBuffer, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_AllCard));
	if (wDataSize!=sizeof(CMD_S_AllCard)) 
		return;

	CMD_S_AllCard * pNetInfo=(CMD_S_AllCard *)pBuffer;

	NNSoundFun::playEffect("fapai.mp3");
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_cbPlayStatus[i] = pNetInfo->cbPlayStatus[i];
	}
	int nValue=0;
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i] == 0)
		{
			continue;
		}
		NNPlayer* pPlayer = getPlayerByChairID(i);
		if (pPlayer == m_pLocal)
		{
			pPlayer->showStartCard(pNetInfo->cbCardData[i]);
		}
		else
		{
			pPlayer->showBackCard();
		}
		pPlayer->sendCardAni(nValue);
		nValue++;
	}
	if (isSelfInGame()&& pNetInfo->bIsQiang)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"NNQiangZhuangWait");
		WidgetFun::setWidgetUserInfo(pNode,"WaitTime",utility::toString(nValue*0.5));
		WidgetFun::runWidgetAction(pNode,"QiangZhuangWaitAni");
	}
}
//用户叫庄
void NNGameScence::OnSubCallBanker(void * pBuffer, int wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CallBanker));
	if (wDataSize!=sizeof(CMD_S_CallBanker)) 
		return;

	CMD_S_CallBanker * pNetInfo=(CMD_S_CallBanker *)pBuffer;

	if (pNetInfo->wCallChairID == m_pLocal->GetChairID())
	{
		WidgetFun::setVisible(this,"QiangZhuangStateNode",false);
	}

	NNPlayer* player = getPlayerByChairID(pNetInfo->wCallChairID);
	if (player)
	{
		player->HideClock();
		player->setJiaoZhuangFen(pNetInfo->cbCallDouble);
	}
}
//游戏开始
void NNGameScence::OnSubGameStart(void * pBuffer, int wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return;

	CMD_S_GameStart * pNetInfo=(CMD_S_GameStart *)pBuffer;
	m_cbBankerChairID = pNetInfo->wBankerUser;
	checkZhuang(pNetInfo);
}
//用户加注
void NNGameScence::OnSubAddScore(void * pBuffer, int wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) 
		return;

	CMD_S_AddScore * pNetInfo=(CMD_S_AddScore *)pBuffer;
	NNPlayer* pPlayer = getPlayerByChairID(pNetInfo->wAddScoreUser);
	pPlayer->setJiaoZhuangFen(pNetInfo->lAddScoreCount,false);
}
//发牌消息
void NNGameScence::OnSubSendCard(void * pBuffer, int wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	CMD_S_SendCard * pNetInfo=(CMD_S_SendCard *)pBuffer;

	WidgetFun::setVisible(this,"BetStateNode",false);
	showBoxNumNode(true);
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		if (m_cbPlayStatus[i] == 0)
		{
			continue;
		}
		NNPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData[i],pPlayer == m_pLocal);
	}
	if (isSelfInGame())
	{
		m_pLocal->showClock(15);
	}
}
//用户摊牌
void NNGameScence::OnSubOpenCard(void * pBuffer, int wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_Open_Card));
	if (wDataSize!=sizeof(CMD_S_Open_Card)) 
		return;

	CMD_S_Open_Card * pNetInfo=(CMD_S_Open_Card *)pBuffer;
	NNPlayer* pPlayer = getPlayerByChairID(pNetInfo->wPlayerID);
	pPlayer->showNiuCard(pNetInfo->bOpen);
	pPlayer->HideClock();
	if (pPlayer == m_pLocal)
	{
		showBoxNumNode(false);
	}
}
//用户强退
void NNGameScence::OnSubPlayerExit(void * pBuffer, int wDataSize)
{
}
//游戏结束
void NNGameScence::OnSubGameEnd(void * pBuffer, int wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	CMD_S_GameEnd * pNetInfo=(CMD_S_GameEnd *)pBuffer;
	showBoxNumNode(false);
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		NNPlayer* pPlayer = getPlayerByChairID(i);
		if (pNetInfo->lGameScore[i] != 0)
		{
			pPlayer->showAddGold(pNetInfo->lGameScore[i]);
		}
		if (pPlayer == m_pLocal)
		{
			showJieSuanInfo(pNetInfo->lGameScore[i]);
		}

	}
}
//控制
void NNGameScence::OnSubAdminControl(void * pBuffer, int wDataSize)
{

}