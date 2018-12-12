#include "LoginLayer.h"
#include "Login.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "GroupSprite.h"
#include "RegistLayer.h"
#include "ModifyPassWord.h"
#include "MovingLabelLayer.h"
#include "RoomOption.h"
#include "PromptBox.h"
#include "SceneControl.h"
using namespace CocosDenshion;
Scene* LoginLayer::scene()
{
	Scene *scene = Scene::create();
	LoginLayer *layer = LoginLayer::create();
	scene->addChild(layer);
	return scene;
}

LoginLayer::LoginLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::onAnsLoginFinish), MSG_UI_ANS_LOGINFINISH, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::onLoginFaild), MSG_UI_ANS_LOGINFAIL, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::onFindPassword), MSG_UI_ANS_FINDPASS, NULL);
}
LoginLayer::~LoginLayer()
{
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFINISH);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_FINDPASS);
    
    this->unschedule(schedule_selector(LoginLayer::checkIfAutoLogin));
}

void LoginLayer::setLogin(bool loginBool)
{
	m_bLogin = loginBool;
}

bool LoginLayer::init(bool bLogin)
{
	if (!Layer::init())
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();
	m_bLogin = true;
	m_from = false;

	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);
	Sprite *logoing = Sprite::createWithSpriteFrame(spriteFrame("beark.png"));
	logoing->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(logoing,0);

	Sprite *pLogo = Sprite::createWithSpriteFrame(spriteFrame("login_02.png"));
	if (pLogo)
	{
		pLogo->setAnchorPoint(Vec2(0, 1));
		pLogo->setPosition(Vec2(winSize.width*0.5 - pLogo->getContentSize().width / 2, winSize.height-100));
		this->addChild(pLogo, 0);
	}
	
	float fontSize = 45;

	float delta1 = 25;
	float delta2 = 20;
	float delta3 = 30;
	Sprite *m_pAccountsBk = Sprite::createWithSpriteFrame(spriteFrame("home_05.png"));
	m_pAccountsBk->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - 120 - pLogo->getContentSize().height - delta1));
	m_pAccountsBk->setAnchorPoint(Vec2(0.5f,1));
	this->addChild(m_pAccountsBk,0);

	Sprite *m_pPassWordBk = Sprite::createWithSpriteFrame(spriteFrame("home_05.png"));
	m_pPassWordBk->setPosition(Vec2(SCREEN_WIDTH/2, m_pAccountsBk->getPositionY() -	m_pAccountsBk->getContentSize().height-delta2));
	m_pPassWordBk->setAnchorPoint(Vec2(0.5f,1));
	this->addChild(m_pPassWordBk,0);

	float accountStartX = SCREEN_WIDTH /2- m_pAccountsBk->getContentSize().width / 2 + 4;
	Sprite *accountsTitle =  Sprite::createWithSpriteFrame(spriteFrame("home_07.png"));
	accountsTitle->setPosition(Vec2(accountStartX+10, m_pAccountsBk->getPositionY()-m_pAccountsBk->getContentSize().height/2));
	accountsTitle->setColor(ccc3(0,0,0));
	accountsTitle->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(accountsTitle, 0);

	Sprite *passwordTitle =  Sprite::createWithSpriteFrame(spriteFrame("home_08.png"));
	passwordTitle->setPosition(Vec2(accountStartX+10, m_pPassWordBk->getPositionY()-m_pPassWordBk->getContentSize().height/2));
	passwordTitle->setColor(ccc3(0,0,0));
	passwordTitle->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(passwordTitle, 0);	

	fontSize = 50;
	int placeHolderFontSize = 45;
	float passwordInputStartX = accountStartX+50;
	float passwordInputWidth = SCREEN_WIDTH - passwordInputStartX - (SCREEN_WIDTH/2 - m_pAccountsBk->getContentSize().width / 2);

	passwordInput = EditBox::create(cocos2d::Size(passwordInputWidth,60), Scale9Sprite::create());
	passwordInput->setPosition(Vec2(passwordInputStartX, passwordTitle->getPositionY()));
	passwordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	passwordInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	passwordInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行
	passwordInput->setMaxLength(20);
	passwordInput->setAnchorPoint(Vec2(0,0.5f));
	passwordInput->setFontColor(ccc3(0,0,0));
	passwordInput->setFontSize(placeHolderFontSize);
	passwordInput->setColor(ccc3(112,112,112));
	passwordInput->setPlaceHolder(pConfigMgr->text("display_text.xml", "t5"));
	passwordInput->setPlaceholderFontColor(ccc3(160,160,160));
	passwordInput->setPlaceholderFontSize(placeHolderFontSize);

	userInput = EditBox::create(cocos2d::Size(passwordInputWidth,60), Scale9Sprite::create());
	userInput->setPosition(Vec2(passwordInputStartX, accountsTitle->getPositionY()));
	userInput->setTag(FLTAG_AccountsTex);
	userInput->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	userInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	userInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行
	userInput->setAnchorPoint(Vec2(0,0.5f));
	userInput->setFontColor(ccc3(0,0,0));
	userInput->setFontSize(placeHolderFontSize);
	userInput->setColor(ccc3(112,112,112));
	userInput->setPlaceHolder(pConfigMgr->text("display_text.xml", "t4"));
	userInput->setPlaceholderFontColor(ccc3(160,160,160));
	userInput->setPlaceholderFontSize(placeHolderFontSize);

	this->addChild(userInput);
	this->addChild(passwordInput);

	Sprite * denglNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("home_03.png"));
	Sprite * denglSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("home_06.png"));
	Sprite * denglDisSprite = Sprite::createWithSpriteFrame(spriteFrame("home_03.png"));
	denglDisSprite->setColor(ccc3(100,100,100));
	m_pDengluItemSprite = MenuItemSprite::create(denglNormalSprite, denglSelectSprite,denglDisSprite, CC_CALLBACK_1(LoginLayer::onLogin,this));
	m_pLoginMenu = Menu::create(m_pDengluItemSprite,NULL);
	m_pLoginMenu->setPosition(Vec2(SCREEN_WIDTH/2, m_pPassWordBk->getPositionY() - m_pPassWordBk->getContentSize().height -
	denglNormalSprite->getContentSize().height/2-delta3));
	this->addChild(m_pLoginMenu);

	Sprite * registNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("login_05.png"));
	Sprite * registSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("login_05.png"));
	Sprite* guestNormal = Sprite::createWithSpriteFrame(spriteFrame("login_07.png"));
	Sprite* guestSelect = Sprite::createWithSpriteFrame(spriteFrame("login_08.png"));
	MenuItemSprite*regist = MenuItemSprite::create(registNormalSprite,registSelectSprite, CC_CALLBACK_1(LoginLayer::onRegist,this));
	regist->setPosition(SCREEN_WIDTH/2, m_pLoginMenu->getPositionY() - denglNormalSprite->getContentSize().height - delta2);
	m_pRegisterMenu = Menu::create(regist,NULL);
	m_pRegisterMenu->setPosition(Vec2(5, registNormalSprite->getPositionY()-delta2));
	this->addChild(m_pRegisterMenu);

	std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
	std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
    bool auto_login = UserDefault::getInstance()->getBoolForKey("AUTO_LOGIN", false);
	if(userAccount.length() > 1 &&userPassword.length() > 1)
	{
		userInput->setText(userAccount.c_str());
		passwordInput->setText(userPassword.c_str());
	}

	this->setKeypadEnabled(true);

	if (!bLogin)
	{
		return  true;
	}
	return true;
}

