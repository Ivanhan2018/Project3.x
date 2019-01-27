/***************************************
 牌管理类
 游戏界面
 xxxx 2013.1.5
 ***************************************/

#include "CardManager.h"
#include "CocosUnits.h"
#include "GlobalUnits.h"
#include "BJLRes.h"//by hxh
#include "MyConfig.h"//by hxh
#include "SoundControl.h"//by hxh
#include "MyNSString.h"//by hxh

using namespace cocos2d;

// 得分统计界面标签相对位置<by hxh 20160707>
#if 1
Vec2 LblPos[9] = {
	ccp(0.25,0.2*1.0),//0闲
	ccp(0.6,0.2*1.0),//1平
	ccp(1.0,0.2*1.0),//2庄
	ccp(0.25,0.2*0.65),//3闲天王
	ccp(1.0,0.2*0.65),//4庄天王
	ccp(0.6,0.2*0.65),//5同点平
	ccp(0.25,0.2*1.35),//6闲对子
	ccp(1.0,0.2*1.35),//7庄对子
	ccp(0.35,0.2*0.4),//8合计输赢
};
#else
Vec2 LblPos[9] = {
	ccp(0.3,0.25*1.2),
	ccp(0.9,0.25*1.2),
	ccp(1.5,0.25*1.2),
	ccp(0.3,0.25*0.8),
	ccp(1.5,0.25*0.8),
	ccp(0.9,0.25*0.8),
	ccp(0.3,0.25*1.6),
	ccp(1.5,0.25*1.6),
	ccp(0.5,0.14),//8合计输赢
};
#endif
    
//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引
CardManager *CardManager::create()
{
	CardManager *cardManager = new CardManager();
	if (cardManager&&cardManager->initCardManager())
	{
		cardManager->autorelease();
		return cardManager;
	}	
	CC_SAFE_DELETE(cardManager);
	return NULL;
}

void CardManager::onExit()
{
	Layer::onExit();
}

bool CardManager::initCardManager()
{
	bool bRet = false;
	do 
	{
		Winsize = Director::getInstance()->getWinSize();

		MyConfig& myConfig=MyConfig::Instance();

		XianZhuangBg = Sprite::createWithSpriteFrame(spriteFrame("BJL_21.png"));
		XianZhuangBg->setAnchorPoint(Vec2::ZERO);
		XianZhuangBg->setPosition(ccp(myConfig.m_XianZhuangBgPos[0],myConfig.m_XianZhuangBgPos[1]));
		XianZhuangBg->setVisible(false);
		this->addChild(XianZhuangBg);

		m_pUserPlaySprite = Sprite::createWithSpriteFrame(spriteFrame("GAME_POINT_0.png"));
		m_pUserPlaySprite->setPosition(ccp(myConfig.m_UserPlayPos[0],myConfig.m_UserPlayPos[1]));
		m_pUserPlaySprite->setVisible(false);
		XianZhuangBg->addChild(m_pUserPlaySprite);
		m_pUserBankSprite = Sprite::createWithSpriteFrame(spriteFrame("GAME_POINT_0.png"));
		m_pUserBankSprite->setPosition(ccp(myConfig.m_UserBankPos[0],myConfig.m_UserBankPos[1]));
		m_pUserBankSprite->setVisible(false);
		XianZhuangBg->addChild(m_pUserBankSprite);

		Player = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_Player));//"BJL_27_2.png"
		Player->setPosition(ccp(myConfig.m_PlayerPos[0],myConfig.m_PlayerPos[1]));
		Player->setVisible(false);
		XianZhuangBg->addChild(Player);

		AndBureauSprite = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_AndBureau));//"BJL_27_3.png"
		AndBureauSprite->setAnchorPoint(Vec2::ZERO);
		AndBureauSprite->setPosition(ccp(myConfig.m_AndBureauPos[0],myConfig.m_AndBureauPos[1]));
		AndBureauSprite->setVisible(false);
	    XianZhuangBg->addChild(AndBureauSprite);

		Banker = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_Banker));//"BJL_27_1.png"
		Banker->setPosition(ccp(myConfig.m_BankerPos[0],myConfig.m_BankerPos[1]));
		Banker->setVisible(false);
		XianZhuangBg->addChild(Banker);

		//结束动画
		GAMEoverSprite = Sprite::create(texture_name::s_GameEnd);
		GAMEoverSprite->setAnchorPoint(Vec2::ZERO);
		GAMEoverSprite->setPosition(ccp(myConfig.m_GAMEoverPos1[0],myConfig.m_GAMEoverPos1[1]));
		this->addChild(GAMEoverSprite,3);

		initPlayerCard();

	// add by hxh 20160806
