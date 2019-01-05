#include "DDZCardTableLayer.h"
#include "DDZSoundControl.h"
#include "DDZSceneView.h"
#include "DDZViewMenuLayer.h"
#include "DDZUserInfoManager.h"

DDZCardTableLayer::DDZCardTableLayer()
	:m_diFen(0)
{
	m_userLeft = nullptr;
	m_userLeft = nullptr;
	m_userSelf = nullptr;
}

DDZCardTableLayer::~DDZCardTableLayer()
{

}

bool DDZCardTableLayer::init()
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	//背景图片
	if(!Layer::init())
	{
		return false;
	}

	//右边玩家头像
	m_userRight = DDZUser::create();
	this->addChild(m_userRight);
	m_userRight->setAnchorPoint(ccp(1.0f,0.5f));
	m_userRight->setIconPos(ccp(size.width-60,size.height/2+80.0f));
	m_userRight->setUserInfoPos(ccp(size.width-80,size.height/2+200.0f));
	m_userRight->setVisible(true);
	m_userRight->setViewID(2);
	m_userRight->setUserChairId(2);

	//左边玩家头像
	m_userLeft = DDZUser::create();
	this->addChild(m_userLeft);
	m_userLeft->setAnchorPoint(ccp(0,0.5f));
	m_userLeft->setIconPos(ccp(60.0f,size.height/2+80.0f));
	m_userLeft->setUserInfoPos(ccp(80.0f,size.height/2+200.0f));
	m_userLeft->setVisible(true);
	m_userLeft->setViewID(0);
	m_userLeft->setUserChairId(0);

	//左边玩家头像
	m_userSelf = DDZUser::create();
	this->addChild(m_userSelf);
	m_userSelf->setAnchorPoint(ccp(0,0.5f));
	m_userSelf->setIconPos(ccp(72, 118));
	m_userSelf->setUserInfoPos(ccp(80.0f,200.0f));
	m_userSelf->setVisible(true);
	m_userSelf->setViewID(1);
	m_userSelf->setUserChairId(1);

	//	自身玩家准备
	m_pImgMiddleReady = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_img_text_ready);
	this->addChild(m_pImgMiddleReady);
	m_pImgMiddleReady->setPosition(Vec2(size.width * 0.5, size.height * 0.3));
	m_pImgMiddleReady->setVisible(false);

	//	玩家昵称
	m_nikeName = LabelTTF::create("","Helvetica-Bold",32);
	this->addChild(m_nikeName);
	m_nikeName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_nikeName->setColor(ccc3(255,204,0));
	m_nikeName->setPosition(Vec2(size.width * 0.106 + 80, size.height * 0.055));
	m_nikeName->setString(DDZUserInfoManager::getInstance()->m_UserName.c_str());

	//	数字——金币
	m_pLabNumGold = Label::createWithBMFont(DDZtexture_name::s_fntDDZGoldNum, "0");
	addChild(m_pLabNumGold);
	m_pLabNumGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabNumGold->setPosition(Vec2(size.width * 0.425 + 80, size.height * 0.05));
	m_coinNumber = DDZUserInfoManager::getInstance()->m_UserGold;
	m_pLabNumGold->setString(Value(m_coinNumber).asString());

	// 文字——倍数
	Sprite* pMultiple = Sprite::createWithSpriteFrameName(DDZtexture_name::s_text_multiple);
	addChild(pMultiple);
	pMultiple->setPosition(Vec2(size.width * 0.9, size.height * 0.94));
	cocos2d::Size sMulSize = pMultiple->getContentSize();

	//	数字——倍数
	m_pLabNumMultiple = Label::createWithBMFont(DDZtexture_name::s_fntDDZGoldNum, "1");
	pMultiple->addChild(m_pLabNumMultiple);
	m_pLabNumMultiple->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabNumMultiple->setPosition(Vec2(sMulSize.width * 0.6, sMulSize.height * 0.5));

	//	文字——底分
	Sprite * pImgDiFen = Sprite::createWithSpriteFrameName(DDZtexture_name::s_text_difen);
	this->addChild(pImgDiFen);
	pImgDiFen->setPosition(Vec2(size.width * 0.87, size.height * 0.86));
	cocos2d::Size sDiFenSize = pImgDiFen->getContentSize();

	//	数字——底分
	m_pLabNumDiFen = Label::createWithBMFont(DDZtexture_name::s_fntDDZGoldNum, "1");
	pImgDiFen->addChild(m_pLabNumDiFen);
	m_pLabNumDiFen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabNumDiFen->setPosition(Vec2(sDiFenSize.width * 1.226, sDiFenSize.height * 0.5));

	//排队动画
	m_pdNode = SpriteBatchNode::create(DDZtexture_name::s_cardTable);
	this->addChild(m_pdNode);
	m_pdNode->setPosition(ccp(size.width/2, size.height/2-50.0f));
	m_pdNode->setVisible(false);

	Sprite * paiziSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_paizi);
	m_pdNode->addChild(paiziSprite);
	paiziSprite->setAnchorPoint(ccp(0.5f,0));

	Sprite * zawuSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_zawu);
	m_pdNode->addChild(zawuSprite);
	zawuSprite->setAnchorPoint(ccp(0.5f,1.0f));

	//等待其他玩家中
	float runTime = 0.1f;
	Sprite * dengSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_deng);
	m_pdNode->addChild(dengSprite);
	dengSprite->setPosition(ccp(-40.0f,40.0f));
	dengSprite->runAction(RepeatForever::create(Sequence::create(MoveTo::create(runTime,ccp(dengSprite->getPositionX(),dengSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(dengSprite->getPositionX(),dengSprite->getPositionY())),DelayTime::create(6*runTime),NULL)));

	Sprite * daiSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_dai);
	m_pdNode->addChild(daiSprite);
	daiSprite->setPosition(ccp(dengSprite->getPositionX()+dengSprite->getContentSize().width,dengSprite->getPositionY()));
	daiSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(runTime),MoveTo::create(runTime,ccp(daiSprite->getPositionX(),daiSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(daiSprite->getPositionX(),daiSprite->getPositionY())),DelayTime::create(5*runTime),NULL)));

	Sprite * qiSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_qi);
	m_pdNode->addChild(qiSprite);
	qiSprite->setPosition(ccp(daiSprite->getPositionX()+daiSprite->getContentSize().width,dengSprite->getPositionY()));
	qiSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2*runTime),MoveTo::create(runTime,ccp(qiSprite->getPositionX(),qiSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(qiSprite->getPositionX(),qiSprite->getPositionY())),DelayTime::create(4*runTime),NULL)));

	Sprite * taSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_ta);
	m_pdNode->addChild(taSprite);
	taSprite->setPosition(ccp(qiSprite->getPositionX()+qiSprite->getContentSize().width,dengSprite->getPositionY()));
	taSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(3*runTime),MoveTo::create(runTime,ccp(taSprite->getPositionX(),taSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(taSprite->getPositionX(),taSprite->getPositionY())),DelayTime::create(3*runTime),NULL)));

	Sprite * wanSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_wan);
	m_pdNode->addChild(wanSprite);
	wanSprite->setPosition(ccp(taSprite->getPositionX()+taSprite->getContentSize().width,dengSprite->getPositionY()));
	wanSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(4*runTime),MoveTo::create(runTime,ccp(wanSprite->getPositionX(),wanSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(wanSprite->getPositionX(),wanSprite->getPositionY())),DelayTime::create(2*runTime),NULL)));

	Sprite * jiaSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_jia);
	m_pdNode->addChild(jiaSprite);
	jiaSprite->setPosition(ccp(wanSprite->getPositionX()+wanSprite->getContentSize().width,dengSprite->getPositionY()));
	jiaSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5*runTime),MoveTo::create(runTime,ccp(jiaSprite->getPositionX(),jiaSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(jiaSprite->getPositionX(),jiaSprite->getPositionY())),DelayTime::create(runTime),NULL)));

	Sprite * zhongSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_font_zhong);
	m_pdNode->addChild(zhongSprite);
	zhongSprite->setPosition(ccp(jiaSprite->getPositionX()+jiaSprite->getContentSize().width,dengSprite->getPositionY()));
	zhongSprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(6*runTime),MoveTo::create(runTime,ccp(zhongSprite->getPositionX(),zhongSprite->getPositionY()+10)),
		MoveTo::create(runTime,ccp(zhongSprite->getPositionX(),zhongSprite->getPositionY())),NULL)));

	return true;
}