void LoginLayer::checkIfAutoLogin(float dt)
{
    bool auto_login = UserDefault::getInstance()->getBoolForKey("AUTO_LOGIN", false);
    if (auto_login == true) {
        this->unschedule(schedule_selector(LoginLayer::checkIfAutoLogin));
        this->onLogin(NULL);
        UserDefault::getInstance()->setBoolForKey("AUTO_LOGIN", false);
    }
}

void LoginLayer::onEnter()
{
    Layer::onEnter();
}
void LoginLayer::onExit()
{
	Layer::onExit();
}
void LoginLayer::onLoginFaild(Object* obj)
{
	m_pDengluItemSprite->setEnabled(true);
	CCLOG("login faied");
	PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptpasswordUsed);
	this->addChild(box,100);
}
void LoginLayer::setIsfaildLogin(bool mIsFaildLogin)
{

}
void LoginLayer::onRegist(Object *obj)
{
	playButtonSound();
	Scene* scene = Scene::create();
	RegistLayer * pRegisterLayer = RegistLayer::create();
	pRegisterLayer->setRegistLayerFrom(1);
	scene->addChild(pRegisterLayer);
	Director::getInstance()->replaceScene(scene);
}

void LoginLayer::onLogin(Object *obj)
{
	CCLOG("onLogin");
	playButtonSound();
	m_pDengluItemSprite->setEnabled(true);
	char* user = const_cast<char*>( userInput->getText());
	char* password = const_cast<char*>(passwordInput->getText());



    if(strlen(user) == 0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t36"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer);
		return;
	}

	bool bRet = pMsgDispatch->connectLoginServer();
	if (!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t26"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer);
		return;
	}
	else
	{
		EntityMgr::instance()->login()->setUserLogin(user, password);
	}
}
void LoginLayer::GuestLogon(Object *obj)
{

}

