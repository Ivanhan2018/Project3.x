#include "ActivityLayer.h"
#include "RoomLayer.h"
#include "ActivitySprite.h"
#include "FreeLayer.h"
#include "GlobalUnits.h"
#include "Encrypt.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

using namespace cocos2d;
ActivityLayer::ActivityLayer()
{

}
ActivityLayer::~ActivityLayer()
{
	m_ActivityContextArray->release();
	m_ActivitySpriteArray->release();
}
Scene* ActivityLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		ActivityLayer *layer = ActivityLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool ActivityLayer::init()
{
	bool bRet = false;
	do 
	{
		winSize = Director::getInstance()->getWinSize();
		Sprite *pPlayerInfoBJLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		Sprite *pPlayerInfoBJRight = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		pPlayerInfoBJRight->setRotationY(180);
		pPlayerInfoBJLeft->setPosition(Vec2(winSize.width*0.25,winSize.height*0.5));
		pPlayerInfoBJRight->setPosition(Vec2(winSize.width*0.75 - 1,winSize.height*0.5));
		this->addChild(pPlayerInfoBJLeft,0);
		this->addChild(pPlayerInfoBJRight,0);


		Sprite *pPlayerInfoBJLeft1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		Sprite *pPlayerInfoBJRight1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		pPlayerInfoBJRight1->setRotationY(180);
		pPlayerInfoBJLeft1->setPosition(Vec2(winSize.width*0.25+5,winSize.height*0.0-18 - 195));
		pPlayerInfoBJRight1->setPosition(Vec2(winSize.width*0.75-5,winSize.height*0.0-18 - 195));
		pPlayerInfoBJLeft1->setScaleY(1.2f);
		pPlayerInfoBJRight1->setScaleY(1.2f);
		pPlayerInfoBJLeft1->setAnchorPoint(Vec2(0.5,0));
		pPlayerInfoBJRight1->setAnchorPoint(Vec2(0.5,0));
		this->addChild(pPlayerInfoBJLeft1,1);
		this->addChild(pPlayerInfoBJRight1,1);

		//免费
		Sprite *pMianFeiSpriteNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
		Sprite *pMianFeiSpriteSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
		MenuItemSprite* pMianfei = MenuItemSprite::create(pMianFeiSpriteNormal,pMianFeiSpriteSelect,CC_CALLBACK_1(ActivityLayer::toFreeLayer,this));
		pMianFeiSpriteSelect->setScale(0.9f);
		pMianFeiSpriteSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu* mianfeiMenu = Menu::create(pMianfei,NULL);
		mianfeiMenu->setPosition(Vec2(winSize.width*0.054,winSize.height*0.885));
		this->addChild(mianfeiMenu);


		Sprite *pPlayerInfoLogo = Sprite::createWithSpriteFrame(spriteFrame("dt_register_14.png"));
		pPlayerInfoLogo->setPosition(Vec2(winSize.width*0.5,winSize.height*0.923));
		this->addChild(pPlayerInfoLogo,2);

		Sprite *pPlayerInfoLogoText = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_1.png"));
		pPlayerInfoLogoText->setPosition(Vec2(winSize.width*0.5,winSize.height*0.933));
		this->addChild(pPlayerInfoLogoText,3);

		m_nSelectActivity = 0;
		m_ActivitySpriteArray = Array::create();
		m_ActivitySpriteArray->retain();
		m_ActivityContextArray = Array::create();
		m_ActivityContextArray->retain();

		activityTips = LabelTTF::create(ConfigMgr::instance()->text(String::createWithFormat("t%d",500)->getCString()),"",26);
		activityTips->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.37));
		activityTips->setColor(ccc3(0,0,0));
		this->addChild(activityTips,3);

		//活动1
		for (int i = 0; i <5 ; i++)
		{
			ActivitySprite* sprite = ActivitySprite::ActivitySpriteWith(i);
			sprite->setPosition(Vec2(sprite->getContentSize().width * 0.6 + i*sprite->getContentSize().width,winSize.height * 0.61));
			if(m_nSelectActivity == i)
			{
				sprite->setSelect(true);
				m_cDistanceSize = sprite->getContentSize().width;
			}
			else
			{
				sprite->setSelect(false);
			}
			m_ActivitySpriteArray->addObject(sprite);
			this->addChild(sprite,1);
			String* str1 ;
			str1 = String::createWithFormat(String::createWithFormat("t%d",550+i)->getCString());
			LabelTTF* label1;
			if(i <= 1)
			{
				label1 = LabelTTF::create(ConfigMgr::instance()->text(str1->getCString()),"",22,cocos2d::Size(750,0),kCCTextAlignmentLeft);
			}
			else
			{
				label1 = LabelTTF::create("","",22,cocos2d::Size(750,0),kCCTextAlignmentLeft);
			}
			label1->setPosition(Vec2(winSize.width * 0.05,winSize.height * 0.25));
			label1->setAnchorPoint(Vec2(0.0,0.5));
			label1->setColor(ccc3(0,0,0));
			this->addChild(label1,3);

			if(i!= 0)
			{
				label1->setVisible(false);
			}
			if(i == 0)
			{
				Sprite* enterNormal = Sprite::createWithSpriteFrame(spriteFrame("activityenter.png"));
				Sprite* enterSelect = Sprite::createWithSpriteFrame(spriteFrame("activityenter.png"));
				MenuItemSprite* enterItemSprite = MenuItemSprite::create(enterNormal,enterSelect,CC_CALLBACK_1(ActivityLayer::enterActivity,this));
				enterSelect->setScale(0.9f);
				enterSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
				m_EnterMenu = Menu::create(enterItemSprite,NULL);
				m_EnterMenu->setPosition(Vec2(winSize.width * 0.8,winSize.height * 0.1));
				this->addChild(m_EnterMenu,10);
			}
			m_ActivityContextArray->addObject(label1);
		}



		//activityTips = LabelTTF::create(ConfigMgr::instance()->text(String::createWithFormat("t%d",199)->getCString()),"",26);
		//activityTips->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.37));
		//activityTips->setColor(ccc3(0,0,0));
		//this->addChild(activityTips,3);

		Sprite* leftNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_3.png"));
		leftNormal->setFlipX(true);
		Sprite* leftSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_3.png"));
		leftSelect->setFlipX(true);
		Sprite* rightNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_3.png"));
		Sprite* rightSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_3.png"));
		MenuItemSprite* leftItem = MenuItemSprite::create(leftNormal,leftSelect,CC_CALLBACK_1(ActivityLayer::leftActivity,this));
		MenuItemSprite* rightItem = MenuItemSprite::create(rightNormal,rightSelect,CC_CALLBACK_1(ActivityLayer::rightActivity,this));
		Menu* arrowMenu = Menu::create(leftItem,rightItem,NULL);
		arrowMenu->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.6));
		arrowMenu->alignItemsHorizontallyWithPadding(winSize.width * 0.9);
		this->addChild(arrowMenu,4);
		
		
			//sprite->setPosition(Vec2(sprite1->getContentSize().width * 0.6,winSize.height * 0.61));
		//this->addChild(sprite,1);

		m_pWhiteBox = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_4.png"));
		m_pWhiteBox->setPosition(Vec2(m_cDistanceSize * 0.6,winSize.height * 0.61));
		this->addChild(m_pWhiteBox);

		m_pUpArrow = Sprite::createWithSpriteFrame(spriteFrame("dt_activity_0.png"));
		m_pUpArrow->setPosition(Vec2(m_cDistanceSize * 0.6,winSize.height * 0.61 - 80));
		this->addChild(m_pUpArrow);
		

		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
			(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(ActivityLayer::dismiss,this));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		pCloseButton->setPosition(Vec2(winSize.width*0.943,winSize.height*0.923));
		this->addChild(pCloseButton,1);
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;

}