void DDZCardTableLayer::setMultiplyingPower(int base,int add)
{
	//倍数
	m_times = /*base**/add;
	if (add == 0)
	{
		return;
	}
	m_pLabNumMultiple->setString(Value(add).asString());
}

void DDZCardTableLayer::setCoinNumber(int coinNum)
{
	if (coinNum<=0)
	{
		coinNum=0;
	}
	m_coinNumber = coinNum;
	m_pLabNumGold->setString(Value(coinNum).asString());
}  

void DDZCardTableLayer::waitingMatching(bool isWaiting)
{
	if (!isWaiting)
	{
		m_pdNode->setVisible(false);
	}else{
		m_pdNode->setVisible(true);
	}
}

void DDZCardTableLayer::setUserIsOnline(int userId,bool userIsOnline)
{
	switch(userId)
	{
	case 0:
		m_userLeft->setIsOnline(userIsOnline);
		break;
	case 2:
		m_userRight->setIsOnline(userIsOnline);
		break;
	case 1:
		m_userSelf->setIsOnline(userIsOnline);
		break;
		break;
	}
}

void DDZCardTableLayer::setUserInfor(DDZCCuserInfor* pInfor)
{
	DDZUser* pUser = getUserByUserID(pInfor->m_userInforID);
	
	if (pUser)
	{
		pUser->setUserInfor(pInfor);
	}
	else
	{
		switch(pInfor->m_ViewID)
		{
		case 0:
			m_userLeft->setUserInfor(pInfor);
			break;
		case 2:
			m_userRight->setUserInfor(pInfor);
			break;
		case 1:
			m_userSelf->setUserInfor(pInfor);
			break;
		default:
			break;
		}
	}
	
}