#ifdef USE_PRINTTEST3
	Test=LabelTTF::create("","",16);
	Test->setAnchorPoint(ccp(0,0));
	Test->setColor(ccc3(0,0,0));
	Test->setPosition(ccp(73,290));
	XianZhuangBg->addChild(Test);

	Test2=LabelTTF::create("","",16);
	Test2->setAnchorPoint(ccp(0,0));
	Test2->setColor(ccc3(0,0,0));
	Test2->setPosition(ccp(285,290));
	XianZhuangBg->addChild(Test2);
#endif

		bRet = true;
	} while (0);

	return bRet;
}

void CardManager::initPlayerCard()
{
	for (int i = 0;i < 2;i++)
	{
		playerCard[i].cards[0] = CardSprite::CardSpriteWith(0);
		playerCard[i].cards[1] = CardSprite::CardSpriteWith(0);
		playerCard[i].cards[2] = CardSprite::CardSpriteWith(0);

		//playerCard[i].cards[0]->setScale(0.5f);
		//playerCard[i].cards[1]->setScale(0.5f);
		//playerCard[i].cards[2]->setScale(0.5f);

		playerCard[i].cards[0]->setVisible(false);
		playerCard[i].cards[1]->setVisible(false);
		playerCard[i].cards[2]->setVisible(false);

		this->addChild(playerCard[i].cards[0],2-i);
		this->addChild(playerCard[i].cards[1],2-i);
		this->addChild(playerCard[i].cards[2],2-i);
	}

	MyConfig& myConfig=MyConfig::Instance();
	//Vec2 pt=ccp(Winsize.width*0.8,Winsize.height);
	Vec2 pt=ccp(myConfig.m_playerCardPos1[0],myConfig.m_playerCardPos1[1]);
	playerCard[0].cards[0]->setPosition(pt);
	playerCard[0].cards[1]->setPosition(pt);
	playerCard[0].cards[2]->setPosition(pt);
	playerCard[1].cards[0]->setPosition(pt);
	playerCard[1].cards[1]->setPosition(pt);
	playerCard[1].cards[2]->setPosition(pt);
	memset(m_ucHandCardData,0,sizeof(m_ucHandCardData));
	//memset(m_bPlayerStatus,0,sizeof(m_bPlayerStatus));
	memset(m_ucDeskCardData,0,sizeof(m_ucDeskCardData));
	m_ZXP=0;
	for (int i = 0; i < 8; i++)
	{
		PlayhomeTTf[i]  =  LabelTTF::create("0","",16);
		PlayhomeTTf[i]->setAnchorPoint(Vec2::ZERO);
		PlayhomeTTf[i]->setColor(ccc3(173,118,0));
		// by hxh 20160707
#if 0
		PlayhomeTTf[i]->setPosition(ccp(GAMEoverSprite->getPositionX()*LblPos[i].x,Winsize.height*LblPos[i].y));
#else
		PlayhomeTTf[i]->setPosition(ccp(myConfig.m_GAMEoverLblPos[i][0],myConfig.m_GAMEoverLblPos[i][1]));
#endif
		GAMEoverSprite->addChild(PlayhomeTTf[i],3);
	}
	ScoreGameover  =  LabelTTF::create("0","",16);
	//ScoreGameover->setPosition(ccp(GAMEoverSprite->getPositionX()*LblPos[8].x,Winsize.height*LblPos[8].y));
	ScoreGameover->setPosition(ccp(myConfig.m_GAMEoverLblPos[8][0],myConfig.m_GAMEoverLblPos[8][1]));
    ScoreGameover->setColor(ccc3(173,118,0));
	ScoreGameover->setAnchorPoint(Vec2::ZERO);
	GAMEoverSprite->addChild(ScoreGameover,3);
	// add by hxh 20160714
#if 1
	Xian  =  LabelTTF::create("0","",16);
	Xian->setPosition(ccp(myConfig.m_GAMEoverLblPos[9][0],myConfig.m_GAMEoverLblPos[9][1]));
    Xian->setColor(ccc3(173,118,0));
	Xian->setAnchorPoint(Vec2::ZERO);
	GAMEoverSprite->addChild(Xian,3);

	Zhuang  =  LabelTTF::create("0","",16);
	Zhuang->setPosition(ccp(myConfig.m_GAMEoverLblPos[10][0],myConfig.m_GAMEoverLblPos[10][1]));
    Zhuang->setColor(ccc3(173,118,0));
	Zhuang->setAnchorPoint(Vec2::ZERO);
	GAMEoverSprite->addChild(Zhuang,3);
#endif
}

