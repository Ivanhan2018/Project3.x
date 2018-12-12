#include "CardTableLayer.h"
#include "SoundControl.h"
#include "SceneView.h"
#include "GlobalUnits.h"
#include "CocosUnits.h"
#include "MyNSString.h"
#include "MyConfig.h"
#include "EntityMgr.h"

enum
{		
	YANWU_PARTICLE = -77, // 烟雾粒子效果
};

CardTableLayer::CardTableLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CardTableLayer::SetPlaceLandScore), "UpdatePlacelBankScore", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CardTableLayer::AddGameRecord), "AddGameRecord", NULL);
#ifdef USE_PRINTTEST
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CardTableLayer::PrintTest), "PrintTest", NULL);
#endif
#ifdef USE_PRINTTEST2
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CardTableLayer::PrintTest2), "PrintTest2", NULL);
#endif
}
CardTableLayer::~CardTableLayer()
{
	NotificationCenter::getInstance()->removeObserver(this, "UpdatePlacelBankScore");
	NotificationCenter::getInstance()->removeObserver(this, "AddGameRecord");
#ifdef USE_PRINTTEST
	NotificationCenter::getInstance()->removeObserver(this, "PrintTest");
#endif
#ifdef USE_PRINTTEST2
	NotificationCenter::getInstance()->removeObserver(this, "PrintTest2");
#endif
}

bool CardTableLayer::init()
{
	size = Director::getInstance()->getWinSize();
	//背景图片
	if(!Node::init())
	{
		return false;
	}
	Sprite *sptBack = Sprite::create("bjl/BJL/BJL_02.png");
	sptBack->setAnchorPoint(Vec2::ZERO);
	sptBack->setPosition(ccp(0,0));
	this->addChild(sptBack);
	
	MyConfig& myConfig=MyConfig::Instance();
	int YPos=myConfig.m_GameRecordPos[1];

	//游戏记录
	SpriteGameRecord = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_GameRecord));
	SpriteGameRecord->setAnchorPoint(Vec2::ZERO);
	SpriteGameRecord->ignoreAnchorPointForPosition(true);
	SpriteGameRecord->setPosition(ccp(myConfig.m_GameRecordPos[0],myConfig.m_GameRecordPos[1]));
	this->addChild(SpriteGameRecord);

	m_pWinLostRecordBatch = SpriteBatchNode::create(texture_name::s_cardtable);
	this->addChild(m_pWinLostRecordBatch);

	Sprite *SpriteBankerUser = Sprite::create(texture_name::s_BankerUser);
	SpriteBankerUser->setAnchorPoint(Vec2::ZERO);
	SpriteBankerUser->setPosition(ccp(myConfig.m_BankerUserPos[0],myConfig.m_BankerUserPos[1]));
	this->addChild(SpriteBankerUser);

	//玩家信息
	int X=myConfig.m_MyNamePos[0];
	int Y=myConfig.m_MyNamePos[1],dY=35;
	int m_cbGender = g_GlobalUnits.getGolbalUserData()->cbGender;
	if (m_cbGender == 1)
	{
		SpriteMyUsed = Sprite::createWithSpriteFrame(spriteFrame("head_4.png"));
	}else
	{
		SpriteMyUsed = Sprite::createWithSpriteFrame(spriteFrame("min_4.png"));

	}
	SpriteMyUsed->setAnchorPoint(Vec2::ZERO);
	SpriteMyUsed->setPosition(ccp(myConfig.m_MyPicPos[0],myConfig.m_MyPicPos[1]));
	this->addChild(SpriteMyUsed);
	string szText2 = g_GlobalUnits.getGolbalUserData()->szNickName;
	m_nikeName = LabelTTF::create(szText2.c_str(),"Helvetica-Bold",20);
	m_nikeName->setAnchorPoint(Vec2::ZERO);
	m_nikeName->setColor(ccc3(255,204,0));
	m_nikeName->setPosition(ccp(myConfig.m_MyNamePos[0],myConfig.m_MyNamePos[1]));
	this->addChild(m_nikeName,1);

	//金币数字
	szText0 = g_GlobalUnits.getGolbalUserData()->lScore;
	string szText1 =addCommaToNumber(szText0);
	m_coinNumberAtlas = LabelTTF::create(szText1.c_str(),"Helvetica-Bold",20);
	m_coinNumberAtlas->setAnchorPoint(ccp(0,0));
	m_coinNumberAtlas->setColor(ccc3(255,204,0));
	m_coinNumberAtlas->setPosition(ccp(myConfig.m_MyGoldPos[0],myConfig.m_MyGoldPos[1]));
	this->addChild(m_coinNumberAtlas);

	m_Chengji  = LabelTTF::create("0","Helvetica-Bold",20);
	m_Chengji->setAnchorPoint(Vec2::ZERO);
	m_Chengji->setColor(ccc3(255,204,0));
	m_Chengji->setPosition(ccp(myConfig.m_MyChengjiPos[0],myConfig.m_MyChengjiPos[1]));
	this->addChild(m_Chengji);

	float X1=myConfig.m_BankerNamePos[0];
	float Y1=myConfig.m_BankerNamePos[1];
	float dY1=40;
	float dX1=180;
	//显示庄家数目
	// 庄家头像、庄家名字、成绩、金币、局数等UI对象的绝对坐标改为相对坐标
	FinUsedSprite = Sprite::createWithSpriteFrame(spriteFrame("head_1.png"));
	FinUsedSprite->setAnchorPoint(ccp(0,0));
	FinUsedSprite->setScale(0.7);
	FinUsedSprite->setPosition(ccp(myConfig.m_BankerPicPos[0],myConfig.m_BankerPicPos[1]));
	SpriteBankerUser->addChild(FinUsedSprite);

	laberName=LabelTTF::create("","",16);
	laberName->setAnchorPoint(ccp(0,0));
	laberName->setColor(ccc3(173,118,0));
	laberName->setPosition(ccp(myConfig.m_BankerNamePos[0],myConfig.m_BankerNamePos[1]));
	SpriteBankerUser->addChild(laberName);