void DDZCardTableLayer::setLordIcon( int userId)
{
	m_userLeft->setLordIcon(userId==0?true:false);
	m_userRight->setLordIcon(userId==2?true:false);
	m_userSelf->setLordIcon(userId==1?true:false);
}

void DDZCardTableLayer::onEnter()
{
	Node::onEnter();
	DDZSoundControl::sharedDDZSoundControl()->setMusicVol(-1);//-1表示上次系统音量
	DDZSoundControl::sharedDDZSoundControl()->setSoundEffVol(-1);//
	DDZSoundControl::sharedDDZSoundControl()->playBackGroundMusic(DDZsound_name::s_bgMusic,true);
}

void DDZCardTableLayer::onExit()
{
	Node::onExit();
	//DDZSoundControl::sharedDDZSoundControl()->coloseBackGroundMusic();
}

void DDZCardTableLayer::onExitTransitionDidStart()
{
	Node::onExitTransitionDidStart();
	//
	DDZSoundControl::sharedDDZSoundControl()->coloseBackGroundMusic();
}

void DDZCardTableLayer::setNickName(const char * nikeName )
{
	m_nikeName->setString(nikeName);
}

void DDZCardTableLayer::setDiFen( int num )
{
	m_diFen = num;
	if (m_diFen == 0)
	{
		return;
	}
	m_pLabNumDiFen->setString(Value(m_diFen).asString());
}

void DDZCardTableLayer::dealGameReady()
{
	if (m_diFen == 0)
	{
		return;
	}
	string sZero = Value(m_diFen).asString();
	m_pLabNumDiFen->setString(sZero);
	m_pLabNumMultiple->setString(Value(1).asString());
}

void DDZCardTableLayer::dealGameOver()
{
}

DDZUser* DDZCardTableLayer::getUserByUserID(long int lnUserID)
{
	DDZUser* pUser = nullptr;
	if (lnUserID == m_userLeft->getUserID())
	{
		pUser = m_userLeft;
	}
	else if (lnUserID == m_userRight->getUserID())
	{
		pUser = m_userRight;
	}
	else if (lnUserID == m_userSelf->getUserID())
	{
		pUser = m_userSelf;
	}

	return pUser;
}

