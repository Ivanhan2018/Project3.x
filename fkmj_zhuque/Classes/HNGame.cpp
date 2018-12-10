#include "HNGame.h"
#include "ScriptData.h"
#include "utility.h"
#include "WindowInfo.h"
#include "WidgetManager.h"
#include "WidgetScenceXMLparse.h"
#include "Socket.h"
#include "HNScenceManager.h"
#include "ScriptXMLparse.h"
#include "WidgetSkinXMLparse.h"
#include "WidgetFun.h"
USING_NS_CC;

CCScene* HNGame::scene()
{
    CCScene *scene = CCScene::create();
    HNGame *layer = HNGame::create();
    scene->addChild(layer);
    return scene;
}
void HNGame::setString(std::string kTxt)
{
}
bool HNGame::init()
{
    if ( !CCLayer::init() )
    {
        return false;
	}
	int iTemp = PROCESS_VERSION(7,0,3);
	WidgetSkinXMLparse kSkinXml1("Script/HNWidgetSkin.xml");

	cocos2d::ScriptXMLparse kScriptXml1("Script/HNScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/HNScriptValueStr.xml");
	cocos2d::ScriptXMLparse kScriptXml3("Script/HNScriptValueSame.xml");

	this->scheduleUpdate();
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
	setKeyboardEnabled(true);

	new WindowInfo;
	
	ScriptData<std::string> kUITest("UITest");

	if (kUITest.Value() != "")
	{
		CCMenuItemImage *pImagicItem1 = CCMenuItemImage::create(
			"progress_y.png",
			"progress_n.png",
			this,
			menu_selector(HNGame::reLoad));

		pImagicItem1->setPosition(ccp(80 ,150));

		CCMenuItemImage *pImagicItem2 = CCMenuItemImage::create(
			"progress_y.png",
			"progress_n.png",
			this,
			menu_selector(HNGame::reMask));

		pImagicItem1->setPosition(ccp(80 ,150));
		pImagicItem2->setPosition(ccp(80 ,70));

		CCMenu* pMenu1 = CCMenu::create(pImagicItem1,pImagicItem2,NULL);
		pMenu1->setPosition(CCPoint::ZERO);
		this->addChild(pMenu1, 2);


		cocos2d::CCNode* pNode = cocos2d::CCNode::create();
		addChild(pNode);
		pNode->setTag(10000);

		if (!WidgetManager::pInstance())
		{
			new WidgetManager;
		}
		WidgetScenceXMLparse kXml11(kUITest.Value(),pNode);
		WindowInfo::Instance().changWinSize(pNode);
		cocos2d::Sprite* pSprite = cocos2d::Sprite::create("Point.png");
		pSprite->setPosition(640,360);
		ScriptData<int> kAple("UITestMaskAple");
		pSprite->setOpacity(kAple.Value());
		pNode->addChild(pSprite, 2);
		pSprite->setTag(11000);

		return true;
	}

	//cocos2d::Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);

	new HNScenceManager(this);

	WindowInfo::Instance().changWinSize(HNScenceManager::Instance().getRootNode());

    return true;
}

void HNGame::reLoad(cocos2d::Ref* pSender)
{
	ScriptDataManager::getInterface()->clear();
	cocos2d::ScriptXMLparse kScriptXml1("Script/HNScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/HNScriptValueStr.xml");
	cocos2d::ScriptXMLparse kScriptXml3("Script/HNScriptValueSame.xml");

	ScriptData<std::string> kUITest("UITest");
	WidgetManager::Instance().clearWidgetSkin();
	WidgetSkinXMLparse kSkinXml1("Script/HNWidgetSkin.xml");
	cocos2d::CCNode* pNode = getChildByTag(10000);
	pNode->removeAllChildren();
	WidgetScenceXMLparse kXml11(kUITest.Value(),pNode);

	cocos2d::Sprite* pSprite = cocos2d::Sprite::create("Point.png");
	pSprite->setPosition(640,360);
	ScriptData<int> kAple("UITestMaskAple");
	pSprite->setOpacity(kAple.Value());
	pNode->addChild(pSprite, 2);
	pSprite->setTag(11000);


	std::string kValue = WidgetFun::getWidgetUserInfo(this,"Aple");
	if (kValue == "")
	{
		pSprite->setOpacity(0);
	}
	else
	{
		ScriptData<int> kAple("UITestMaskAple");
		pSprite->setOpacity(kAple.Value());
	}

}
void HNGame::reMask(cocos2d::Ref* pSender)
{
	std::string kValue = WidgetFun::getWidgetUserInfo(this,"Aple");
	cocos2d::CCNode* pNode = getChildByTag(10000);
	cocos2d::Sprite* pSprite = (cocos2d::Sprite*)pNode->getChildByTag(11000);
	if (kValue == "")
	{
		ScriptData<int> kAple("UITestMaskAple");
		pSprite->setOpacity(kAple.Value());
		WidgetFun::setWidgetUserInfo(this,"Aple","1");
	}
	else
	{
		WidgetFun::setWidgetUserInfo(this,"Aple","");
		pSprite->setOpacity(0);
	}
}
void HNGame::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
	
}
void HNGame::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void HNGame::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void HNGame::onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void HNGame::update(float delta)
{
}
void HNGame::onExit()
{
	cocos2d::Layer::onExit();
}
void HNGame::ExitGame()
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HNGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK
		|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		if (HNScenceManager::pInstance())
		{
			HNScenceManager::Instance().BackKey();
		}
	}
}
