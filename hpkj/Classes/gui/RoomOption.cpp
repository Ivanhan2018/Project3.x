//
//  RegistScene.cpp
//  Game
//
//  Created by 兰 天乐 on 13-6-7.
//
//

#include "RoomOption.h"
#include "CocosUnits.h"
#include "cocos-ext.h"
#include "AnsString.h"
#include "utf-8.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "LoginLayer.h"
#include "MovingLabelLayer.h"
#include "FreeLayer.h"
#include "OverRegistLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

USING_NS_CC;

RoomOption::RoomOption()
{
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(RoomOption::onCheckVersion), MSG_UI_ANS_VERSIONNEW, NULL);
}

RoomOption::~RoomOption()
{
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_VERSIONNEW);
}

bool RoomOption::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
	//背景
	Sprite* spriteBack0 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
	Sprite* spriteBack1 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
	spriteBack0->setPosition(Vec2(spriteBack0->getContentSize().width * 0.5,winSize.height * 0.5));
	spriteBack1->setPosition(Vec2(winSize.width - spriteBack1->getContentSize().width * 0.5-2,winSize.height * 0.5));
	spriteBack1->setFlipX(true);
	this->addChild(spriteBack0);
	this->addChild(spriteBack1);

	//免费
	Sprite *pMianFeiSpriteNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
	Sprite *pMianFeiSpriteSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
	MenuItemSprite* pMianfei = MenuItemSprite::create(pMianFeiSpriteNormal,pMianFeiSpriteSelect,CC_CALLBACK_1(RoomOption::toFreeLayer,this));
	pMianFeiSpriteSelect->setScale(0.9f);
	pMianFeiSpriteSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* mianfeiMenu = Menu::create(pMianfei,NULL);
	mianfeiMenu->setPosition(Vec2(winSize.width*0.054,winSize.height*0.885));
	this->addChild(mianfeiMenu,1);

	//返回
	Sprite* backNormal= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	Sprite* backSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	MenuItemSprite* backItemSprite = MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(RoomOption::back,this));
	backSelect->setScale(0.9f);
	backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* backMenu= Menu::create(backItemSprite,NULL);
	backMenu->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93));
	this->addChild(backMenu);

	//标题
	Sprite* spriteTitleBack = Sprite::createWithSpriteFrame(spriteFrame("dt_register_14.png"));
	Sprite* spriteTitle = Sprite::createWithSpriteFrame(spriteFrame("dt_option_0.png"));
	spriteTitleBack->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.92));
	spriteTitle->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.93));
	this->addChild(spriteTitleBack);
	this->addChild(spriteTitle);

	//底框
	Sprite* spriteBackBox0 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg")); 
	Sprite* spriteBackBox1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
	spriteBackBox0->setPosition(Vec2(winSize.width * 0.5 - spriteBackBox0->getContentSize().width * 0.5,spriteBackBox0->getContentSize().height * 0.57));
	spriteBackBox1->setPosition(Vec2(winSize.width * 0.5 + spriteBackBox1->getContentSize().width * 0.5 - 2,spriteBackBox1->getContentSize().height * 0.57));
	spriteBackBox0->setScaleY(1.12f);
	spriteBackBox1->setScaleY(1.12f);
	spriteBackBox1->setFlipX(true);
	this->addChild(spriteBackBox0);
	this->addChild(spriteBackBox1);

	//游客账号栏
	Sprite* spriteLableBack0 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	Sprite* spriteLableBack1 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	spriteLableBack0->setPosition(Vec2(winSize.width * 0.5 - spriteLableBack0->getContentSize().width * 0.5,winSize.height * 0.7));
	spriteLableBack1->setPosition(Vec2(winSize.width * 0.5 + spriteLableBack1->getContentSize().width * 0.5-1,winSize.height * 0.7));
	spriteLableBack1->setFlipX(true);
	this->addChild(spriteLableBack0);
	this->addChild(spriteLableBack1);
	LabelTTF* pPlayerAccountTips;
	if(g_GlobalUnits.getGolbalUserData() && g_GlobalUnits.getGolbalUserData()->cbVisitor != 0)
	{
		pPlayerAccountTips = LabelTTF::create(pConfigMgr->text("t131"),"",22);
	}
	else
	{
		pPlayerAccountTips = LabelTTF::create(pConfigMgr->text("t309"),"",22);
	}
	pPlayerAccountTips->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.7));
	pPlayerAccountTips->setAnchorPoint(Vec2(0.0,0.5));
	pPlayerAccountTips->setColor(ccc3(106,57,6));
	this->addChild(pPlayerAccountTips);
	
	string szText0 = EntityMgr::instance()->login()->getAccout();
	LabelTTF* pPlayerAccountLabel = LabelTTF::create(szText0.c_str(),"",22);
	pPlayerAccountLabel->setColor(ccc3(106,57,6));
	pPlayerAccountLabel->setPosition(Vec2(winSize.width * 0.07 + 64,winSize.height * 0.7));
	pPlayerAccountLabel->setAnchorPoint(Vec2(0.0,0.5));
	this->addChild(pPlayerAccountLabel);
	//切换账号
	if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
	{
		Sprite* pChangeAccountNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_option_8.png"));
		Sprite* pChangeAccountSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_option_8.png"));
		MenuItemSprite* pChangeMenuItemSprite = MenuItemSprite::create(pChangeAccountNormal,pChangeAccountSelect,CC_CALLBACK_1(RoomOption::changeAccount,this));
		pChangeAccountSelect->setScale(0.9f);
		pChangeAccountSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu* pChangeMenu= Menu::create(pChangeMenuItemSprite,NULL);
		pChangeMenu->setPosition(Vec2(winSize.width * 0.85,winSize.height * 0.7));
		this->addChild(pChangeMenu);
	}
	else
	{
		Sprite* pChangeAccountNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_overregist0.png"));
		Sprite* pChangeAccountSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_overregist0.png"));
		MenuItemSprite* pChangeMenuItemSprite = MenuItemSprite::create(pChangeAccountNormal,pChangeAccountSelect,CC_CALLBACK_1(RoomOption::changeAccount,this));
		pChangeAccountSelect->setScale(0.9f);
		pChangeAccountSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu* pChangeMenu= Menu::create(pChangeMenuItemSprite,NULL);
		pChangeMenu->setPosition(Vec2(winSize.width * 0.85,winSize.height * 0.7));
		this->addChild(pChangeMenu);
	}
	
	//声音
	LabelTTF* pVoiceTipsLabel = LabelTTF::create(pConfigMgr->text("t132"),"",25);
	pVoiceTipsLabel->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.61));
	pVoiceTipsLabel->setAnchorPoint(Vec2(0.0,0.5));
	pVoiceTipsLabel->setColor(ccc3(106,57,6));
	this->addChild(pVoiceTipsLabel);
	//静音模式
	Sprite* spriteLableBack2 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	Sprite* spriteLableBack3 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	spriteLableBack2->setPosition(Vec2(winSize.width * 0.5 - spriteLableBack2->getContentSize().width * 0.5,winSize.height * 0.52));
	spriteLableBack3->setPosition(Vec2(winSize.width * 0.5 + spriteLableBack3->getContentSize().width * 0.5-1,winSize.height * 0.52));
	spriteLableBack3->setFlipX(true);
	this->addChild(spriteLableBack2);
	this->addChild(spriteLableBack3);
	LabelTTF* pSilentTipsLabel  = LabelTTF::create(pConfigMgr->text("t133"),"",22);
	pSilentTipsLabel->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.52));
	pSilentTipsLabel->setAnchorPoint(Vec2(0.0,0.5));
	pSilentTipsLabel->setColor(ccc3(106,57,6));
	this->addChild(pSilentTipsLabel);
	//静音开关
	//默认关闭
	m_bOpenSlientMode = UserDefault::getInstance()->getBoolForKey("ZJD_SOUNDSTATE");
	Sprite* silentNormal;
	Sprite* silentSelect;
	if(m_bOpenSlientMode)
	{
		silentNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_option_5.png"));
		silentSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_option_5.png"));
	}
	else
	{
		silentNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_option_4.png"));
		silentSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_option_4.png"));
	}
	
	m_pSilentItemSprite = MenuItemSprite::create(silentNormal,silentSelect,CC_CALLBACK_1(RoomOption::openSilentMode,this));
	Menu* slientMneu = Menu::create(m_pSilentItemSprite,NULL);
	slientMneu->setPosition(Vec2(winSize.width * 0.85,winSize.height * 0.52));
	this->addChild(slientMneu);
	//音乐
	Sprite* spriteLableBack4 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	Sprite* spriteLableBack5 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	spriteLableBack4->setPosition(Vec2(winSize.width * 0.5 - spriteLableBack4->getContentSize().width * 0.5,winSize.height * 0.41));
	spriteLableBack5->setPosition(Vec2(winSize.width * 0.5 + spriteLableBack5->getContentSize().width * 0.5-1,winSize.height * 0.41));
	spriteLableBack5->setFlipX(true);
	this->addChild(spriteLableBack4);
	this->addChild(spriteLableBack5);
	LabelTTF* pMusicTipsLabel  = LabelTTF::create(pConfigMgr->text("t134"),"",22);
	pMusicTipsLabel->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.41));
	pMusicTipsLabel->setAnchorPoint(Vec2(0.0,0.5));
	pMusicTipsLabel->setColor(ccc3(106,57,6));
	this->addChild(pMusicTipsLabel);
	m_pMusicControlBlackBack = Sprite::createWithSpriteFrame(spriteFrame("dt_option_1.png"));
	Sprite* m_pMusicControlWhiteBack = Sprite::createWithSpriteFrame(spriteFrame("dt_option_2.png"));
	m_pMusicControlSprite = Sprite::createWithSpriteFrame(spriteFrame("dt_option_7.png"));
	m_pMusicControlBlackBack->setPosition(Vec2(winSize.width * 0.55,winSize.height * 0.41));

	m_pMusicControlWhiteTimer = ProgressTimer::create(m_pMusicControlWhiteBack);
	m_pMusicControlWhiteTimer->setMidpoint(Vec2(0.0f,0.0f));
	m_pMusicControlWhiteTimer->setBarChangeRate(Vec2(1.0f,0.0f));
	m_pMusicControlWhiteTimer->setType(ProgressTimer::Type::BAR);
	m_pMusicControlWhiteTimer->setPosition(Vec2(winSize.width * 0.55,winSize.height * 0.41));
	float musicNum = UserDefault::getInstance()->getFloatForKey("ZJD_SOUNDNUM",1.0);
	m_pMusicControlSprite->setPosition(Vec2(m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 + m_pMusicControlBlackBack->getContentSize().width*musicNum,winSize.height * 0.41));
	this->addChild(m_pMusicControlBlackBack);
	this->addChild(m_pMusicControlWhiteTimer);
	this->addChild(m_pMusicControlSprite);
	m_pMusicControlWhiteTimer->setPercentage(musicNum*100);



	//音效
	Sprite* spriteLableBack6 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	Sprite* spriteLableBack7 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	spriteLableBack6->setPosition(Vec2(winSize.width * 0.5 - spriteLableBack6->getContentSize().width * 0.5,winSize.height * 0.3));
	spriteLableBack7->setPosition(Vec2(winSize.width * 0.5 + spriteLableBack7->getContentSize().width * 0.5-1,winSize.height * 0.3));
	spriteLableBack7->setFlipX(true);
	this->addChild(spriteLableBack6);
	this->addChild(spriteLableBack7);
	LabelTTF* pSoundTipsLabel  = LabelTTF::create(pConfigMgr->text("t135"),"",22);
	pSoundTipsLabel->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.3));
	pSoundTipsLabel->setAnchorPoint(Vec2(0.0,0.5));
	pSoundTipsLabel->setColor(ccc3(106,57,6));
	this->addChild(pSoundTipsLabel);
	m_pSoundControlBlackBack = Sprite::createWithSpriteFrame(spriteFrame("dt_option_1.png"));
	Sprite* m_pSoundControlWhiteBack = Sprite::createWithSpriteFrame(spriteFrame("dt_option_2.png"));
	m_pSoundControlSprite = Sprite::createWithSpriteFrame(spriteFrame("dt_option_7.png"));
	m_pSoundControlBlackBack->setPosition(Vec2(winSize.width * 0.55,winSize.height * 0.3));
	m_pSoundControlWhiteTimer = ProgressTimer::create(m_pSoundControlWhiteBack);
	m_pSoundControlWhiteTimer->setMidpoint(Vec2(0.0f,0.0f));
	m_pSoundControlWhiteTimer->setBarChangeRate(Vec2(1.0f,0.0f));
	m_pSoundControlWhiteTimer->setType(ProgressTimer::Type::BAR);
	m_pSoundControlWhiteTimer->setPosition(Vec2(winSize.width * 0.55,winSize.height * 0.3));
	m_pSoundControlWhiteBack->setPosition(Vec2(winSize.width * 0.55,winSize.height * 0.3));

	float effNum = UserDefault::getInstance()->getFloatForKey("SOUNDEFFVOL",0.5f);
	m_pSoundControlSprite->setPosition(Vec2(m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 + m_pMusicControlBlackBack->getContentSize().width*effNum,winSize.height * 0.3));

	this->addChild(m_pSoundControlBlackBack);
	this->addChild(m_pSoundControlWhiteTimer);
	this->addChild(m_pSoundControlSprite);
	m_pSoundControlWhiteTimer->setPercentage(effNum*100);

	//其他
	LabelTTF* pOtherTipsLabel = LabelTTF::create(pConfigMgr->text("t136"),"",25);
	pOtherTipsLabel->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.2));
	pOtherTipsLabel->setAnchorPoint(Vec2(0.0,0.5));
	pOtherTipsLabel->setColor(ccc3(106,57,6));
	this->addChild(pOtherTipsLabel);

	//版本检测
	Sprite* spriteLableBack8 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	Sprite* spriteLableBack9 = Sprite::createWithSpriteFrame(spriteFrame("dt_option_3.png"));
	spriteLableBack8->setPosition(Vec2(winSize.width * 0.5 - spriteLableBack8->getContentSize().width * 0.5,winSize.height * 0.09));
	spriteLableBack9->setPosition(Vec2(winSize.width * 0.5 + spriteLableBack9->getContentSize().width * 0.5-1,winSize.height * 0.09));
	spriteLableBack9->setFlipX(true);
	this->addChild(spriteLableBack8);
	this->addChild(spriteLableBack9);
	LabelTTF* pCheckTipsLabel  = LabelTTF::create(pConfigMgr->text("t137"),"",22);
	pCheckTipsLabel->setPosition(Vec2(winSize.width * 0.07,winSize.height * 0.09));
	pCheckTipsLabel->setAnchorPoint(Vec2(0.0,0.5));
	pCheckTipsLabel->setColor(ccc3(106,57,6));
	this->addChild(pCheckTipsLabel);
	const char* pVesion = String::createWithFormat("%d.%d.%d",pConfigMgr->m_VersionInfo.m_nHighVersion,pConfigMgr->m_VersionInfo.m_nMiddleVersion,pConfigMgr->m_VersionInfo.m_nLowVersion)->getCString();
	LabelTTF* pVesionLabel = LabelTTF::create(pVesion,"",22);
	pVesionLabel->setPosition(Vec2(winSize.width * 0.07 + 108,winSize.height * 0.09));
	pVesionLabel->setAnchorPoint(Vec2(0.0,0.5));
	pVesionLabel->setColor(ccc3(106,57,6));
	this->addChild(pVesionLabel);
	Sprite* pCheckVesionNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_option_6.png"));
	Sprite* pCheckVesionSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_option_6.png"));
	MenuItemSprite* pCheckVesionItemSprite = MenuItemSprite::create(pCheckVesionNormal,pCheckVesionSelect,CC_CALLBACK_1(RoomOption::checkVesion,this));
	pCheckVesionSelect->setScale(0.9f);
	pCheckVesionSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* pCheckMenu= Menu::create(pCheckVesionItemSprite,NULL);
	pCheckMenu->setPosition(Vec2(winSize.width * 0.85,winSize.height * 0.09));
	this->addChild(pCheckMenu);

	this->setKeypadEnabled(true);
	//setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
    return true;
}