void LoginLayer::setBoolFrom(bool b){
	m_from = b;
}

void LoginLayer::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->end();
}

void LoginLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
	}
	if (keycode == EventKeyboard::KeyCode::KEY_F1)
	{
		onAnsLoginFinish(NULL);
	}
}

void LoginLayer::onAnsLoginFinish(Object* obj)
{
	m_pDengluItemSprite->setEnabled(true);
	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	Scene* scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(SceneControl::sharedSceneControl()->getScene(scene));
}

void LoginLayer::onFindPassword(Object* obj)
{
	playButtonSound();
	tagGetPassObj* pObj = (tagGetPassObj*)obj;
	CCLOG((const char*)pObj->szErrorDescribe);
	if(pObj->lErrCode == 0)
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypePasswordFind);
		box->setPromptText(String::createWithFormat("%s%s",pConfigMgr->text("t164"),pObj->szPassword)->getCString());
		this->addChild(box,30);
		UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD",pObj->szPassword);
		passwordInput->cleanup();
		passwordInput->setText(pObj->szPassword);

	}else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s",(const char*)pObj->szErrorDescribe)->getCString(),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer);
	}

	userInput->setTouchEnabled(true);
	passwordInput->setTouchEnabled(true);
	m_pLoginMenu->setTouchEnabled(true);
	m_pRegisterMenu->setTouchEnabled(true);
}

void LoginLayer::changePassWord(Object* obj)
{
	playButtonSound();
	if(m_bLogin)
	{
		Director::getInstance()->replaceScene(ModifyPassWord::scene());
	}
	else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t455"), Vec2(427, SCREEN_WIDTH/2));
		this->addChild(layer);
	}

}

std::string LoginLayer::createAccount()
{
	string szAccount;
	int t = 0;
	srand((unsigned)time(NULL));
	for (int i = 0; i < 4; i++)
	{
		//t = rand() % 2 ? 65 : 97;
		t = 97;
		t += rand() % 26;
		szAccount += (char)t;
        
	}
    
	for (int i = 0; i < 4; i++)
	{
		t = rand() % 10;
		szAccount +=  ans::AnsString::int2String(t);
        
	}
	return  szAccount;
}

std::string LoginLayer::createPassword()
{
	string szPassword;
	int t = 0;
	srand((unsigned)time(NULL));
    
	for (int i = 0; i < 6; i++)
	{
		t = rand() % 10;
		szPassword +=  ans::AnsString::int2String(t);
	}
	return  szPassword;
}