#ifdef USE_PRINTTEST
	Test=LabelTTF::create("","",16);
	Test->setAnchorPoint(ccp(0,0));
	Test->setColor(ccc3(173,118,0));
	Test->setPosition(ccp(size.width*0.1,size.height*0.7));
	this->addChild(Test);
#endif
#ifdef USE_PRINTTEST2
	Test2=LabelTTF::create("","",16);
	Test2->setAnchorPoint(ccp(0,0));
	Test2->setColor(ccc3(173,118,0));
	Test2->setPosition(ccp(size.width*0.6,size.height*0.7));
	this->addChild(Test2);
#endif

	goldname=LabelTTF::create("","",16);
	goldname->setAnchorPoint(Vec2::ZERO);
	goldname->setColor(ccc3(173,118,0));
	goldname->setPosition(ccp(myConfig.m_BankerGoldPos[0],myConfig.m_BankerGoldPos[1]));
	SpriteBankerUser->addChild(goldname);

	ChengjiScore=LabelTTF::create("0","",16);
	ChengjiScore->setAnchorPoint(Vec2::ZERO);
	ChengjiScore->setColor(ccc3(173,118,0));
	ChengjiScore->setPosition(ccp(myConfig.m_BankerChengjiPos[0],myConfig.m_BankerChengjiPos[1]));
	SpriteBankerUser->addChild(ChengjiScore);

	JiNumber = LabelTTF::create("0","",16);
	JiNumber->setAnchorPoint(Vec2::ZERO);
	JiNumber->setColor(ccc3(173,118,0));
	JiNumber->setPosition(ccp(myConfig.m_BankerJiNumberPos[0],myConfig.m_BankerJiNumberPos[1]));
	SpriteBankerUser->addChild(JiNumber);

	FinUsedSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("head_2.png"));
	setUserIsOnline(INVALID_CHAIR,0,0,0);

	//排队动画
	m_pdNode = SpriteBatchNode::create(texture_name::s_cardTable1);//<>
	m_pdNode->setPosition(ccp(myConfig.m_PaiZiPos[0],myConfig.m_PaiZiPos[1]));
	Sprite* paiziSprite = Sprite::createWithSpriteFrameName(texture_name::s_paizi);
	paiziSprite->setAnchorPoint(ccp(0.5f,0));

	//等待其他玩家中
	Sprite * dengSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_deng);
	dengSprite->setPosition(ccp(-40.0f,40.0f));
	float runTime = 0.1f;
	dengSprite->runAction(RepeatForever::create(Sequence::create(MoveTo::create(runTime,ccp(dengSprite->getPositionX(),dengSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(dengSprite->getPositionX(),dengSprite->getPositionY())),DelayTime::create(6*runTime),NULL)));

	m_pdNode->addChild(dengSprite);
	Sprite * daiSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_dai);
	daiSprite->setPosition(ccp(dengSprite->getPositionX()+dengSprite->getContentSize().width,dengSprite->getPositionY()));
	daiSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(runTime),MoveTo::create(runTime,ccp(daiSprite->getPositionX(),daiSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(daiSprite->getPositionX(),daiSprite->getPositionY())),DelayTime::create(5*runTime),NULL)));

	m_pdNode->addChild(daiSprite);
	Sprite * qiSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_qi);
	qiSprite->setPosition(ccp(daiSprite->getPositionX()+daiSprite->getContentSize().width,dengSprite->getPositionY()));
	qiSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2*runTime),MoveTo::create(runTime,ccp(qiSprite->getPositionX(),qiSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(qiSprite->getPositionX(),qiSprite->getPositionY())),DelayTime::create(4*runTime),NULL)));

	m_pdNode->addChild(qiSprite);
	Sprite * taSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_ta);
	taSprite->setPosition(ccp(qiSprite->getPositionX()+qiSprite->getContentSize().width,dengSprite->getPositionY()));
	taSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(3*runTime),MoveTo::create(runTime,ccp(taSprite->getPositionX(),taSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(taSprite->getPositionX(),taSprite->getPositionY())),DelayTime::create(3*runTime),NULL)));

	m_pdNode->addChild(taSprite);
	Sprite * wanSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_wan);
	wanSprite->setPosition(ccp(taSprite->getPositionX()+taSprite->getContentSize().width,dengSprite->getPositionY()));
	wanSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(4*runTime),MoveTo::create(runTime,ccp(wanSprite->getPositionX(),wanSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(wanSprite->getPositionX(),wanSprite->getPositionY())),DelayTime::create(2*runTime),NULL)));

	m_pdNode->addChild(wanSprite);
	Sprite * jiaSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_jia);
	jiaSprite->setPosition(ccp(wanSprite->getPositionX()+wanSprite->getContentSize().width,dengSprite->getPositionY()));
	jiaSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5*runTime),MoveTo::create(runTime,ccp(jiaSprite->getPositionX(),jiaSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(jiaSprite->getPositionX(),jiaSprite->getPositionY())),DelayTime::create(runTime),NULL)));

	m_pdNode->addChild(jiaSprite);
	Sprite * zhongSprite = Sprite::createWithSpriteFrameName(texture_name::s_font_zhong);
	zhongSprite->setPosition(ccp(jiaSprite->getPositionX()+jiaSprite->getContentSize().width,dengSprite->getPositionY()));
	zhongSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(6*runTime),MoveTo::create(runTime,ccp(zhongSprite->getPositionX(),zhongSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(zhongSprite->getPositionX(),zhongSprite->getPositionY())),NULL)));
	m_pdNode->addChild(zhongSprite);

	Sprite * A8 = Sprite::createWithSpriteFrameName(texture_name::s_A_8);
	A8->setPosition(ccp(zhongSprite->getPositionX()+zhongSprite->getContentSize().width,dengSprite->getPositionY()));
	A8->runAction(RepeatForever::create(Sequence::create(DelayTime::create(7*runTime),MoveTo::create(runTime,ccp(A8->getPositionX(),A8->getPositionY()+10)),
		MoveTo::create(runTime,ccp(A8->getPositionX(),A8->getPositionY())),NULL)));
	m_pdNode->addChild(A8);

	this->addChild(m_pdNode);

	//排队粒子效果
	ParticleSystemQuad * yanwu_particle = ParticleSystemQuad::create("bjl/hlddz_particle/souren.plist");
	yanwu_particle->setPosition(ccp(myConfig.m_YanwuParticlePos[0],myConfig.m_YanwuParticlePos[1]));
	this->addChild(yanwu_particle,100,YANWU_PARTICLE);
	SetUpdate();
	hnnnnnnn = false;
	return true;
}
void CardTableLayer::waitingMatching(bool isWaiting)
{
	if (!isWaiting)
	{
		this->removeChild(m_pdNode);
		this->getChildByTag(YANWU_PARTICLE)->setVisible(false);
	}else{
		m_pdNode->setVisible(true);
		this->getChildByTag(YANWU_PARTICLE)->setVisible(true);
	}
}