void CardManager::showPlayerCardfive()
{
	for (int iChairID=0;iChairID<2;iChairID++)
	{
		playerCard[iChairID].cards[0]->changeCardWith(m_ucHandCardData[0]);
		playerCard[iChairID].cards[1]->changeCardWith(m_ucHandCardData[1]);
		playerCard[iChairID].cards[2]->changeCardWith(m_ucHandCardData[2]);
		playerCard[iChairID].cards[0]->setVisible(true);
		playerCard[iChairID].cards[1]->setVisible(true);
		playerCard[iChairID].cards[2]->setVisible(true);
	}
}
void CardManager::showPlayerCard()
{
	XianZhuangBg->setVisible(true);//<by hxh>
	Player->setVisible(true);
	Banker->setVisible(true);
// 	m_pUserPlaySprite->setVisible(true);
// 	m_pUserBankSprite->setVisible(true);
	playerCard[0].cards[0]->changeCardWith(m_ucHandCardData[0]);
	playerCard[0].cards[1]->changeCardWith(m_ucHandCardData[1]);
	playerCard[1].cards[0]->changeCardWith(m_ucHandCardData1[0]);
	playerCard[1].cards[1]->changeCardWith(m_ucHandCardData1[1]);
	playerCard[1].cards[0]->setVisible(true);
	playerCard[1].cards[1]->setVisible(true);
	playerCard[0].cards[0]->setVisible(true);
	playerCard[0].cards[1]->setVisible(true);
	SetAnmionPlayOneCard();
}
void CardManager::showPlayerCard1( BYTE m_cbTableCardArray[2][3],BYTE m_cbCardCount[2])
{
	FreehomeThirdCard = false;
	cbBankerCount= GetCardListPip(m_cbTableCardArray[0],2);
	cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],2);
	// add by hxh 20160808
#if 1
    if (m_cbCardCount[0]==3)
#else
	if (cbBankerCount<=5 && cbPlayerTwoCardCount<8)
#endif
	{
		playerCard[0].cards[2]->changeCardWith(m_ucHandCardData[2]);
		playerCard[0].cards[2]->setVisible(true);
		schedule(schedule_selector(CardManager::SendCardToPlayerRuntion),2.0f);
		FreehomeThirdCard = true;
		m_cbBankerCount= GetCardListPip(m_cbTableCardArray[0],3);//闲家
	}

	BYTE cbPlayerThirdCardValue = 0 ;	//第三张牌点数
	//计算点数
