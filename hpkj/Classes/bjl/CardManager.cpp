/***************************************
 牌管理类
 游戏界面
 xxxx 2013.1.5
 ***************************************/

#include "CardManager.h"
#include "CocosUnits.h"
#include "GlobalUnits.h"
#include "BJLRes.h"
#include "MyConfig.h"
#include "SoundControl.h"

using namespace cocos2d;

// 得分统计界面标签相对位置
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

		Player = Sprite::createWithSpriteFrame(spriteFrame("BJL_27_2.png"));
		Player->setPosition(ccp(myConfig.m_PlayerPos[0],myConfig.m_PlayerPos[1]));
		Player->setVisible(false);
		XianZhuangBg->addChild(Player);

		AndBureauSprite = Sprite::createWithSpriteFrame(spriteFrame("BJL_27_3.png"));
		AndBureauSprite->setAnchorPoint(Vec2::ZERO);
		AndBureauSprite->setPosition(ccp(myConfig.m_AndBureauPos[0],myConfig.m_AndBureauPos[1]));
		AndBureauSprite->setVisible(false);
	    XianZhuangBg->addChild(AndBureauSprite);

		Banker = Sprite::createWithSpriteFrame(spriteFrame("BJL_27_1.png"));
		Banker->setPosition(ccp(myConfig.m_BankerPos[0],myConfig.m_BankerPos[1]));
		Banker->setVisible(false);
		XianZhuangBg->addChild(Banker);

		//结束动画
		GAMEoverSprite = Sprite::create("bjl/background/GAME_END.png");
		GAMEoverSprite->setAnchorPoint(Vec2::ZERO);
		GAMEoverSprite->setPosition(ccp(myConfig.m_GAMEoverPos1[0],myConfig.m_GAMEoverPos1[1]));
		this->addChild(GAMEoverSprite,3);

		initPlayerCard();

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

		playerCard[i].cards[0]->setVisible(false);
		playerCard[i].cards[1]->setVisible(false);
		playerCard[i].cards[2]->setVisible(false);

		this->addChild(playerCard[i].cards[0],2-i);
		this->addChild(playerCard[i].cards[1],2-i);
		this->addChild(playerCard[i].cards[2],2-i);
	}

	MyConfig& myConfig=MyConfig::Instance();
	Vec2 pt=ccp(myConfig.m_playerCardPos1[0],myConfig.m_playerCardPos1[1]);
	playerCard[0].cards[0]->setPosition(pt);
	playerCard[0].cards[1]->setPosition(pt);
	playerCard[0].cards[2]->setPosition(pt);
	playerCard[1].cards[0]->setPosition(pt);
	playerCard[1].cards[1]->setPosition(pt);
	playerCard[1].cards[2]->setPosition(pt);
	memset(m_ucHandCardData,0,sizeof(m_ucHandCardData));
	memset(m_bPlayerStatus,0,sizeof(m_bPlayerStatus));
	memset(m_ucDeskCardData,0,sizeof(m_ucDeskCardData));
	for (int i = 0; i < 8; i++)
	{
		PlayhomeTTf[i]  =  LabelTTF::create("0","",16);
		PlayhomeTTf[i]->setAnchorPoint(Vec2::ZERO);
		PlayhomeTTf[i]->setColor(ccc3(173,118,0));
		PlayhomeTTf[i]->setPosition(ccp(myConfig.m_GAMEoverLblPos[i][0],myConfig.m_GAMEoverLblPos[i][1]));
		GAMEoverSprite->addChild(PlayhomeTTf[i],3);
	}
	ScoreGameover  =  LabelTTF::create("0","",16);
	ScoreGameover->setPosition(ccp(myConfig.m_GAMEoverLblPos[8][0],myConfig.m_GAMEoverLblPos[8][1]));
    ScoreGameover->setColor(ccc3(173,118,0));
	ScoreGameover->setAnchorPoint(Vec2::ZERO);
	GAMEoverSprite->addChild(ScoreGameover,3);

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
	SetAnmionPlayOneCard();
}
void CardManager::showPlayerCard1( BYTE m_cbTableCardArray[2][3],BYTE m_cbCardCount[2])
{
	FreehomeThirdCard = false;
	cbBankerCount= GetCardListPip(m_cbTableCardArray[0],2);
	cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],2);	
	if (cbBankerCount<=5 && cbPlayerTwoCardCount<8)
	{
		playerCard[0].cards[2]->changeCardWith(m_ucHandCardData[2]);
		playerCard[0].cards[2]->setVisible(true);
		schedule(schedule_selector(CardManager::SendCardToPlayerRuntion),2.0f);
		FreehomeThirdCard = true;
		m_cbBankerCount= GetCardListPip(m_cbTableCardArray[0],3);//闲家

	}

	BYTE cbPlayerThirdCardValue = 0 ;	//第三张牌点数
	//计算点数
	cbPlayerThirdCardValue =GetCardListPip(m_cbTableCardArray[0],2);
	BYTE m_cbPlayerTwoCardCount= 0; 

	//闲一点 庄七点  闲拿了第三张牌  庄不拿牌出现问题
	if (cbBankerCount< 8 && cbPlayerTwoCardCount<8)
	{
		switch(cbBankerCount)
		{
		case 0:
			if (FreehomeThirdCard == true &&cbPlayerThirdCardValue !=7 && cbPlayerThirdCardValue !=6)
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
		case 1:
			if (FreehomeThirdCard == true &&cbPlayerThirdCardValue !=7&&cbPlayerThirdCardValue !=4&&cbPlayerThirdCardValue !=6)
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
		case 2:
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
		case 3:
			if(FreehomeThirdCard == true && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 4)
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);

			}
			else if (FreehomeThirdCard == false) 
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);

			}			
			break;
		case 4:
			if(FreehomeThirdCard == true && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 && cbPlayerThirdCardValue != 0 &&cbPlayerThirdCardValue !=4)
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			else if ( FreehomeThirdCard == false) 
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
		case 5:
			if(FreehomeThirdCard == true && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 2 
				&& cbPlayerThirdCardValue != 3  && cbPlayerThirdCardValue != 8 &&
				cbPlayerThirdCardValue != 9 &&  cbPlayerThirdCardValue != 0 &&cbPlayerThirdCardValue !=6)
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			else if (FreehomeThirdCard == false) 
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
		case 6:
			if( FreehomeThirdCard == true && ( cbPlayerThirdCardValue == 6 || cbPlayerThirdCardValue == 7 ))
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}else if (FreehomeThirdCard == false &&cbPlayerTwoCardCount !=7)
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.0f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
		case 7:
			if( FreehomeThirdCard == true && ( cbPlayerThirdCardValue == 6 ))
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}else if (FreehomeThirdCard == false )
			{
				playerCard[1].cards[2]->changeCardWith(m_ucHandCardData1[2]);
				playerCard[1].cards[2]->setVisible(true);
				schedule(schedule_selector(CardManager::SendCardToBankThirdCard),2.50f);
				cbPlayerTwoCardCount =GetCardListPip(m_cbTableCardArray[1],3);
			}
			break;
			//不须补牌
		default:
			break;
		}
	}
	char buf1[50];
	char buf2[50];
	sprintf(buf1, "GAME_POINT_%d.png",cbPlayerTwoCardCount);
	m_pUserBankSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf1));
	Zhuang->setString(String::createWithFormat("%d",cbPlayerTwoCardCount)->getCString());//< 20160714>
	int ZXP=0;
	if (FreehomeThirdCard)
	{
		sprintf(buf2, "GAME_POINT_%d.png",m_cbBankerCount);
		m_pUserPlaySprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf2));
		Xian->setString(String::createWithFormat("%d",m_cbBankerCount)->getCString());//< 20160714>
		if(m_cbBankerCount>cbPlayerTwoCardCount)
			ZXP=1;
		else if(m_cbBankerCount==cbPlayerTwoCardCount)
			ZXP=2;

	}else
	{	
		sprintf(buf2, "GAME_POINT_%d.png",cbBankerCount);
		m_pUserPlaySprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(buf2));
		Xian->setString(String::createWithFormat("%d",cbBankerCount)->getCString());//< 20160714>
		if(cbBankerCount>cbPlayerTwoCardCount)
			ZXP=1;
		else if(cbBankerCount==cbPlayerTwoCardCount)
			ZXP=2;
	}
	NotificationCenter::getInstance()->postNotification("AddGameRecord", Integer::create(ZXP));
	schedule(schedule_selector(CardManager::SetPlayBankNumber),4.0f);
}
void CardManager::SetPlayBankNumber( float flat )
{
	unschedule(schedule_selector(CardManager::SetPlayBankNumber));
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
	XianZhuangBg->setVisible(false);
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
	memset(m_bPlayerStatus,0,sizeof(m_bPlayerStatus));
	memset(m_ucDeskCardData,0,sizeof(m_ucDeskCardData));
	Player->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_Player));
	Banker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_Banker));
	ScoreGameover->setString(String::createWithFormat("%d",0)->getCString());
	for (int Index = 0; Index < 8; Index++){
		PlayhomeTTf[Index]->setString(String::createWithFormat("%d",0)->getCString());
	}
}

void CardManager::HideCards()
{
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
}
void CardManager::SetHandCardData1( unsigned char bHandCardData1[] )
{
	memcpy(m_ucHandCardData1,bHandCardData1,sizeof(m_ucHandCardData1));
	m_ucHandCardData1[0] = bHandCardData1[0];
	m_ucHandCardData1[1] = bHandCardData1[1];
	m_ucHandCardData1[2] = bHandCardData1[2];
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
	Player->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("BJL_27_4.png"));
}

void CardManager::afterCatchBankUsed( Object *obj )
{
	Banker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("BJL_27_5.png"));
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
	HideCards();
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