bool CardTableLayer::setBankerName(WORD wBankerUser)
{
	if(wBankerUser==INVALID_CHAIR)
	{
		string str = "系统坐庄";
		MyNSString::GBKToUTF8(str);
		laberName->setString(str.c_str());
		return false;
	}
	else
	{
	    CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItemByChairID(wBankerUser);
		if (!pUserItem)
		{
			return false;
		}
		DWORD dwUserID=pMsgDispatch->m_dwUserID;
		string str =pUserItem->getUserNickName();
		if(pUserItem->getUserID()==dwUserID)
		{
		    str = "由您坐庄";
		}
		MyNSString::GBKToUTF8(str);
		laberName->setString(str.c_str());
		return false;
	}
}

void CardTableLayer::setUserIsOnline(WORD wBankerUser,LONGLONG lBankerScore,LONGLONG lBankerWinScore,WORD wBankerTime)
{
	setBankerName(wBankerUser);
	goldname->setString(String::createWithFormat("%lld",lBankerScore)->getCString());
	ChengjiScore->setString(String::createWithFormat("%lld",lBankerWinScore)->getCString());
	JiNumber->setString(String::createWithFormat("%d",wBankerTime)->getCString());
	goldname->setVisible(true);
	ChengjiScore->setVisible(true);
	JiNumber->setVisible(true);
	FinUsedSprite->setVisible(true);
}
//系统坐庄的时候
void CardTableLayer::setUserIcon( int userId,int gender,char* name,int gold,char* huafeidian )
{
	if (!hnnnnnnn)
	{
		if (gender == 1)
		{
			FinUsedSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("head_2.png"));
		}else
		{
			FinUsedSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("min_2.png"));
		}
		laberName->setString(name);
		goldname->setString(String::createWithFormat("%d",gold)->getCString());
		hnnnnnnn = true;
	}
	laberName->setVisible(true);
	goldname->setVisible(true);
	ChengjiScore->setVisible(true);
	JiNumber->setVisible(true);
	FinUsedSprite->setVisible(true);
}