//	cbPlayerThirdCardValue =GetCardPip(m_cbTableCardArray[0][2]);
	cbPlayerThirdCardValue =GetCardListPip(m_cbTableCardArray[0],2);
	BYTE m_cbPlayerTwoCardCount= 0; 

	// add by hxh 20160808
	if (m_cbCardCount[1]==3)
	{
	    showBankThirdCard(m_cbTableCardArray);
	}
	char buf1[50];
	char buf2[50];
	sprintf(buf1, "GAME_POINT_%d.png",cbPlayerTwoCardCount);
	m_pUserBankSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf1));
	Zhuang->setString(String::createWithFormat("%d",cbPlayerTwoCardCount)->getCString());//<by hxh 20160714>
	int ZXP=0;
	if (FreehomeThirdCard)
	{
		sprintf(buf2, "GAME_POINT_%d.png",m_cbBankerCount);
		m_pUserPlaySprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf2));
		Xian->setString(String::createWithFormat("%d",m_cbBankerCount)->getCString());//<by hxh 20160714>
		if(m_cbBankerCount>cbPlayerTwoCardCount)
			ZXP=1;
		else if(m_cbBankerCount==cbPlayerTwoCardCount)
			ZXP=2;

	}else
	{	
		sprintf(buf2, "GAME_POINT_%d.png",cbBankerCount);
		m_pUserPlaySprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf2));
		Xian->setString(String::createWithFormat("%d",cbBankerCount)->getCString());//<by hxh 20160714>
		if(cbBankerCount>cbPlayerTwoCardCount)
			ZXP=1;
		else if(cbBankerCount==cbPlayerTwoCardCount)
			ZXP=2;
	}
	m_ZXP=ZXP;

	auto pDelay = DelayTime::create(3.0f);
	auto pCall = CallFunc::create([=]()
	{
		m_pUserBankSprite->setVisible(true);
		m_pUserPlaySprite->setVisible(true);
	});
	auto pSeq = Sequence::create(pDelay, pCall, nullptr);
	this->runAction(pSeq);
	
	{
		//	用调度器会出现奔溃现象，SetPlayBankNumber中发送消息找不到奔溃
		auto pDelay = DelayTime::create(4.0f);
		auto pCall = CallFunc::create(CC_CALLBACK_0(CardManager::SetPlayBankNumber, this));
		auto pSeq = Sequence::create(pDelay, pCall, nullptr);
		this->runAction(pSeq);
	}
}

void CardManager::showBankThirdCard(BYTE m_cbTableCardArray[2][3])
{
	playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
	playerCard[1].cards[2]->setVisible(true);
	schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
	cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
}

void CardManager::SetPlayBankNumber()
{
	// add by hxh 20160811
	NotificationCenter::getInstance()->postNotification("AddGameRecord", Integer::create(m_ZXP));
	if (FreehomeThirdCard)
	{
		if (m_cbBankerCount>cbPlayerTwoCardCount){
			SetAnimation(Player,0);
		}else if (m_cbBankerCount<cbPlayerTwoCardCount){
			SetAnimation(Banker,1);
		}else
		{
			AndBureauSprite->setVisible(true);
			Banker->setVisible(false);
			Player->setVisible(false);
		}
	}else{
		if (cbBankerCount>cbPlayerTwoCardCount){
			SetAnimation(Player,0);
		}else if (cbBankerCount<cbPlayerTwoCardCount){
			SetAnimation(Banker,1);
		}else{
			AndBureauSprite->setVisible(true);
			Banker->setVisible(false);
			Player->setVisible(false);
		}
	}
}
void CardManager::updatePlayerCard(int chairID,unsigned char bHandCardData[3])
{
	playerCard[chairID].cards[0]->changeCardWith(bHandCardData[0]);
	playerCard[chairID].cards[1]->changeCardWith(bHandCardData[1]);
	playerCard[chairID].cards[2]->changeCardWith(bHandCardData[2]);
}
void CardManager::resetManager()
{
	XianZhuangBg->setVisible(false);//<by hxh>
	Player->setVisible(false);
	Banker->setVisible(false);
	m_pUserPlaySprite->setVisible(false);
	m_pUserBankSprite->setVisible(false);
	AndBureauSprite->setVisible(false);
	playerCard[0].cards[0]->changeCardWith(0);
	playerCard[0].cards[1]->changeCardWith(0);
	playerCard[0].cards[2]->changeCardWith(0);
	playerCard[1].cards[0]->changeCardWith(0);
	playerCard[1].cards[1]->changeCardWith(0);
	playerCard[1].cards[2]->changeCardWith(0);

	MyConfig& myConfig=MyConfig::Instance();
	//Vec2 pt=ccp(Winsize.width*0.8,Winsize.height);
	Vec2 pt=ccp(myConfig.m_playerCardPos1[0],myConfig.m_playerCardPos1[1]);
	playerCard[0].cards[0]->setPosition(pt);
	playerCard[0].cards[1]->setPosition(pt);
	playerCard[0].cards[2]->setPosition(pt);
	playerCard[1].cards[0]->setPosition(pt);
	playerCard[1].cards[1]->setPosition(pt);
	playerCard[1].cards[2]->setPosition(pt);
	playerCard[0].cards[0]->setVisible(false);
	playerCard[0].cards[1]->setVisible(false);
	playerCard[0].cards[2]->setVisible(false);
	playerCard[1].cards[0]->setVisible(false);
	playerCard[1].cards[1]->setVisible(false);
	playerCard[1].cards[2]->setVisible(false);
	memset(m_ucHandCardData,0,sizeof(m_ucHandCardData));
	memset(m_ucHandCardData1,0,sizeof(m_ucHandCardData1));
	//memset(m_bPlayerStatus,0,sizeof(m_bPlayerStatus));
	memset(m_ucDeskCardData,0,sizeof(m_ucDeskCardData));
	m_ZXP=0;
	Player->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_Player));
	Banker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_Banker));
	ScoreGameover->setString(String::createWithFormat("%d",0)->getCString());
	for (int Index = 0; Index < 8; Index++){
		PlayhomeTTf[Index]->setString(String::createWithFormat("%d",0)->getCString());
	}
}

