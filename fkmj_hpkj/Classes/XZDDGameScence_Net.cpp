#include "XZDDGameScence.h"
#include "GameLib.h"

#include "CMD_XZDD.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"

void XZDDGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,XZDDGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,XZDDGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD,this,XZDDGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,XZDDGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,XZDDGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,XZDDGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE,this,XZDDGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,XZDDGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,XZDDGameScence::OnSubGangScore);

	addNetCBDefine(SUB_S_HUANPAI,this,XZDDGameScence::OnSubHuanPai);
	addNetCBDefine(SUB_S_XUANQUE_NOTICE,this,XZDDGameScence::OnSubXuanQueNotice);
	addNetCBDefine(SUB_S_XUANQUE,this,XZDDGameScence::OnSubXuanQue);
	addNetCBDefine(SUB_S_HUANPAI_CHENGDU,this,XZDDGameScence::OnSubHuanPaiChengDu);

	addNetCBDefine(SUB_S_HUANPAI_NOTICE,this,XZDDGameScence::OnSubHuanPaiNotice);

	addNetCBDefine(SUB_S_MASTER_HANDCARD,this,XZDDGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD,this,XZDDGameScence::OnMasterLeftCard);
}
//游戏开始
void XZDDGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_GameStart));
	if (wDataSize!=sizeof(XZDD_CMD_S_GameStart)) 
		return;

	//变量定义
	XZDD_CMD_S_GameStart * pGameStart=(XZDD_CMD_S_GameStart *)pBuffer;

	defaultState();

	m_iBankerUser=pGameStart->wBankerUser;
	m_iCurrentUser=pGameStart->wCurrentUser;
	m_iUserAction = pGameStart->cbUserAction;
	m_nLeftTime = pGameStart->nLeftTime;

	WidgetFun::setText(this,"LastCardCout",(int)pGameStart->cbLeftCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData,MAX_COUNT-1);
		m_pPlayer[i]->startGame();
	}
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	XZDDButtonAction_ShowCard(NULL,NULL);
	m_nGameState = XZDD_STATE_READY;
}
//用户出牌
void XZDDGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_OutCard));
	if (wDataSize!=sizeof(XZDD_CMD_S_OutCard)) 
		return;

	//消息处理
	XZDD_CMD_S_OutCard * pOutCard=(XZDD_CMD_S_OutCard *)pBuffer;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setActOutCard(-1);
	}

	XZDDPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);
	pPlayer->blackAllHandCard(true);

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
//发牌消息
void XZDDGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_SendCard));
	if (wDataSize!=sizeof(XZDD_CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false); //发牌了 先隐藏操作按钮
	//变量定义
	XZDD_CMD_S_SendCard * pSendCard=(XZDD_CMD_S_SendCard *)pBuffer;
	setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask,pSendCard->cbCardData);
	XZDDPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	}
	if (pPlayer == m_pLocal&& !pSendCard->bHasHuPai)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	else if(pPlayer == m_pLocal&& pSendCard->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		m_pLocal->setHasHu(true);
		if (pSendCard->cbActionMask ==WIK_NULL )
		{
			TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOutCard,this),3,pSendCard->cbCardData);
		}
		else
		{
			TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate,this),3.f,pSendCard->cbActionMask);
		}
	}
	else
	{
		m_pLocal->blackAllHandCard(true);
	}
	m_nGameState = XZDD_STATE_PLAYING;
}
//操作提示
void XZDDGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_OperateNotify));
	if (wDataSize!=sizeof(XZDD_CMD_S_OperateNotify)) 
		return;

	//变量定义
	XZDD_CMD_S_OperateNotify * pOperateNotify=(XZDD_CMD_S_OperateNotify *)pBuffer;
	m_wResumeUser =pOperateNotify->wResumeUser;
	setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask,pOperateNotify->cbActionCard);
	if (pOperateNotify->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate,this),3.f,pOperateNotify->cbActionMask);
	}
}
//操作结果
void XZDDGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_OperateResult));
	if (wDataSize!=sizeof(XZDD_CMD_S_OperateResult)) 
		return ;

	//消息处理
	XZDD_CMD_S_OperateResult * pOperateResult=(XZDD_CMD_S_OperateResult *)pBuffer;

	XZDDPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);
	if (pPlayer == m_pLocal)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	XZDDPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer)
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}

	setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//游戏结束
