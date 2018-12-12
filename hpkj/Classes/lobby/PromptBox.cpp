#include "PromptBox.h"
#include <limits.h>
#include "GlobalDef.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "LoginLayer.h"
#include "GroupSprite.h"
#include "SceneControl.h"
#include "FreeLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

PromptBox* PromptBox::PromptBoxWith(Vec2 pos,mPromptType type)
{
	PromptBox * box = new PromptBox();
	if(box && box->initPromptBox(pos,type))
	{
		box->autorelease();
		return box;
	}
	CC_SAFE_DELETE(box);
	return NULL;
}
PromptBox::PromptBox()
{
	m_bIsLack = false;
	m_pBackMenu = nullptr;

	back = nullptr;
	m_pAlartWord = nullptr  ;
}
PromptBox::~PromptBox()
{

}

void PromptBox::setBackgroundScale(float scale)
{
    back->setScale(scale);
}
bool PromptBox::initPromptBox(Vec2 pos , mPromptType type)
{
	if ( !Layer::init() )
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PromptBox::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(PromptBox::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(PromptBox::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(PromptBox::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	m_nTypes = type;
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	LayerColor* colorLayer = LayerColor::create(ccc4(166, 166, 166, 100), winSize.width, winSize.height);
	this->addChild(colorLayer);
	if(m_nTypes == mPromptpasswordUsed)
	{
		Sprite* back = Sprite::createWithSpriteFrame(spriteFrame("beting45.png"));
		back->setPosition(pos);
		this->addChild(back);
		LabelTTF* label = LabelTTF::create(pConfigMgr->text("display_DuangDong.xml", "t165"),"",22,cocos2d::Size(360, 150), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		label->setColor(ccc3(255,255,0));
		label->setPosition(pos);
		this->addChild(label);

		MoveTo* moveto0 = MoveTo::create(0.2f,Vec2(back->getPositionX(),back->getPositionY() + 10));
		MoveTo* moveto1 = MoveTo::create(0.2f,Vec2(back->getPositionX(),back->getPositionY() + 10));
		MoveTo* moveto2 = MoveTo::create(0.2f,Vec2(label->getPositionX(),label->getPositionY() + 10));
		DelayTime* delay0 = DelayTime::create(2.0f);
		DelayTime* delay1 = DelayTime::create(2.0f);
		DelayTime* delay2 = DelayTime::create(2.0f);
		CallFuncN* call0 = CallFuncN::create(this,callfuncN_selector(PromptBox::onActionDown));
		back->runAction(Sequence::create(moveto0,delay0,call0,NULL));
		label->runAction(Sequence::create(moveto2,delay2,NULL));
	}
	else if(m_nTypes == mPromptUsedCHINESE)
	{
		Sprite* back = Sprite::createWithSpriteFrame(spriteFrame("beting45.png"));
		back->setPosition(pos);
		this->addChild(back);
		LabelTTF* label = LabelTTF::create(pConfigMgr->text("display_DuangDong.xml", "t7"),"",22,cocos2d::Size(360, 150), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		label->setColor(ccc3(255,255,0));
		label->setPosition(pos);
		this->addChild(label);
		MoveTo* moveto0 = MoveTo::create(0.2f,Vec2(back->getPositionX(),back->getPositionY() + 10));
		MoveTo* moveto1 = MoveTo::create(0.2f,Vec2(back->getPositionX(),back->getPositionY() + 10));
		MoveTo* moveto2 = MoveTo::create(0.2f,Vec2(label->getPositionX(),label->getPositionY() + 10));
		DelayTime* delay0 = DelayTime::create(2.0f);
		DelayTime* delay1 = DelayTime::create(2.0f);
		DelayTime* delay2 = DelayTime::create(2.0f);
		CallFuncN* call0 = CallFuncN::create(this,callfuncN_selector(PromptBox::onActionDown));
		back->runAction(Sequence::create(moveto0,delay0,call0,NULL));
		label->runAction(Sequence::create(moveto2,delay2,NULL));
	}
	else if(m_nTypes == mPromptTypeServer)
	{
		Sprite* back = Sprite::createWithSpriteFrame(spriteFrame("beting45.png"));
		back->setPosition(pos);
		this->addChild(back);
		LabelTTF* label = LabelTTF::create(pConfigMgr->text("display_DuangDong.xml", "t5"),"",22,cocos2d::Size(360, 150), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		label->setColor(ccc3(255,255,0));
		label->setPosition(pos);
		this->addChild(label);

		MoveTo* moveto0 = MoveTo::create(0.2f,Vec2(back->getPositionX(),back->getPositionY() + 10));
		MoveTo* moveto1 = MoveTo::create(0.2f,Vec2(back->getPositionX(),back->getPositionY() + 10));
		MoveTo* moveto2 = MoveTo::create(0.2f,Vec2(label->getPositionX(),label->getPositionY() + 10));
		DelayTime* delay0 = DelayTime::create(2.0f);
		DelayTime* delay1 = DelayTime::create(2.0f);
		DelayTime* delay2 = DelayTime::create(2.0f);
		CallFuncN* call0 = CallFuncN::create(this,callfuncN_selector(PromptBox::onActionDown));
		back->runAction(Sequence::create(moveto0,delay0,call0,NULL));
		label->runAction(Sequence::create(moveto2,delay2,NULL));
	}
	else if(m_nTypes == mPromptexitSelect)
	{
		//背景
		Sprite* exitBack = Sprite::createWithSpriteFrame(spriteFrame("beting46.png"));
		exitBack->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(exitBack);

		//tips
		Sprite* spriteTips = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer0.png"));
		spriteTips->setPosition(Vec2(winSize.width * 0.5,exitBack->getPositionY()+exitBack->getContentSize().height/2-spriteTips->getContentSize().height/2-15));
		this->addChild(spriteTips);

		Sprite* closeNormal = Sprite::createWithSpriteFrame(spriteFrame("gamble_15.png"));
		Sprite* closeSelect = Sprite::createWithSpriteFrame(spriteFrame("gamble_15.png"));
		MenuItemSprite* closeI = MenuItemSprite::create(closeNormal,closeSelect,CC_CALLBACK_1(PromptBox::close,this));
		Menu* closeMenu = Menu::create(closeI,NULL);
		closeMenu->setPosition(Vec2(exitBack->getContentSize().width - closeNormal->getContentSize().width * 0.5,exitBack->getContentSize().height - closeNormal->getContentSize().height * 0.5));
		exitBack->addChild(closeMenu);

		Sprite* overRegistNormal = Sprite::createWithSpriteFrame(spriteFrame("beting47.png"));
		Sprite* overRegistSelect = Sprite::createWithSpriteFrame(spriteFrame("beting48.png"));
		MenuItemSprite* overRegist = MenuItemSprite::create(overRegistNormal,overRegistSelect,CC_CALLBACK_1(PromptBox::onOverRegist,this));
		Sprite* exitNormal =  Sprite::createWithSpriteFrame(spriteFrame("beting49.png"));
		Sprite* exitSelect =  Sprite::createWithSpriteFrame(spriteFrame("beting50.png"));
		MenuItemSprite* exitItem = MenuItemSprite::create(exitNormal,exitSelect,CC_CALLBACK_1(PromptBox::exitGame,this));
		Menu* exitItemMenu = Menu::create(exitItem,overRegist,NULL);
		exitItemMenu->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.43));
		exitItemMenu->alignItemsHorizontallyWithPadding(50);
		this->addChild(exitItemMenu);

	}
	else
	{

		back = Sprite::createWithSpriteFrame(spriteFrame("setting_07.png"));
		back->setPosition(pos);
		this->addChild(back);
		Sprite* spriteTips = Sprite::createWithSpriteFrame(spriteFrame("dt_prompt0.png"));
		spriteTips->setPosition(Vec2(winSize.width * 0.5 ,back->getContentSize().height * 0.45 + winSize.height * 0.5));
		this->addChild(spriteTips);

		Sprite * backNormal = Sprite::createWithSpriteFrame(spriteFrame("gamble_15.png"));
		Sprite * backSelect = Sprite::createWithSpriteFrame(spriteFrame("gamble_15.png"));
		MenuItemSprite * backItemSprite = MenuItemSprite::create(backNormal, backSelect, CC_CALLBACK_1(PromptBox::closePromptBox,this));
		m_pBackMenu = Menu::create(backItemSprite,NULL);
		m_pBackMenu->setPosition(Vec2(back->getContentSize().width * 0.6 +  winSize.width * 0.5 - backSelect->getContentSize().width * 0.4,back->getContentSize().height * 0.6 + winSize.height * 0.5 - backSelect->getContentSize().height * 0.4));
		backSelect->setScale(0.9f);
		backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		this->addChild(m_pBackMenu);
		if(m_nTypes == mPromptTypePasswordFind)
		{
			m_pAlartWord = LabelTTF::create("111", "HelveticaNeue-Bold", 20, cocos2d::Size(360, 150), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
			m_pAlartWord->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.51));
			this->addChild(m_pAlartWord);
		}
		else
		{
			m_pAlartWord = LabelTTF::create(getTheCharWithType(type), "HelveticaNeue-Bold", 20, cocos2d::Size(360, 150), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
			m_pAlartWord->setColor(ccc3(0,0,0));
			m_pAlartWord->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.51));
			this->addChild(m_pAlartWord);
		}
		if(m_nTypes == mPromptTypeMakeSureModifyNickName || m_nTypes == mPromptTypeNosever)
		{
			m_pAlartWord->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.51));
			Sprite *okNormal =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
			Sprite *okSelect =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
			MenuItemSprite * okItemSprite = MenuItemSprite::create(okNormal, okSelect,  CC_CALLBACK_1(PromptBox::okButton,this));
			m_pOkMenu = Menu::create(okItemSprite,NULL);
			m_pOkMenu->setPosition(Vec2(winSize.width * 0.5 ,winSize.height * 0.5 - back->getContentSize().height * 0.5 + okSelect->getContentSize().height * 0.5+5));
			okSelect->setScale(0.9f);
			okSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

			this->addChild(m_pOkMenu);
		}
		if(m_nTypes == mPromptStartFaild || m_nTypes == mPromptTypeLackCoins)
		{
			Sprite *okNormal =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
			Sprite *okSelect =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
			MenuItemSprite * okItemSprite = MenuItemSprite::create(okNormal, okSelect, CC_CALLBACK_1(PromptBox::okButton,this));
			okSelect->setScale(0.9f);
			okSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
			m_pOkMenu = Menu::create(okItemSprite,/*,cannelItemSprite,*/NULL);
			m_pOkMenu->setPosition(Vec2(winSize.width * 0.5 ,winSize.height * 0.5 - back->getContentSize().height * 0.5 + okSelect->getContentSize().height * 0.5+5));
			this->addChild(m_pOkMenu);
		}

		if(m_nTypes == mPromptTypeLackCoins)
		{
			m_pAlartWord->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.52));
		}
	}
	return true;
}