void CardManager::HideCards()
{
	// add by hxh 20160808
#ifdef USE_PRINTTEST3		
	Test->setString("");
	Test2->setString("");
#endif
	XianZhuangBg->setVisible(false);
	Player->setVisible(false);
	Banker->setVisible(false);
	m_pUserPlaySprite->setVisible(false);
	m_pUserBankSprite->setVisible(false);
	AndBureauSprite->setVisible(false);
	playerCard[0].cards[0]->setVisible(false);
	playerCard[0].cards[1]->setVisible(false);
	playerCard[0].cards[2]->setVisible(false);
	playerCard[1].cards[0]->setVisible(false);
	playerCard[1].cards[1]->setVisible(false);
	playerCard[1].cards[2]->setVisible(false);
}

void CardManager::SetHandCardData( unsigned char bHandCardData[] )
{
	memcpy(m_ucHandCardData,bHandCardData,sizeof(m_ucHandCardData));
	m_ucHandCardData[0] = bHandCardData[0];
	m_ucHandCardData[1] = bHandCardData[1];
	m_ucHandCardData[2] = bHandCardData[2];
// add by hxh 20160806
#ifdef USE_PRINTTEST3
	string str=String::createWithFormat("%s,%s,%s",GetCardName(bHandCardData[0]),GetCardName(bHandCardData[1]),GetCardName(bHandCardData[2]))->getCString();
	MyNSString::GBKToUTF8(str);
	Test->setString(str);
#endif
}
void CardManager::SetHandCardData1( unsigned char bHandCardData1[] )
{
	memcpy(m_ucHandCardData1,bHandCardData1,sizeof(m_ucHandCardData1));
	m_ucHandCardData1[0] = bHandCardData1[0];
	m_ucHandCardData1[1] = bHandCardData1[1];
	m_ucHandCardData1[2] = bHandCardData1[2];
// add by hxh 20160806
#ifdef USE_PRINTTEST3
	string str=String::createWithFormat("%s,%s,%s",GetCardName(bHandCardData1[0]),GetCardName(bHandCardData1[1]),GetCardName(bHandCardData1[2]))->getCString();
	MyNSString::GBKToUTF8(str);
	Test2->setString(str);
#endif
}
BYTE CardManager::GetCardPip( BYTE cbCardData )
{
	//计算牌点
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbPipCount=(cbCardValue>=10)?0:cbCardValue;

	return cbPipCount;
}

BYTE CardManager::GetCardListPip( const BYTE cbCardData[], BYTE cbCardCount )
{
	//变量定义
	BYTE cbPipCount=0;

	//获取牌点
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbPipCount=(GetCardPip(cbCardData[i])+cbPipCount)%10;
	}
	return cbPipCount;
}

