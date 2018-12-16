#include "RegistLayer.h"
#include "RoomLayer.h"
#include "GroupSprite.h"
#include "MovingLabelLayer.h"
#include "LoginLayer.h"
#include "PromptBox.h"
#include "utf-8.h"
#include "RoomLayer.h"
//#include "AgreementLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

using namespace cocos2d;
RegistLayer::RegistLayer()
{
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(RegistLayer::onRegisterFaild), MSG_UI_ANS_REGNFAIL, NULL);

	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(RegistLayer::onRegistSucc), MSG_UI_ANS_REGSUCC, NULL);
}
RegistLayer::~RegistLayer()
{
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_REGNFAIL);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_REGSUCC);
}

bool RegistLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! Layer::init());
		ccColor3B whiteColor = ccc3(255,255,255);
		ccColor3B blackColor = ccc3(0,0,0);
		winSize = WINSIZE;

		LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);

		//±≥æ∞
		Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame("room_01.png"));
		bk1->setAnchorPoint(Vec2(0,1));
		bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
		bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
		bk1->setScaleY(162 / bk1->getContentSize().height);
		this->addChild(bk1,0);

		float fontSize = 38;
		LabelTTF *title = LabelTTF::create(pConfigMgr->text("display_text.xml", "t6"), "", fontSize);
		title->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - bk1->getContentSize().height / 2));
		title->setColor(ccc3(255,255,255));
		this->addChild(title, 0);


		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
		Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
			(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(RegistLayer::dismiss,this));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		//pCloseButton->setPosition(Vec2(pCloseNormalButton->getContentSize().width / 2, SCREEN_HEIGHT - bk1->getContentSize().height / 2));
		pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
		this->addChild(pCloseButton,1);

		float delta1 = 25;
		float delta2 = 20;
		float delta3 = 30;
		Sprite *m_pAccountsBk = Sprite::createWithSpriteFrame(spriteFrame("regist_03.png"));
		m_pAccountsBk->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - bk1->getContentSize().height - delta1));
		m_pAccountsBk->setAnchorPoint(Vec2(0.5f,1));
		this->addChild(m_pAccountsBk,0);

		Sprite *m_pPassWordBk = Sprite::createWithSpriteFrame(spriteFrame("regist_03.png"));
		m_pPassWordBk->setPosition(Vec2(SCREEN_WIDTH/2, m_pAccountsBk->getPositionY() -
			m_pAccountsBk->getContentSize().height-delta2));
		m_pPassWordBk->setAnchorPoint(Vec2(0.5f,1));
		this->addChild(m_pPassWordBk,0);

		Sprite *m_pPassWordOkBk = Sprite::createWithSpriteFrame(spriteFrame("regist_03.png"));
		m_pPassWordOkBk->setPosition(Vec2(SCREEN_WIDTH/2, m_pPassWordBk->getPositionY() -
			m_pPassWordBk->getContentSize().height-delta2));
		m_pPassWordOkBk->setAnchorPoint(Vec2(0.5f,1));
		this->addChild(m_pPassWordOkBk,0);

		fontSize = 28;
		float accountStartX = SCREEN_WIDTH /2- m_pAccountsBk->getContentSize().width / 2 + 4;
		LabelTTF *accountsTitle = LabelTTF::create(pConfigMgr->text("display_text.xml", "t2"), "", fontSize);
		accountsTitle->setPosition(Vec2(accountStartX, m_pAccountsBk->getPositionY()-m_pAccountsBk->getContentSize().height/2));
		accountsTitle->setColor(ccc3(0,0,0));
		accountsTitle->setAnchorPoint(Vec2(0,0.5f));
		this->addChild(accountsTitle, 0);

		LabelTTF *passwordTitle = LabelTTF::create(pConfigMgr->text("display_text.xml", "t3"), "", fontSize);
		passwordTitle->setPosition(Vec2(accountStartX, m_pPassWordBk->getPositionY()-m_pPassWordBk->getContentSize().height/2));
		passwordTitle->setColor(ccc3(0,0,0));
		passwordTitle->setAnchorPoint(Vec2(0,0.5f));
		this->addChild(passwordTitle, 0);

		LabelTTF *passwordOkTitle = LabelTTF::create(pConfigMgr->text("display_text.xml", "t7"), "", fontSize);
		passwordOkTitle->setPosition(Vec2(accountStartX, m_pPassWordOkBk->getPositionY()-m_pPassWordOkBk->getContentSize().height/2));
		passwordOkTitle->setColor(ccc3(0,0,0));
		passwordOkTitle->setAnchorPoint(Vec2(0,0.5f));
		this->addChild(passwordOkTitle, 0);

		fontSize = 25;
		float placeHolderFontSize = 8;
		float passwordInputStartX = accountStartX+60;
		float passwordInputWidth = SCREEN_WIDTH - passwordInputStartX - (SCREEN_WIDTH/2 - m_pAccountsBk->getContentSize().width / 2);
		// ‰»ÎøÚ

		m_pUserNameKuang = EditBox::create(cocos2d::Size(passwordInputWidth, 48), Scale9Sprite::create());
		m_pUserNameKuang->setPosition(Vec2(passwordInputStartX, accountsTitle->getPositionY()));
		m_pUserNameKuang->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		m_pUserNameKuang->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_pUserNameKuang->setAnchorPoint(Vec2(0,0.5f));
		m_pUserNameKuang->setFontColor(ccc3(0,0,0));
		m_pUserNameKuang->setFontSize(fontSize);
		m_pUserNameKuang->setColor(ccc3(112,112,112));
		m_pUserNameKuang->setPlaceHolder(pConfigMgr->text("display_text.xml", "t4"));
		m_pUserNameKuang->setPlaceholderFontColor(ccc3(160,160,160));
		m_pUserNameKuang->setPlaceholderFontSize(placeHolderFontSize);

		m_pPassWordKuang = EditBox::create(cocos2d::Size(passwordInputWidth,48), Scale9Sprite::create());
		m_pPassWordKuang->setPosition(Vec2(passwordInputStartX, passwordTitle->getPositionY()));
		m_pPassWordKuang->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
		m_pPassWordKuang->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_pPassWordKuang->setMaxLength(20);
		m_pPassWordKuang->setAnchorPoint(Vec2(0,0.5f));
		m_pPassWordKuang->setFontColor(ccc3(0,0,0));
		m_pPassWordKuang->setFontSize(fontSize);
		m_pPassWordKuang->setColor(ccc3(112,112,112));
		m_pPassWordKuang->setPlaceHolder(pConfigMgr->text("display_text.xml", "t5"));
		m_pPassWordKuang->setPlaceholderFontColor(ccc3(160,160,160));
		m_pPassWordKuang->setPlaceholderFontSize(placeHolderFontSize);

		m_pPassWordKuangOK = EditBox::create(cocos2d::Size(passwordInputWidth,48), Scale9Sprite::create());
		m_pPassWordKuangOK->setPosition(Vec2(passwordInputStartX, passwordOkTitle->getPositionY()));
		m_pPassWordKuangOK->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
		m_pPassWordKuangOK->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_pPassWordKuangOK->setMaxLength(20);
		m_pPassWordKuangOK->setAnchorPoint(Vec2(0,0.5f));
		m_pPassWordKuangOK->setFontColor(ccc3(0,0,0));
		m_pPassWordKuangOK->setFontSize(fontSize);
		m_pPassWordKuangOK->setColor(ccc3(112,112,112));
		m_pPassWordKuangOK->setPlaceHolder(pConfigMgr->text("display_text.xml", "t8"));
		m_pPassWordKuangOK->setPlaceholderFontColor(ccc3(160,160,160));
		m_pPassWordKuangOK->setPlaceholderFontSize(placeHolderFontSize);

		this->addChild(m_pUserNameKuang);
		this->addChild(m_pPassWordKuang);
		this->addChild(m_pPassWordKuangOK);

		fontSize = 30;
		LabelTTF *hintLabel = LabelTTF::create("hintLabel"/*pConfigMgr->text("display_text.xml", "t9")*/, "", fontSize);
		hintLabel->setPosition(Vec2(accountStartX, m_pPassWordOkBk->getPositionY()-m_pPassWordOkBk->getContentSize().height-delta2));
		hintLabel->setColor(ccc3(160,160,160));
		hintLabel->setAnchorPoint(Vec2(0,0.5f));
		this->addChild(hintLabel, 0);

		//
		Sprite * registNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("regist_01.png"));
		Sprite * registSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("regist_02.png"));
		MenuItemSprite *registItemSprite = MenuItemSprite::create(registNormalSprite, registSelectSprite,  CC_CALLBACK_1(RegistLayer::showAgreePlayerXieYi,this));
		Menu *registMenu = Menu::create(registItemSprite,NULL);
		registMenu->setPosition(Vec2(SCREEN_WIDTH/2, hintLabel->getPositionY() - delta3 * 2));
		this->addChild(registMenu);

		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;

}