void RoomOption::onEnter()
{
    Layer::onEnter();
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-1000,true);
}

void RoomOption::onExit()
{
	//
   Layer::onExit();
}

bool RoomOption::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	if(m_pMusicControlSprite->boundingBox().containsPoint(touchPoint))
	{
		m_nChangeVoice = 1;
		return true;
	}
	if(m_pSoundControlSprite->boundingBox().containsPoint(touchPoint))
	{
		m_nChangeVoice = 2;
		return true;
	}
	return false;
}

void RoomOption::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	if(m_nChangeVoice == 1)
	{
		if(touchPoint.x >= m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 && touchPoint.x <= m_pMusicControlBlackBack->getPositionX() + m_pMusicControlBlackBack->getContentSize().width * 0.5)
		{
			m_pMusicControlSprite->setPosition(Vec2(touchPoint.x,m_pMusicControlSprite->getPositionY()));
			m_pMusicControlWhiteTimer->setPercentage(((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width)*100);
			//此处改变音乐音量大小
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->setFloatForKey("ZJD_SOUNDNUM",(m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->flush();
		}
	}
	if(m_nChangeVoice == 2)
	{
		if(touchPoint.x >= m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 && touchPoint.x <= m_pMusicControlBlackBack->getPositionX() + m_pMusicControlBlackBack->getContentSize().width * 0.5)
		{
			m_pSoundControlSprite->setPosition(Vec2(touchPoint.x,m_pSoundControlSprite->getPositionY()));
			m_pSoundControlWhiteTimer->setPercentage(((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width)*100);
			UserDefault::getInstance()->setFloatForKey("SOUNDEFFVOL",(m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->flush();
		}
		//此处改变音效音量大小
	}
}

void RoomOption::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	if(m_nChangeVoice == 1)
	{
		if(touchPoint.x >= m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 && touchPoint.x <= m_pMusicControlBlackBack->getPositionX() + m_pMusicControlBlackBack->getContentSize().width * 0.5)
		{
			m_pMusicControlSprite->setPosition(Vec2(touchPoint.x,m_pMusicControlSprite->getPositionY()));
			m_pMusicControlWhiteTimer->setPercentage(((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width)*100);
			//此处改变音乐音量大小
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->setFloatForKey("ZJD_SOUNDNUM",(m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->flush();
		}
	}
	if(m_nChangeVoice == 2)
	{
		if(touchPoint.x >= m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 && touchPoint.x <= m_pMusicControlBlackBack->getPositionX() + m_pMusicControlBlackBack->getContentSize().width * 0.5)
		{
			m_pSoundControlSprite->setPosition(Vec2(touchPoint.x,m_pSoundControlSprite->getPositionY()));
			m_pSoundControlWhiteTimer->setPercentage(((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width)*100);
			UserDefault::getInstance()->setFloatForKey("SOUNDEFFVOL",(m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->flush();
		}
		//此处改变音效音量大小
	}
}

void RoomOption::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	if(m_nChangeVoice == 1)
	{
		if(touchPoint.x >= m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 && touchPoint.x <= m_pMusicControlBlackBack->getPositionX() + m_pMusicControlBlackBack->getContentSize().width * 0.5)
		{
			m_pMusicControlSprite->setPosition(Vec2(touchPoint.x,m_pMusicControlSprite->getPositionY()));
			m_pMusicControlWhiteTimer->setPercentage(((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width)*100);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->setFloatForKey("ZJD_SOUNDNUM",(m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->flush();
			//此处改变音乐音量大小
		}
	}
	if(m_nChangeVoice == 2)
	{
		if(touchPoint.x >= m_pMusicControlBlackBack->getPositionX() - m_pMusicControlBlackBack->getContentSize().width * 0.5 && touchPoint.x <= m_pMusicControlBlackBack->getPositionX() + m_pMusicControlBlackBack->getContentSize().width * 0.5)
		{
			m_pSoundControlSprite->setPosition(Vec2(touchPoint.x,m_pSoundControlSprite->getPositionY()));
			m_pSoundControlWhiteTimer->setPercentage(((m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width)*100);
			UserDefault::getInstance()->setFloatForKey("SOUNDEFFVOL",(m_pSoundControlBlackBack->getContentSize().width * 0.5 + touchPoint.x - m_pSoundControlBlackBack->getPositionX())/m_pSoundControlBlackBack->getContentSize().width);
			UserDefault::getInstance()->flush();
		}
		//此处改变音效音量大小
	}
}

void RoomOption::back(Object* obj)
{
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void RoomOption::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
	}
}

void RoomOption::toFreeLayer(Object* obj)
{
	playButtonSound();
	Director::getInstance()->replaceScene(FreeLayer::scene());
}

void RoomOption::changeAccount(Object* obj)
{
	playButtonSound();
	if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
	{
		Director::getInstance()->replaceScene(LoginLayer::scene());
	}
	else
	{
		Scene* scene = Scene::create();
		OverRegistLayer* regist = OverRegistLayer::create();
		regist->setRegistLayerFrom(0);
		scene->addChild(regist);
		Director::getInstance()->replaceScene(scene);
	}
	
}

void RoomOption::openSilentMode(Object* obj)
{
	playButtonSound();
	if(m_bOpenSlientMode)
	{
		UserDefault::getInstance()->setBoolForKey("ZJD_SOUNDSTATE",false);
		m_bOpenSlientMode = false;
		playBGM("music/roomback.mp3");
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("ZJD_SOUNDSTATE",true);
		m_bOpenSlientMode = true;
		stopBGM();
	}
	Sprite* silentNormal;
	Sprite* silentSelect;
	if(m_bOpenSlientMode)
	{
		silentNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_option_5.png"));
		silentSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_option_5.png"));
	}
	else
	{
		silentNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_option_4.png"));
		silentSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_option_4.png"));
	}
	m_pSilentItemSprite->setNormalImage(silentNormal);
	m_pSilentItemSprite->setSelectedImage(silentSelect);
}

void RoomOption::checkVesion(Object* obj)
{
	playButtonSound();
	bool bRet = pMsgDispatch->connectLoginServer();
	if(bRet)
	{
		int version = ((pConfigMgr->m_VersionInfo.m_nHighVersion)<<24) + ((pConfigMgr->m_VersionInfo.m_nMiddleVersion)<<16) + (pConfigMgr->m_VersionInfo.m_nLowVersion);
		//给服务端发送版本
		pMsgDispatch->sendGetConfigPacket(version);
	}
	else
	{
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
		this->addChild(box,100);
	}
}

void RoomOption::onCheckVersion(Object* obj)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptVersionNew);
	this->addChild(box,50);
}