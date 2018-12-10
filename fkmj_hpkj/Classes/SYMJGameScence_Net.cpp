#include "SYMJGameScence.h"
#include "GameLib.h"

#include "CMD_SYMJ.h"
#include "SYMJPlayer.h"
#include "SYMJGameLogic.h"

void SYMJGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,SYMJGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,SYMJGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD,this,SYMJGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,SYMJGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,SYMJGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,SYMJGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE,this,SYMJGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,SYMJGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,SYMJGameScence::OnSubGangScore);
	addNetCBDefine(SUB_S_FANBEI_NOTICE,this,SYMJGameScence::OnSubFanBeiNotice);
	addNetCBDefine(SUB_S_SEND_FANBEI,this,SYMJGameScence::OnSubSendFanBei);

	addNetCBDefine(SUB_S_MASTER_HANDCARD,this,SYMJGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD,this,SYMJGameScence::OnMasterLeftCard);
}
//游戏开始
void SYMJGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	defaultState();

	m_iBankerUser=pGameStart->wBankerUser;
	m_iCurrentUser=pGameStart->wCurrentUser;
	m_iUserAction = pGameStart->cbUserAction;

	WidgetFun::setText(this,"LastCardCout",(int)pGameStart->cbLeftCardCount);

	int nIndex = 1;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData,MAX_COUNT-1);
		m_pPlayer[i]->startGame();
	}

	HNMJButtonAction_ShowCard(NULL,NULL);
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	if (pGameStart->cbXiaoHuTag == 1)
	{
		setGameState(HNMJ_STATE_XIAO_HU);
	}
	else
	{
		setGameState(HNMJ_STATE_PLAYING);
	}

	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_TING_CARD))
	{
		m_pLocal->showTingCard();
	}

}
//用户出牌
void SYMJGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) 
		return;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setActOutCard(-1);
	}

	SYMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);
	if (pPlayer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_SHOW_TING_CARD))
	{
		m_pLocal->showTingCard();
	}

	defaultPlayerActionState();
}


//用户出牌
void SYMJGameScence::OnSubOutCardCSGang(const void * pBuffer, WORD wDataSize)
{
}

//发牌消息
void SYMJGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false);
	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	SYMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	}
	setGameState(HNMJ_STATE_PLAYING);

	if (pSendCard->bHasCSGang && pPlayer == m_pLocal)
	{
		m_pLocal->blackAllHandCard(true);
		TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(SYMJGameScence::OnAutoOutCard,this),3,pSendCard->cbCardData);
	}
	else
	{
		pPlayer->blackAllHandCard(false);
		setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask,pSendCard->cbCardData);
	}
}

//操作提示
void SYMJGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask,pOperateNotify->cbActionCard);
}
//操作结果
void SYMJGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return ;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	SYMJPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);

	SYMJPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer &&(pOperateResult->cbOperateCode == WIK_PENG 
		|| pOperateResult->cbOperateCode == WIK_LEFT
		|| pOperateResult->cbOperateCode == WIK_CENTER
		|| pOperateResult->cbOperateCode == WIK_RIGHT
		|| (pOperateResult->cbOperateCode == WIK_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )
		|| (pOperateResult->cbOperateCode == WIK_CS_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )))
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}
	if (pOperateResult->cbOperateCode == WIK_PENG  
		|| pOperateResult->cbOperateCode == WIK_LEFT
		|| pOperateResult->cbOperateCode == WIK_CENTER
		|| pOperateResult->cbOperateCode == WIK_RIGHT)
	{
		setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	}
	else
	{
		m_iCurrentUser = INVALID_CHAIR;
	}
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//游戏结束
void SYMJGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	WidgetFun::setVisible(this,"TuoGuanStateNode",false);
	showJieSuanInfo(pGameEnd);
	setGameState(HNMJ_STATE_NULL);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int iChirID = m_pPlayer[i]->GetChairID();
		m_pPlayer[i]->showHandCard(pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID],
			pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
		m_pPlayer[i]->showCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);	
	}
	defaultMaster();
	removeWaringSound();
}
//用户托管
void SYMJGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) 
		return;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	SYMJPlayer* pPlayer = getPlayerByChairID(pTrustee->wChairID);
	pPlayer->setTuoGuan(pTrustee->bTrustee);
	if (pPlayer == m_pLocal)
	{
		WidgetFun::setVisible(this,"TuoGuanStateNode",pTrustee->bTrustee);
	}
}
//
void SYMJGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_ChiHu));
	if (wDataSize!=sizeof(CMD_S_ChiHu))
		return;

	//消息处理
	CMD_S_ChiHu * pChiHu=(CMD_S_ChiHu *)pBuffer;

	SYMJPlayer* pPlayer = getPlayerByChairID(pChiHu->wChiHuUser);
//	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(pChiHu->cbChiHuCard);
	pPlayer->addNewInCard(pChiHu->cbChiHuCard);
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		SYMJPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->removeHandOutCard(pChiHu->cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->runAniDianPao();
	}
	else
	{
		pPlayer->showHuPai(true);
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
	}
}
//
void SYMJGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_GangScore));
	if (wDataSize!=sizeof(CMD_S_GangScore)) 
		return;

	//消息处理
	CMD_S_GangScore * pGangScore=(CMD_S_GangScore *)pBuffer;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		SYMJPlayer* pPlayer = getPlayerByChairID(i);
		//pPlayer->showAddGold(pGangScore->lGangScore[i]);
	}
}

void SYMJGameScence::OnSubFanBeiNotice(const void *pBuffer, WORD wDataSize)
{
	if (WidgetFun::getChildWidget(this,"GameFanBeiNode"))
	{
		WidgetFun::setVisible(this,"GameFanBeiNode",true);
	}
}

void SYMJGameScence::OnSubSendFanBei(const void *pBuffer, WORD wDataSize)
{

	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_FanBei));
	if (wDataSize!=sizeof(CMD_S_FanBei)) 
		return;

	CMD_S_FanBei* pFanBei = (CMD_S_FanBei*)pBuffer;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		SYMJPlayer* pPlayer = getPlayerByChairID(i);
		if (pPlayer)
		{
			pPlayer->showFanBei(pFanBei->cbFanBei[i]);
		}
	}
}