void PromptBox::setPromptText(const char* szDes)
{
	m_pAlartWord->setString(szDes);
}
void PromptBox::onActionDown(Node* obj)
{
	LoginLayer* layer = LoginLayer::create();
	layer->setLogin(false);
	Scene* scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}
void PromptBox::okButton(Object* obj)
{
	if(m_nTypes == mPromptTypeMakeSureModifyNickName)
	{
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_SUREMODYFINICKNAME,NULL);
		this->removeFromParentAndCleanup(true);
	}

	if(m_nTypes == mPromptTypeLackCoins || m_nTypes == mPromptTypeNosever)
	{
		this->removeFromParentAndCleanup(true);
	}

	if(m_nTypes == mPromptStartFaild)
	{
		if(m_bIsLack)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			if(m_nTypes == mPromptStartFaild)
			{
				showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),String::createWithFormat("%d",m_nLevel)->getCString(),String::createWithFormat("%d",m_nXXX)->getCString());
			}
			else
			{
				showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),String::createWithFormat("%d",1)->getCString(),String::createWithFormat("%d",2)->getCString());
			}
#endif
			this->removeFromParentAndCleanup(true);
		}
		else
		{
			bool ret = pMsgDispatch->connectGameServer();
			if (!ret)
			{
				PromptBox* box = PromptBox::PromptBoxWith(Vec2(427, SCREEN_WIDTH/2), mPromptTypeServerShut);
				this->getParent()->addChild(box,20);
				this->removeFromParentAndCleanup(true);
				return;
			}
			this->removeFromParentAndCleanup(true);
		}
	}
}