void ActivityLayer::enterActivity(Object* obj)
{

	playButtonSound();
	int gameID = g_GlobalUnits.getGolbalUserData()->dwUserID;
	std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");

	char szMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)userPassword.c_str(), szMD5);
	String* str = String::createWithFormat("http://HD.91zjd.com/CheckValidUser.aspx?strUserID=%d&strPwd=%s",gameID,szMD5);
	CCLOG("%s",str->getCString());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	toAndridUserProtion(str->getCString());
#endif
}

void ActivityLayer::changeActivity(int type)
{
	playButtonSound();
	m_nSelectActivity = type;
	if(m_nSelectActivity == 0)
	{
		m_EnterMenu->setVisible(true);
	}
	else
	{
		m_EnterMenu->setVisible(false);
	}
	activityTips->setString(ConfigMgr::instance()->text(String::createWithFormat("t%d",500+m_nSelectActivity)->getCString()));

	for (unsigned int i = 0;i  < m_ActivitySpriteArray->count();i++)
	{
		ActivitySprite* sprite = (ActivitySprite*)m_ActivitySpriteArray->objectAtIndex(i);
		sprite->setSelect(false);
		if(i == type)
		{
			sprite->setSelect(true);
			m_pWhiteBox->setPosition(sprite->getPosition());
			m_pUpArrow->setPosition(Vec2(sprite->getPositionX(),sprite->getPositionY() - 80));
		}
	}
	for (unsigned int i = 0;i<m_ActivityContextArray->count();i++)
	{
		LabelTTF* sprite = (LabelTTF*)m_ActivityContextArray->objectAtIndex(i);
		sprite->setVisible(false);
		if(i == type)
		{
			sprite->setVisible(true);
		}
	}
}