void RegistLayer::setRegistLayerFrom(int num)
{
	m_nRegistFrom = num;
}

void RegistLayer::selectMan(Object* obj)
{
	nGenter = 1;
	if(nGenter == 1)
	{
		spriteSlectSex->setPosition(Vec2(winSize.width*0.195,winSize.height*0.406));
	}
	else
	{
		spriteSlectSex->setPosition(Vec2(winSize.width*0.195,winSize.height*0.615));
	}
}

void RegistLayer::selectWoman(Object* obj)
{
	nGenter = 2;
	if(nGenter == 1)
	{
		spriteSlectSex->setPosition(Vec2(winSize.width*0.195,winSize.height*0.406));
	}
	else
	{
		spriteSlectSex->setPosition(Vec2(winSize.width*0.195,winSize.height*0.615));
	}
}

void RegistLayer::dismiss(Object *obj){
	playButtonSound();
	if(m_nRegistFrom == 0)
	{
		Scene* scene = Scene::create();
		RoomLayer * pRoomLayer = RoomLayer::create();
		pRoomLayer->closeLoginAward();
		pRoomLayer->automaticLogin(false);
		scene->addChild(pRoomLayer);
		Director::getInstance()->replaceScene(scene);
	}
	else
	{
		Director::getInstance()->replaceScene(LoginLayer::scene());
	}
}

void RegistLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{	
		if(pConfigMgr->m_Config.m_bToAndroid)
		{
			pConfigMgr->m_Config.m_bToAndroid = false;
			CCLOG("222222222222222222now m_config.m_btoandroid is true  to false");
			return;
		}
		else
		{
			dismiss(NULL);
		}
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_MENU)
	{

	}
}

void RegistLayer::showPlayerXieYi(Object *obj){
	playButtonSound();
	pConfigMgr->m_Config.m_bToAndroid = true;
	CCLOG("222222222222222222222222now m_config.m_btoandroid is false  to true");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	toAndridUserProtion("http://www.91zjd.com/Agreement.html");
#endif

}

void RegistLayer::onRegistSucc(Object* obj)
{
	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	layer->showLoginAward();
//	layer->showMoveingLabelTips(pConfigMgr->text("t407"));
	Scene* scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void RegistLayer::onRegisterFaild(Object* obj)
{
	String* str = (String*)obj;
	PromptBox* box = PromptBox::PromptBoxWith(Vec2(427, SCREEN_WIDTH/2), mPromptTypePasswordFind);
	box->setPromptText(str->getCString());
	this->addChild(box,100);
}

void RegistLayer::showAgreePlayerXieYi(Object *obj){
	playButtonSound();
	winSize = WINSIZE;
	string szAccount  = m_pUserNameKuang->getText();
	string szPassword = m_pPassWordKuang->getText();
	string szSPassword = m_pPassWordKuangOK->getText();

	int n = szAccount.length();

	if (szPassword != szSPassword)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t28"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}


	if(szAccount.length() < 6 || szAccount.length() > 12)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t29"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}

	if (szPassword.length() < 6 || szPassword.length() > 12)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t457"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}

	if ( !isLegal(szAccount) || !isLegal(szPassword))
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t30"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}
	bool bRet = pMsgDispatch->connectLoginServer();
	if (!bRet)
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
		this->addChild(box,100);
	}else{
		EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),n
			
			
			,nGenter,0);
		//EntityMgr::instance()->login()->setUserLogin(szAccount.c_str(),szPassword.c_str());
	}
}

bool RegistLayer::isLegal(string str)
{
	//const char* s = str.c_str();
	char s[128] = {0};
	memcpy(s, str.c_str(), min(sizeof(s)-1, str.length()));
	CCLOG("get user accout str = %s, strlen(str)=%d",str.c_str(), strlen(str.c_str()));
	CCLOG("get user accout char = %s strlen(s)=%d", s, strlen(s));
	int i = 0;
	while (i++ < str.length() - 1)
	{
		if (s[i] < 0 || s[i] > 127)
		{
			//("中文");
			CCLOG("registLayer chinese");
		}
		else if (s[i]>='0' && s[i]<='9')
		{
			//("数字");
			CCLOG("registLayer number");
		}
		else if (s[i]>='A' && s[i]<='Z' || s[i]>='a' && s[i]<='z')
		{
			CCLOG("registLayer a b c");
		}
		else
		{
			//("其它");
			CCLOG("RegistLayer::isLegal false s[%d]:%d %s", i, s[i], &s[i]);
			return  false;
		}
	}
	return  true;
}

