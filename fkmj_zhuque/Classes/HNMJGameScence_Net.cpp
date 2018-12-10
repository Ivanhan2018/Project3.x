#include "HNMJGameScence.h"
#include "GameLib.h"

#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"

void HNMJGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,HNMJGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,HNMJGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_OUT_CARD_CSGANG,this,HNMJGameScence::OnSubOutCardCSGang);
	addNetCBDefine(SUB_S_SEND_CARD,this,HNMJGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_SEND_CARD_CSGANG,this,HNMJGameScence::OnSubSendCard_CSGang);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,HNMJGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,HNMJGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,HNMJGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_GAME_END_LAST_CARD,this,HNMJGameScence::OnSubGameEndLastCard);
	addNetCBDefine(SUB_S_TRUSTEE,this,HNMJGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,HNMJGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,HNMJGameScence::OnSubGangScore);
	addNetCBDefine(SUB_S_XIAO_HU,this,HNMJGameScence::OnSubXiaoHu);
	addNetCBDefine(SUB_S_USE_HAIDI_CARD,this,HNMJGameScence::OnSubUseHaiDiCard);
	addNetCBDefine(SUB_S_PIAO_NOTICE,this,HNMJGameScence::OnSubPiaoNotice);
	
	addNetCBDefine(SUB_S_MASTER_HANDCARD,this,HNMJGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD,this,HNMJGameScence::OnMasterLeftCard);
	
}
//游戏开始
void HNMJGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
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

	if (WidgetFun::getChildWidget(this,"HomeDismissPrivatePlane"))
	{
		WidgetFun::setVisible(this,"HomeDismissPrivatePlane",false);
	}
}
//用户出牌
void HNMJGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
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

	HNMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);
	if (pPlayer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_SHOW_TING_CARD))
	{
		m_pLocal->showTingCard();
	}
	if (pPlayer == m_pLocal)
	{
		m_iCurrentUser = INVALID_CHAIR;
	}

	defaultPlayerActionState();
}


//用户出牌
void HNMJGameScence::OnSubOutCardCSGang(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard_CSGang));
	if (wDataSize!=sizeof(CMD_S_OutCard_CSGang)) 
		return;

	//消息处理
	CMD_S_OutCard_CSGang * pOutCard=(CMD_S_OutCard_CSGang *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->showCSGangCard(pOutCard->cbOutCardData1,pOutCard->cbOutCardData2,true);
	setGameState(HNMJ_STATE_PLAYING);
}
void HNMJGameScence::setGangNodeInfo(cocos2d::Node* pNode,BYTE pCardData[4],BYTE cbCardCout,int iAction)
{
	std::string kImagicFront = WidgetFun::getWidgetUserInfo(this,"SelfActionNode","ImagicFront");
	float fStartPosX = -(cbCardCout-1)*30;
	for (int i = 0;i<4;i++)
	{
		cocos2d::Node* pTempNode = WidgetFun::getChildWidget(pNode,utility::toString("HNMJButton_GangActionNode",i));
		pTempNode->setPositionX(fStartPosX+i*60);
		pTempNode->setVisible(i < cbCardCout);
		WidgetFun::setWidgetUserInfo(pTempNode,"Action",utility::toString(iAction));
		WidgetFun::setWidgetUserInfo(pTempNode,"Action",utility::toString(iAction));
		WidgetFun::setWidgetUserInfo(pTempNode,"Action",utility::toString(iAction));
		WidgetFun::setWidgetUserInfo(pTempNode,"Card",utility::toString((int)pCardData[i]));
		setActionBtnCard(pTempNode,(int)pCardData[i],kImagicFront);
	}
	WidgetFun::set9Size(pNode,"GangBackPlane",cocos2d::Size(cbCardCout*60+40,80));
}
void HNMJGameScence::setGangInfo(BYTE pCardData[4],BYTE cbCardCout)
{
	if (cbCardCout > 4)
	{
		CCASSERT(false,"");
		return;
	}
	for (BYTE i = 0;i<4;i++)
	{
		m_kcbGangCardData[i] = pCardData[i];
	}
	m_kcbGangCardCount = cbCardCout;
	cocos2d::Node* pSelfActionNode = WidgetFun::getChildWidget(this,"SelfActionNode");
	std::string kImagicFront = WidgetFun::getWidgetUserInfo(this,"SelfActionNode","ImagicFront");

	setGangNodeInfo(WidgetFun::getChildWidget(pSelfActionNode,"HNMJButton_GangAction"),pCardData,cbCardCout,WIK_GANG);
	setGangNodeInfo(WidgetFun::getChildWidget(pSelfActionNode,"HNMJButton_GangCS"),pCardData,cbCardCout,WIK_CS_GANG);
	setGangNodeInfo(WidgetFun::getChildWidget(pSelfActionNode,"HNMJButton_BuAction"),pCardData,cbCardCout,WIK_BU_ZHANG);

}
//发牌消息
void HNMJGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"CSGangActionNode",false);
	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	HNMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
		if (pPlayer == m_pLocal)
		{
			m_pLocal->setHasCSGang();
		}
	}
	BYTE kTemp[4] = {pSendCard->cbCardData,0,0,0};
	setGangInfo(kTemp,1);
	setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask,pSendCard->cbCardData);
	setGameState(HNMJ_STATE_PLAYING);
}