void XZDDGameScence::OnSubGameEnd(void * pBuffer, WORD wDataSize)
{
	m_nGameState = XZDD_STATE_NULL;
	datastream kStream((pBuffer),wDataSize);
	XZDD_AllEndInfo kEndInfo;
	kEndInfo.StreamValue(kStream,false);
	word wChairId =  m_pLocal->GetChairID();
	NoticeMsg::Instance().ShowTopMsg(utility::toString((int)wChairId,":",(int)kEndInfo.lGameScoreTotal.size()));
	WidgetFun::setVisible(this,"FreeStateNode",true);
	WidgetFun::setVisible(this,"GameResoult1",true);

	SCORE lMyScore = kEndInfo.lGameScoreTotal[wChairId];

	WidgetFun::setVisible(this,"WinTitle1",lMyScore>=0);
	WidgetFun::setVisible(this,"LoseTitle1",lMyScore<0);

	WidgetFun::setVisible(this,"WinGoldAtlas",lMyScore>=0);
	WidgetFun::setVisible(this,"LoseGoldAtlas",lMyScore<0);

	if (lMyScore>=0)
	{
		WidgetFun::setAtlasTxt(this,"WinGoldAtlas",utility::toString("+",(int)lMyScore));
	}
	else
	{
		WidgetFun::setAtlasTxt(this,"LoseGoldAtlas",utility::toString((int)lMyScore));
	}
	showJieSuanInfo(&kEndInfo);
	defaultMaster();
	GameManagerBase::InstanceBase().OnGameEnd(this,3);
}

void XZDDGameScence::showJieSuanInfo( XZDD_AllEndInfo* kEndInfo)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"GameResoult2");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);

		BYTE cbCardData[MAX_INDEX]={0};
		XZDD_CMD_WeaveItem WeaveItemArray[MAX_WEAVE];
		zeromemory(WeaveItemArray,sizeof(WeaveItemArray));

		int nCardCount = kEndInfo->kCardInfoList[i].kHandCardList.size();
		int nWeaveCount = kEndInfo->kCardInfoList[i].kWeaveItemList.size();
		for (int n=0;n<nCardCount;n++)
		{
			cbCardData[n]=kEndInfo->kCardInfoList[i].kHandCardList[n];
		}
		for (int n=0;n<nWeaveCount;n++)
		{
			WeaveItemArray[n].cbWeaveKind = kEndInfo->kCardInfoList[i].kWeaveItemList[n].cbWeaveKind;
			WeaveItemArray[n].cbCenterCard = kEndInfo->kCardInfoList[i].kWeaveItemList[n].cbCenterCard;
			WeaveItemArray[n].cbPublicCard = kEndInfo->kCardInfoList[i].kWeaveItemList[n].cbPublicCard;
			WeaveItemArray[n].wProvideUser = kEndInfo->kCardInfoList[i].kWeaveItemList[n].wProvideUser;
		}					 
		pPlayer->showHandCard(WeaveItemArray,nWeaveCount,cbCardData,nCardCount);
		pPlayer->showCard(cbCardData,nCardCount);	

		WidgetFun::setButtonTxt(pRootNode,utility::toString("Button_ResoultPlayer",i),pPlayer->GetNickName());
		WidgetFun::setText(pRootNode,utility::toString("GoldTxt",i),kEndInfo->lGameScoreTotal[i]);
		setGameResoultPlayerInfo(kEndInfo,pPlayer,WidgetFun::getChildWidget(pRootNode,utility::toString("ResoultPlayer",i)));
	}
	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,
		utility::toString("Button_ResoultPlayer",m_pLocal->GetChairID())));
}
//用户托管
void XZDDGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_Trustee));
	if (wDataSize!=sizeof(XZDD_CMD_S_Trustee)) 
		return;

	//消息处理
	XZDD_CMD_S_Trustee * pTrustee=(XZDD_CMD_S_Trustee *)pBuffer;
}
//
void XZDDGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	datastream kStream(const_cast<void *>(pBuffer),wDataSize);
	XZDD_CMD_S_ChiHu kChiHu;
	kChiHu.StreamValue(kStream,false);


	XZDDPlayer* pPlayer = getPlayerByChairID(kChiHu.wChiHuUser);
	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(kChiHu.cbChiHuCard);
	if (kChiHu.wChiHuUser != kChiHu.wProviderUser)
	{
		XZDDPlayer* pDestPlayer = getPlayerByChairID(kChiHu.wProviderUser);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->removeHandOutCard(kChiHu.cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->runAniDianPao();
	}
	else
	{
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
		pPlayer->removeHandCard(kChiHu.cbChiHuCard);
	}
	pPlayer->showHuPai(pPlayer != m_pLocal);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayerTemp = getPlayerByChairID(i);
//		pPlayerTemp->showAddGold(kChiHu.lGameScore[i]);
	}
	pPlayer->setHasHu(true);
}
//
void XZDDGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	datastream kStream(const_cast<void *>(pBuffer),wDataSize);
	XZDD_CMD_S_GangScore kGangScore;
	kGangScore.StreamValue(kStream,false);


	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int nScore = kGangScore.lGangScore[i];
		if (nScore !=0)
		{
			XZDDPlayer* pPlayer = getPlayerByChairID(i);
			pPlayer->showAddGold(nScore);
		}
	}
}