void PromptBox::cannelButton(Object* obj)
{
	if(m_nTypes == mPromptStartFaild)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void PromptBox::setLevelAndX(int nLevel,int nXXX,bool isLack)
{
	m_nLevel = nLevel;
	m_nXXX = nXXX;
	m_bIsLack = isLack;
	if(isLack)
	{
		m_pAlartWord->setString(pConfigMgr->text("t451"));
	}
	else
	{
		m_pAlartWord->setString(pConfigMgr->text("t450"));
	}cocos2d::Size winSize = Director::getInstance()->getWinSize();
	m_pAlartWord->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.49));
}
void PromptBox::close(Object *obj)
{
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);
}
void PromptBox::onOverRegist(Object* obj)
{
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);
}
void PromptBox::exitGame(Object* obj)
{
	playButtonSound();
	Director::getInstance()->end();
}
void PromptBox::closePromptBox(Object *obj)
{
	if (m_nTypes == mPromptTypeServerShut)
	{
		pMsgDispatch->setLoginState(enLogin_Server);
		Scene* scene = Scene::create();
		LoginLayer* layer = LoginLayer::create(false);
		layer->setLogin(false);
		scene->addChild(layer);
		Director::getInstance()->replaceScene(scene);
	}
	if(m_nTypes == mPromptTypeLoginFaild)
	{
		LoginLayer* layer = LoginLayer::create();
		layer->setLogin(false);
		Scene* scene = Scene::create();
		scene->addChild(layer);
		Director::getInstance()->replaceScene(scene);
	}
	this->removeFromParentAndCleanup(true);
}