void HNMJGameScence::OnSubSendCard_CSGang( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard_CSGang));
	if (wDataSize!=sizeof(CMD_S_SendCard_CSGang)) 
		return;
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"CSGangActionNode",false);

	//变量定义
	CMD_S_SendCard_CSGang * pSendCard=(CMD_S_SendCard_CSGang *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	pPlayer->showCSGangCard(pSendCard->cbCardData1,pSendCard->cbCardData2,false);
	if (pSendCard->cbActionMask & WIK_CHI_HU )
	{
		setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask);
	}
	int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 2;
	WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	setGameState(HNMJ_STATE_NULL);
}
//操作提示
void HNMJGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	if (!pOperateNotify->bGangStatus_CS)
	{
		setGangInfo(pOperateNotify->cbGangCardData,pOperateNotify->cbGangCardCount);
		setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask1,pOperateNotify->cbActionCard1);
	}
	else
	{
		BYTE kTemp[4] = {pOperateNotify->cbActionCard1,pOperateNotify->cbActionCard2,0,0};
		setGangInfo(kTemp,2);
		setCurrentPlayer(m_pLocal->GetChairID(),0,0);
		showCSGangAction(pOperateNotify->wProviderUser,pOperateNotify->cbActionCard1,pOperateNotify->cbActionMask1
			,pOperateNotify->cbActionCard2,pOperateNotify->cbActionMask2);
	}
}
//操作结果
void HNMJGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return ;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);

	HNMJPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer &&(pOperateResult->cbOperateCode == WIK_PENG 
		|| pOperateResult->cbOperateCode == WIK_LEFT
		|| pOperateResult->cbOperateCode == WIK_CENTER
		|| pOperateResult->cbOperateCode == WIK_RIGHT
		|| (pOperateResult->cbOperateCode == WIK_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )
		|| (pOperateResult->cbOperateCode == WIK_CS_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )
		|| (pOperateResult->cbOperateCode == WIK_BU_ZHANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )))
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}
	if (pOperateResult->cbOperateCode == WIK_CS_GANG && pPlayer == m_pLocal)
	{
		m_pLocal->setHasCSGang(true);//长沙杠，
	}

	setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"CSGangActionNode",false);
}
//游戏结束
void HNMJGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	WidgetFun::setVisible(this,"TuoGuanStateNode",false);
	WidgetFun::setVisible(this,"CSGangActionNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	showJieSuanInfo(pGameEnd);
	setGameState(HNMJ_STATE_NULL);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int iChirID = m_pPlayer[i]->GetChairID();
		m_pPlayer[i]->setHandCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
		if (pGameEnd->cbChiHuCardData[iChirID][0])
		{
			m_pPlayer[i]->addNewInCard(pGameEnd->cbChiHuCardData[iChirID][0]);
		}
		if (pGameEnd->cbChiHuCardData[iChirID][1])
		{
			m_pPlayer[i]->addNewInCard(pGameEnd->cbChiHuCardData[iChirID][1]);
		}
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->showCard();	
	}
	defaultMaster();
	removeWaringSound();

	if (WidgetFun::getChildWidget(this,"HomeDismissPrivatePlane"))
	{
		WidgetFun::setVisible(this,"HomeDismissPrivatePlane",true);
	}
}
void HNMJGameScence::OnSubGameEndLastCard(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd_LastCard));
	if (wDataSize!=sizeof(CMD_S_GameEnd_LastCard)) 
		return;
	CMD_S_GameEnd_LastCard * pGameEndLastCout=(CMD_S_GameEnd_LastCard *)pBuffer;
	if (!WidgetFun::getChildWidget(this,"PlaneGameEndLastCard"))
	{
		return;
	}
	if (!HaveOptionRule(GAME_OPTION_RULE_SHOW_LAST_CARD))
	{
		return;
	}
	WidgetFun::setVisible(this,"PlaneGameEndLastCard",true);
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"GameEndLastCardNode");
	pCardNode->removeAllChildren();
	cocos2d::Node* pNode = NULL;
	float fPosY = 160;
	for (BYTE cbIdex = 0;cbIdex < pGameEndLastCout->cbCardCout;cbIdex++)
	{
		BYTE cbTemp1 = cbIdex / 7;
		BYTE cbTemp2 = cbIdex % 7;
		if (cbTemp2 == 0)
		{
			pNode = WidgetManager::Instance().createWidget("HNMJJieSuanCardNode",pCardNode);
			pNode->setPosition(0,fPosY);
			fPosY -= 50;
		}
		cocos2d::Node* pChildNode = WidgetFun::getChildWidget(pNode,utility::toString("HNMJ_ChiCard",(int)cbTemp2));
		pChildNode->setVisible(true);
		m_pLocal->setCardImagic(pChildNode,pGameEndLastCout->cbCardData[cbIdex],"GameHNMJ/2/mingmah_");
	}
}
//用户托管
void HNMJGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) 
		return;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	HNMJPlayer* pPlayer = getPlayerByChairID(pTrustee->wChairID);
	pPlayer->setTuoGuan(pTrustee->bTrustee);
	if (pPlayer == m_pLocal)
	{
		WidgetFun::setVisible(this,"TuoGuanStateNode",pTrustee->bTrustee);
	}
}
//
void HNMJGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_ChiHu));
	if (wDataSize!=sizeof(CMD_S_ChiHu))
		return;

	//消息处理
	CMD_S_ChiHu * pChiHu=(CMD_S_ChiHu *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pChiHu->wChiHuUser);
	pPlayer->addNewInCard(pChiHu->cbChiHuCard);
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		HNMJPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->removeHandOutCard(pChiHu->cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pPlayer->runAniDianPao();
	}
	else
	{
		pPlayer->showHuPai(true);
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
	}
}
//
void HNMJGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_GangScore));
	if (wDataSize!=sizeof(CMD_S_GangScore)) 
		return;

	//消息处理
	CMD_S_GangScore * pGangScore=(CMD_S_GangScore *)pBuffer;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		HNMJPlayer* pPlayer = getPlayerByChairID(i);
		//pPlayer->showAddGold(pGangScore->lGangScore[i]);
	}
}
void HNMJGameScence::OnSubXiaoHu(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_XiaoHu));
	if (wDataSize!=sizeof(CMD_S_XiaoHu)) 
		return;

	//消息处理
	CMD_S_XiaoHu * pXiaoHu=(CMD_S_XiaoHu *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pXiaoHu->wXiaoHuUser);
	if (pPlayer)
	{
		pPlayer->showXiaoHu(pXiaoHu->dwXiaoCode);
		pPlayer->showCard(pXiaoHu->cbCardData,pXiaoHu->cbCardCount);

		TimeManager::Instance().addCerterTimeCB1<int>(CC_CALLBACK_1(HNMJGameScence::OnXiaoHuEnd,this),this,7.f,pXiaoHu->wXiaoHuUser);
	}
}
void HNMJGameScence::OnSubUseHaiDiCard(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_UseHaiDi));
	if (wDataSize!=sizeof(CMD_S_UseHaiDi)) 
		return;
	//消息处理
	CMD_S_UseHaiDi * pHaiDiNetInfo=(CMD_S_UseHaiDi *)pBuffer;
	setCurrentPlayer(pHaiDiNetInfo->wCurrentUser,0);

	if (pHaiDiNetInfo->wCurrentUser == m_pLocal->GetChairID())
	{
		WidgetFun::setVisible(this,"GameHaiDiNode",true);
	}
}

bool HNMJGameScence::OnXiaoHuEnd( int nChairID )
{
	HNMJPlayer* pPlayer = getPlayerByChairID(nChairID);
	if (pPlayer)
	{
		pPlayer->showHandCard();
	}
	return false;
}

void HNMJGameScence::OnSubPiaoNotice(const void *pBuffer, WORD wDataSize)
{
	if (WidgetFun::getChildWidget(this,"GamePiaoNode"))
	{
		WidgetFun::setVisible(this,"GamePiaoNode",true);
	}
}