void XZDDGameScence::OnSubHuanPai(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_HuanPai));
	if (wDataSize!=sizeof(XZDD_CMD_S_HuanPai))return;

	XZDD_CMD_S_HuanPai* pHuanPai = (XZDD_CMD_S_HuanPai*)pBuffer;
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->setHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this,"ExchangeCardNode",false);
}
void XZDDGameScence::OnSubHuanPaiChengDu( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_HuanPaiChengDu));
	if (wDataSize!=sizeof(XZDD_CMD_S_HuanPaiChengDu))return;

	XZDD_CMD_S_HuanPaiChengDu* pHuanPai = (XZDD_CMD_S_HuanPaiChengDu*)pBuffer;

	showSaiZi_HuanPai(pHuanPai->wSice);
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->saveRemoveHuanPai(pHuanPai->cbRemoveHuanCard);
		m_pLocal->saveHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this,"ExchangeCardNode",false);
}

void XZDDGameScence::OnSubXuanQueNotice( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_XuanQueNotice));
	if (wDataSize!=sizeof(XZDD_CMD_S_XuanQueNotice))return;

	XZDD_CMD_S_XuanQueNotice* pInfo = (XZDD_CMD_S_XuanQueNotice*)pBuffer;
	m_nLeftTime = pInfo->nLeftTime;

	if (pInfo->wChairId == getSelfChairID())
	{
		m_pLocal->setQueColor(pInfo->nQueColor);
	}
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::DoXuanQueNotice,this),2.f);
}

void XZDDGameScence::OnSubXuanQue( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(XZDD_CMD_S_XuanQue));
	if (wDataSize!=sizeof(XZDD_CMD_S_XuanQue))return;

	XZDD_CMD_S_XuanQue* pInfo = (XZDD_CMD_S_XuanQue*)pBuffer;

	for (int i=0;i<MAX_PLAYER;i++)
	{
		cocos2d::log(" user chairid :%d",i);
		cocos2d::log(" user nQueColor:%d",(int)pInfo->nQueColor[i]);

		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setQueColor(pInfo->nQueColor[i]);
		pPlayer->showQueIconAni();
		pPlayer->showXuanQue(false);
	}

}

void XZDDGameScence::OnSubHuanPaiNotice(const void *pBuffer, WORD wDataSize )
{
	showHuanPai();
//	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::showHuanPai,this),4.2f);
}

void XZDDGameScence::showHuanPai()
{
	ASSERT(m_pLocal);

	WidgetFun::setVisible(this,"ExchangeCardNode",true);
	m_pLocal->showHuanPai(GAME_TYPE_CHENGDU);
	m_nGameState = XZDD_STATE_HUANPAI;
}