void ActivityLayer::leftActivity(Object* obj)
{
	playButtonSound();
	if(m_nSelectActivity == 0)
	{
		return;
	}
	m_nSelectActivity--;
	if(m_nSelectActivity == 0)
	{
		m_EnterMenu->setVisible(true);
	}
	else
	{
		m_EnterMenu->setVisible(false);
	}
	activityTips->setString(ConfigMgr::instance()->text(String::createWithFormat("t%d",500+m_nSelectActivity)->getCString()));
	for (int i = 0; i<5 ; i++)
	{
		ActivitySprite* sprite = (ActivitySprite*)m_ActivitySpriteArray->objectAtIndex(i);
		sprite->setPosition(Vec2(sprite->getContentSize().width * 0.6 + i*sprite->getContentSize().width,winSize.height * 0.61));
		if(m_nSelectActivity == i)
		{
			sprite->setSelect(true);
			m_pWhiteBox->setPosition(sprite->getPosition());
			m_pUpArrow->setPosition(Vec2(sprite->getPositionX(),sprite->getPositionY() - 80));
		}
		else
		{
			sprite->setSelect(false);
		}
	}
	for (unsigned int i = 0;i<m_ActivityContextArray->count();i++)
	{
		LabelTTF* sprite = (LabelTTF*)m_ActivityContextArray->objectAtIndex(i);
		sprite->setVisible(false);
		if(i == m_nSelectActivity)
		{
			sprite->setVisible(true);
		}
	}
}

void ActivityLayer::rightActivity(Object* obj)
{
	playButtonSound();
	if(m_nSelectActivity == 4)
	{
		return;
	}
	m_nSelectActivity++;
	if(m_nSelectActivity == 0)
	{
		m_EnterMenu->setVisible(true);
	}
	else
	{
		m_EnterMenu->setVisible(false);
	}
	activityTips->setString(ConfigMgr::instance()->text(String::createWithFormat("t%d",500+m_nSelectActivity)->getCString()));
	for (int i = 0; i<5 ; i++)
	{
		ActivitySprite* sprite = (ActivitySprite*)m_ActivitySpriteArray->objectAtIndex(i);
		sprite->setPosition(Vec2(sprite->getContentSize().width * 0.6 + (i-1)*sprite->getContentSize().width,winSize.height * 0.61));
		if(m_nSelectActivity == i)
		{
			sprite->setSelect(true);
			m_pWhiteBox->setPosition(sprite->getPosition());
			m_pUpArrow->setPosition(Vec2(sprite->getPositionX(),sprite->getPositionY() - 80));
		}
		else
		{
			sprite->setSelect(false);
		}
	}
	for (unsigned int i = 0; i<m_ActivityContextArray->count();i++)
	{
		LabelTTF* sprite = (LabelTTF*)m_ActivityContextArray->objectAtIndex(i);
		sprite->setVisible(false);
		if(i == m_nSelectActivity)
		{
			sprite->setVisible(true);
		}
	}
}

void ActivityLayer::dismiss(Object *obj){
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);


}

void ActivityLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		dismiss(NULL);
	}
}


void ActivityLayer::toFreeLayer(Object* obj)
{
	playButtonSound();
	Director::getInstance()->replaceScene(FreeLayer::scene());
}