void CardManager::SetAnimation(Sprite *Sprite,int Number)
{
	char charBuffer[256] = {0};
	Vector<SpriteFrame *> CardManagerSprite;
	for(int i = 1; i < 7; i++)
	{
		memset(charBuffer, 0, sizeof(charBuffer));
		if (Number == 0)
		{
			sprintf(charBuffer,"WIN_XIAN_0%d.png",i);
		}else
		{
			sprintf(charBuffer,"WIN_ZHUANG_0%d.png",i);

		}
		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->spriteFrameByName(charBuffer);
		CardManagerSprite.pushBack(spriteFrame);
	}
	//执行动画函数
	Animation* fish01_catch_act= 
		Animation::createWithSpriteFrames(CardManagerSprite,0.1f);
	fish01_catch_act->setRestoreOriginalFrame(true);
	Animate* action = Animate::create(fish01_catch_act);
	CCFiniteTimeAction*  mFly= NULL;
	//更新
	if (Number == 0)
	{	
		mFly = Sequence::create(CCCallFuncO::create
			(this, callfuncO_selector(CardManager::afterCatchPalyerUsed),Sprite), NULL);
	}else
	{
		mFly = Sequence::create(CCCallFuncO::create
			(this, callfuncO_selector(CardManager::afterCatchBankUsed),Sprite), NULL);

	}
	Sprite->stopAllActions();
	Sprite->runAction(Sequence::create(action,mFly,NULL));
}
void CardManager::afterCatchPalyerUsed( Object *obj )
{
	Player->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_PlayerWin));//"BJL_27_4.png"
}

void CardManager::afterCatchBankUsed( Object *obj )
{
	Banker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_BankerWin));//"BJL_27_5.png"
}
void CardManager::SetAnmionPlayOneCard()
{
	MyConfig& myConfig=MyConfig::Instance();
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.2f,ccp(myConfig.m_PlayOneCardPos1[0],myConfig.m_PlayOneCardPos1[1]));
	CCFiniteTimeAction* actionMove2 =MoveTo::create( 0.2f,ccp(myConfig.m_PlayOneCardPos2[0],myConfig.m_PlayOneCardPos2[1]));
	CCFiniteTimeAction*  pCallSendCardBk = Sequence::create(CCCallFuncO::create
		(this, callfuncO_selector(CardManager::SetAnmionBankoneCard),playerCard[0].cards[0]), NULL);
	playerCard[0].cards[0]->runAction( Sequence::create(actionMove,actionMove2,pCallSendCardBk, NULL) );
	playSendCardSoundEffect();
}
void CardManager::SetAnmionBankoneCard( Object* obj )
{
	MyConfig& myConfig=MyConfig::Instance();
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.2f,ccp(myConfig.m_BankOneCardPos1[0],myConfig.m_BankOneCardPos1[1]));
	CCFiniteTimeAction* actionMove2 =MoveTo::create( 0.2f,ccp(myConfig.m_BankOneCardPos2[0],myConfig.m_BankOneCardPos2[1]));
	CCFiniteTimeAction* pCallSendCardBk = Sequence::create(CCCallFuncO::create
		(this, callfuncO_selector(CardManager::SetAnmionPlayTwoCard),playerCard[1].cards[0]), NULL);
	playerCard[1].cards[0]->runAction( Sequence::create(actionMove,actionMove2,pCallSendCardBk, NULL) );
	playSendCardSoundEffect();
}