bool PromptBox::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if(!m_pBackMenu) return false;

	m_bBackMenu = m_pBackMenu->onTouchBegan(pTouch, pEvent);
	if(m_nTypes == mPromptTypeMakeSureModifyNickName || m_nTypes == mPromptStartFaild || m_nTypes == mPromptTypeLackCoins)
	{
		m_bOkMenu = m_pOkMenu->onTouchBegan(pTouch, pEvent);
	}
	return true;
}

void PromptBox::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if(m_bBackMenu)
	{
		m_pBackMenu->onTouchMoved(pTouch, pEvent);
	}
	if(m_nTypes == mPromptTypeMakeSureModifyNickName || m_nTypes == mPromptStartFaild || m_nTypes == mPromptTypeLackCoins)
	{
		if(m_bOkMenu)
		{
			m_pOkMenu->onTouchMoved(pTouch, pEvent);
		}
	}
}

void PromptBox::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if(m_bBackMenu)
	{
		m_pBackMenu->onTouchEnded(pTouch, pEvent);
	}
	if(m_nTypes == mPromptTypeMakeSureModifyNickName || m_nTypes == mPromptStartFaild || m_nTypes == mPromptTypeLackCoins)
	{
		if(m_bOkMenu)
		{
			m_pOkMenu->onTouchEnded(pTouch, pEvent);
		}
	}
}

void PromptBox::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	if(m_bBackMenu)
	{
		m_pBackMenu->onTouchCancelled(pTouch, pEvent);
	}
	if(m_nTypes == mPromptTypeMakeSureModifyNickName || m_nTypes == mPromptStartFaild || m_nTypes == mPromptTypeLackCoins)
	{
		if(m_bOkMenu)
		{
			m_pOkMenu->onTouchCancelled(pTouch, pEvent);
		}
	}
}

const char* PromptBox::getTheCharWithType(mPromptType type)
{
	const char * str = "";
	switch (type)
	{
	case mPromptTypeServerShut:
		str = pConfigMgr->text("t41");
		break;
	case mPromptTypeLoginFaild:
		str = pConfigMgr->text("t24");
		break;
	case mPromptTypeErrorQQNumber:
		str = pConfigMgr->text("t37");
		break;
	case mPromptTypeErrorPhone:
		str = pConfigMgr->text("t38");
		break;
	case mPromptTypeTwoInputError:
		str = pConfigMgr->text("t39");
		break;
	case mPromptTypeErrorNickName:
		str = pConfigMgr->text("t307");
		break;
	case mPromptTypeSeverPasswordFaild:
		str = pConfigMgr->text("t28");
		break;
	case mPromptTypeLoginLegal:
		str = pConfigMgr->text("t30");
		break;
	case mPromptTypeMakeSureModifyNickName:
		str = pConfigMgr->text("t308");
		break;
	case mPromptTypeGetChargeFaild:
		str = pConfigMgr->text("t310");
		break;
	case mPromptTypeGetError:
		str = pConfigMgr->text("t454");
		break;
	case mPromptTypeLackCoins:
		str = pConfigMgr->text("t456");
		break;
	case  mPromptVersionNew:
		str = pConfigMgr->text("t479");
		break;
	case  mPromptNickNameError:
		str = pConfigMgr->text("t481");
		break;
	default:
		break;
	}
	return str;
}