void CardTableLayer::onEnter()
{
	Node::onEnter();
	SoundControl::sharedSoundControl()->setMusicVol(-1);//-1表示上次系统音量
	SoundControl::sharedSoundControl()->setSoundEffVol(-1);
	SoundControl::sharedSoundControl()->playBackGroundMusic(sound_name::s_bgMusic,true);
}

void CardTableLayer::onExit()
{
	Node::onExit();
	SoundControl::sharedSoundControl()->coloseBackGroundMusic();
}

String CardTableLayer::getDateNow()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	timeval now;
	gettimeofday(&now, NULL);
	struct tm *tm;
	time_t tp = now.tv_sec;
	tm = localtime(&tp);
	int year = (int)tm->tm_year+1900;
	int mon = (int)tm->tm_mon+1;
	int day = (int)tm->tm_mday;
	int min = (int)tm->tm_min;
	int hour = (int)tm->tm_hour;
	int sec = (int)tm->tm_sec;
	String str;
	str.initWithFormat("%02d:%02d",hour,min);
	return str;
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	struct tm * tm;
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
	int year = (int)tm->tm_year+1900;
	int mon = (int)tm->tm_mon+1;
	int day = (int)tm->tm_mday;
	int min = (int)tm->tm_min;
	int hour = (int)tm->tm_hour;
	int sec = (int)tm->tm_sec;
	String str;
	str.initWithFormat("%02d:%02d",hour,min);
	return str;
#endif
}

void CardTableLayer::setNickName(const char * nikeName )
{
	m_nikeName->setString(nikeName);
}

void CardTableLayer::upHaiou(float ft)
{
}

void CardTableLayer::haiouCall( Object* _obj )
{
	Node*node=(Node*)_obj;
	if(node)node->removeFromParent();
}

void CardTableLayer::setBankerScore( LONGLONG Score )
{
	goldname->setString(String::createWithFormat("%lld",Score)->getCString());
}