void CardManager::SetAnmionPlayTwoCard( Object* obj )
{
	MyConfig& myConfig=MyConfig::Instance();
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.25f,ccp(myConfig.m_PlayOneCardPos1[0],myConfig.m_PlayOneCardPos1[1]));
	CCFiniteTimeAction* actionMove2 =MoveTo::create( 0.25f,ccp(myConfig.m_PlayOneCardPos2[0]+myConfig.m_CardOffsetX,myConfig.m_PlayOneCardPos2[1]));
	CCFiniteTimeAction* pCallSendCardBk = Sequence::create(CCCallFuncO::create
		(this, callfuncO_selector(CardManager::SetAnmionBankTwoCard),playerCard[0].cards[1]), NULL);
	playerCard[0].cards[1]->runAction( Sequence::create(actionMove,actionMove2,pCallSendCardBk, NULL) );
	playSendCardSoundEffect();
}
void CardManager::SetAnmionBankTwoCard( Object* obj )
{
	MyConfig& myConfig=MyConfig::Instance();
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.25f,ccp(myConfig.m_BankOneCardPos1[0],myConfig.m_BankOneCardPos1[1]));
	CCFiniteTimeAction* actionMove2 =MoveTo::create( 0.25f,ccp(myConfig.m_BankOneCardPos2[0]+myConfig.m_CardOffsetX,myConfig.m_BankOneCardPos2[1]));
	playerCard[1].cards[1]->runAction( Sequence::create(actionMove,actionMove2, NULL) );
	playSendCardSoundEffect();
}
void CardManager::SendCardToPlayerRuntion(float loat)
{
	MyConfig& myConfig=MyConfig::Instance();
	unschedule(schedule_selector(CardManager::SendCardToPlayerRuntion));
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.2f,ccp(myConfig.m_PlayOneCardPos1[0],myConfig.m_PlayOneCardPos1[1]));
	CCFiniteTimeAction* actionMove2 =MoveTo::create( 0.2f,ccp(myConfig.m_PlayOneCardPos2[0]+myConfig.m_CardOffsetX*2,myConfig.m_PlayOneCardPos2[1]));
	playerCard[0].cards[2]->runAction( Sequence::create(actionMove,actionMove2, NULL) );
	playSendCardSoundEffect();
}
void CardManager::SendCardToBankThirdCard(float loat)
{
	MyConfig& myConfig=MyConfig::Instance();
	unschedule(schedule_selector(CardManager::SendCardToBankThirdCard));
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.2f,ccp(myConfig.m_BankOneCardPos1[0],myConfig.m_BankOneCardPos1[1]));
	CCFiniteTimeAction* actionMove2 =MoveTo::create( 0.2f,ccp(myConfig.m_BankOneCardPos2[0]+myConfig.m_CardOffsetX*2,myConfig.m_BankOneCardPos2[1]));
	playerCard[1].cards[2]->runAction( Sequence::create(actionMove,actionMove2,NULL));
	playSendCardSoundEffect();
}

void CardManager::playSendCardSoundEffect()
{
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dispatch_card);
}

void CardManager::showGameOver()
{
	schedule(schedule_selector(CardManager::showGameOveAnmionTime),7.0f);
}
void CardManager::showGameOveAnmionTime( float dt )
{
	// add by hxh 20160720
	HideCards();
	// add by hxh 20160729
	LONGLONG lPlayAllScore=ATOI64(ScoreGameover->getString().c_str());
	if(lPlayAllScore>0){
		SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_end_win);
	}
	else if(lPlayAllScore<0){
		SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_end_lost); 
	}
	unschedule(schedule_selector(CardManager::showGameOveAnmionTime));
	MyConfig& myConfig=MyConfig::Instance();
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.2f,ccp(myConfig.m_GAMEoverPos2[0],myConfig.m_GAMEoverPos2[1]));
	GAMEoverSprite->runAction( Sequence::create(actionMove, NULL) );
}
void CardManager::remaggerDissGameOver()
{
	MyConfig& myConfig=MyConfig::Instance();
	CCFiniteTimeAction* actionMove =MoveTo::create( 0.2f,ccp(myConfig.m_GAMEoverPos1[0],myConfig.m_GAMEoverPos1[1]));
	GAMEoverSprite->runAction( Sequence::create(actionMove, NULL) );
}

void CardManager::SetScoreGameOver( LONGLONG lPlayScore,int Index, LONGLONG	lPlayAllScore)
{
	if (lPlayScore !=0)
	{
		PlayhomeTTf[Index]->setString(String::createWithFormat("%lld",lPlayScore)->getCString());
		ScoreGameover->setString(String::createWithFormat("%lld",lPlayAllScore)->getCString());
	}
}