void CardTableLayer::setBankUsedChengjiORrJuNumber( LONGLONG Chengji,LONGLONG JuNumber )
{
	ChengjiScore->setString(String::createWithFormat("%lld",Chengji)->getCString());
	JiNumber->setString(String::createWithFormat("%lld",JuNumber)->getCString());
}

void CardTableLayer::setPlayAllScoreORchengji(LONGLONG Chengji )
{
	szText0+=Chengji;
	m_coinNumberAtlas->setString(String::createWithFormat("%lld",szText0)->getCString());
	m_Chengji->setString(String::createWithFormat("%lld",Chengji)->getCString());
}

void CardTableLayer::SetPlaceLandScore( Object *obj )
{
	long ManNumber = ((Integer *)obj)->getValue();
	szText0-=ManNumber;
	m_coinNumberAtlas->setString(String::createWithFormat("%d",szText0)->getCString());
	NotificationCenter::getInstance()->postNotification("UpdatePlacelinitUnClickTouch", Integer::create(szText0));
}

void CardTableLayer::AddGameRecord(Object *obj)
{
	int ZXP = ((Integer *)obj)->getValue();
	AddWinLostResult(ZXP);
}

#ifdef USE_PRINTTEST
void CardTableLayer::PrintTest( Object *obj )
{
	Test->setString(((String *)obj)->getCString());
	scheduleOnce(schedule_selector(CardTableLayer::PrintTest_callback_once),1);
}

void CardTableLayer::PrintTest_callback_once(float t)
{
	Test->setString("");
}
#endif

#ifdef USE_PRINTTEST2
void CardTableLayer::PrintTest2( Object *obj )
{
	Test2->setString(((String *)obj)->getCString());
	scheduleOnce(schedule_selector(CardTableLayer::PrintTest2_callback_once),1);
}

void CardTableLayer::PrintTest2_callback_once(float t)
{
	Test2->setString("");
}
#endif

void CardTableLayer::initScrollView( BYTE m_cbPlayerCount ,BYTE m_cbBankerCount )
{
	int ZXP=0;
	if(m_cbPlayerCount>m_cbBankerCount)
		ZXP=1;
	else if(m_cbPlayerCount==m_cbBankerCount)
		ZXP=2;
	AddWinLostResult(ZXP);
}

void CardTableLayer::AddWinLostResult(int ZXP)
{
	int i=m_vWinLostRecord.size();
	if(i>=MAX_RECORD_NUM){
	    m_vWinLostRecord.pop_front();
		i--;
		m_pWinLostRecordBatch->removeAllChildren();
		for(int j=0;j<i;j++)
		{
		   SetWinLostResult(j,m_vWinLostRecord[j]);
		}
	}
	SetWinLostResult(i,ZXP);
	m_vWinLostRecord.push_back(ZXP);
}

void CardTableLayer::SetWinLostResult(int i,int ZXP)
{
	MyConfig& myConfig=MyConfig::Instance();
	unsigned int uCount=m_pWinLostRecordBatch->getChildrenCount();
	const Vec2 ptSign = SpriteGameRecord->getPosition();
	float fOffsetX = myConfig.m_fOffsetX; // 横向间隔
	float fStartX = myConfig.m_fStartX; // 横向起始位置X
	float fOffsetY = myConfig.m_fOffsetY; // 纵向间隔
	float fStartY = myConfig.m_fStartY; // 庄纵向起始位置Y

	Sprite *pZXP = Sprite::createWithSpriteFrameName(texture_name::s_ZXPGou);
	pZXP->ignoreAnchorPointForPosition(true);
	pZXP->setPosition(ccp(ptSign.x+fStartX+fOffsetX*i-(1.8+i/10)*(i/5),fStartY-fOffsetY*ZXP));
	m_pWinLostRecordBatch->addChild(pZXP);
}

void CardTableLayer::SetUpdate()
{
	CCLabelAtlas *PlayhomeTTf7  = CCLabelAtlas::create("000000",texture_name::s_ChipMagerNumber,13,17,'0');//<>
	PlayhomeTTf7->setAnchorPoint(Vec2::ZERO);
	PlayhomeTTf7->setPosition(ccp(SpriteGameRecord->getPositionX(),SpriteGameRecord->getPositionY()));
	SpriteGameRecord->addChild(PlayhomeTTf7,2);

}