void CardManager::showGameOverWithNoAnim( BYTE m_cbTableCardArray[2][3],BYTE m_cbCardCount[2])
{
	XianZhuangBg->setVisible(true);
	Player->setVisible(true);
	Banker->setVisible(true);
	m_pUserPlaySprite->setVisible(true);
	m_pUserBankSprite->setVisible(true);
	playerCard[0].cards[0]->changeCardWith(m_ucHandCardData[0]);
	playerCard[0].cards[1]->changeCardWith(m_ucHandCardData[1]);
	playerCard[1].cards[0]->changeCardWith(m_ucHandCardData1[0]);
	playerCard[1].cards[1]->changeCardWith(m_ucHandCardData1[1]);
	playerCard[1].cards[0]->setVisible(true);
	playerCard[1].cards[1]->setVisible(true);
	playerCard[0].cards[0]->setVisible(true);
	playerCard[0].cards[1]->setVisible(true);

	MyConfig& myConfig=MyConfig::Instance();
	//ccp(myConfig.m_PlayOneCardPos2[0],myConfig.m_PlayOneCardPos2[1])
	playerCard[0].cards[0]->setPosition(ccp(myConfig.m_PlayOneCardPos2[0],myConfig.m_PlayOneCardPos2[1]));
	playerCard[0].cards[1]->setPosition(ccp(myConfig.m_PlayOneCardPos2[0]+myConfig.m_CardOffsetX,myConfig.m_PlayOneCardPos2[1]));
	playerCard[1].cards[0]->setPosition(ccp(myConfig.m_BankOneCardPos2[0],myConfig.m_BankOneCardPos2[1]));
	playerCard[1].cards[1]->setPosition(ccp(myConfig.m_BankOneCardPos2[0]+myConfig.m_CardOffsetX,myConfig.m_BankOneCardPos2[1]));

	//-----------

	FreehomeThirdCard = false;
	cbBankerCount= GetCardListPip(m_cbTableCardArray[0],2);
	cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],2);

	if (m_cbCardCount[0]==3)
	{
		playerCard[0].cards[2]->changeCardWith(m_ucHandCardData[2]);
		playerCard[0].cards[2]->setPosition(ccp(myConfig.m_PlayOneCardPos2[0]+myConfig.m_CardOffsetX*2,myConfig.m_PlayOneCardPos2[1]));
		playerCard[0].cards[2]->setVisible(true);
		FreehomeThirdCard = true;
		m_cbBankerCount= GetCardListPip(m_cbTableCardArray[0],3);//闲家
	}

	BYTE cbPlayerThirdCardValue = 0 ;	//第三张牌点数
	cbPlayerThirdCardValue =GetCardListPip(m_cbTableCardArray[0],2);
	BYTE m_cbPlayerTwoCardCount= 0; 

	if (m_cbCardCount[1]==3)
	{
		playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
		playerCard[1].cards[2]->setPosition(ccp(myConfig.m_BankOneCardPos2[0]+myConfig.m_CardOffsetX*2,myConfig.m_BankOneCardPos2[1]));
		playerCard[1].cards[2]->setVisible(true);
		cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);  // 可能有问题
	}

	//========================

	char buf1[50];
	char buf2[50];
	sprintf(buf1, "GAME_POINT_%d.png",cbPlayerTwoCardCount);
	m_pUserBankSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf1));
	Zhuang->setString(String::createWithFormat("%d",cbPlayerTwoCardCount)->getCString());
	int ZXP=0;
	if (FreehomeThirdCard)
	{
		sprintf(buf2, "GAME_POINT_%d.png",m_cbBankerCount);
		m_pUserPlaySprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf2));
		Xian->setString(String::createWithFormat("%d",m_cbBankerCount)->getCString());
		if(m_cbBankerCount>cbPlayerTwoCardCount)
			ZXP=1;
		else if(m_cbBankerCount==cbPlayerTwoCardCount)
			ZXP=2;

	}else
	{	
		sprintf(buf2, "GAME_POINT_%d.png",cbBankerCount);
		m_pUserPlaySprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf2));
		Xian->setString(String::createWithFormat("%d",cbBankerCount)->getCString());
		if(cbBankerCount>cbPlayerTwoCardCount)
			ZXP=1;
		else if(cbBankerCount==cbPlayerTwoCardCount)
			ZXP=2;
	}
	m_ZXP=ZXP;

	//=============================================
	if (FreehomeThirdCard)
	{
		if (m_cbBankerCount>cbPlayerTwoCardCount){
			Player->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_PlayerWin));//"BJL_27_4.png"
		}else if (m_cbBankerCount<cbPlayerTwoCardCount){
			Banker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_BankerWin));//"BJL_27_5.png"
		}else
		{
			AndBureauSprite->setVisible(true);
			Banker->setVisible(false);
			Player->setVisible(false);
		}
	}else{
		if (cbBankerCount>cbPlayerTwoCardCount){
			Player->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_PlayerWin));//"BJL_27_4.png"
		}else if (cbBankerCount<cbPlayerTwoCardCount){
			Banker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_BankerWin));//"BJL_27_5.png"
		}else{
			AndBureauSprite->setVisible(true);
			Banker->setVisible(false);
			Player->setVisible(false);
		}
	}
}



