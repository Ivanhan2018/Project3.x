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
#include "LotterySceneControl.h"
#include "MyNSString.h"//<by hxh>
#include "BetLimit.h"
#include "LotteryGlobal.h"
#include "VersionControl.h"
#include "ValidateLayer.h"

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
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::getKeFuUrl), "GetKeFuUrl", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::getCaiZhongList), "GetCaiZhongList", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::onValidated), "Validated", NULL);
	EntityMgr::instance()->getDispatch()->setIsLogin(true);
}
LoginLayer::~LoginLayer()
{
	EntityMgr::instance()->getDispatch()->setIsLogin(false);

	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFINISH);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_FINDPASS);
	NotificationCenter::getInstance()->removeObserver(this, "GetKeFuUrl");   
	NotificationCenter::getInstance()->removeObserver(this, "GetCaiZhongList");  
	NotificationCenter::getInstance()->removeObserver(this, "Validated"); 
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
	kefuStr = NULL;
	//客服地址
	EntityMgr::instance()->getDispatch()->sendPacketWithGetKeFuURL();

	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);
	Sprite *logoing = Sprite::createWithSpriteFrame(spriteFrame("beark.png"));	
	//logoing->setColor(ccc3(55, 32, 23));
	logoing->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	logoing->setScaleX(SCREEN_WIDTH / logoing->getContentSize().width);	
	logoing->setScaleY(SCREEN_HEIGHT / logoing->getContentSize().height);
	this->addChild(logoing,0);

	int delta = 0;

#ifdef VER_QIANBO
	delta = 183;
#endif
	
	Sprite *pLogo = Sprite::createWithSpriteFrame(spriteFrame("login_02.png"));
	if (pLogo)
	{
		pLogo->setAnchorPoint(Vec2(0, 1));
		Vec2 logoPos=Vec2(winSize.width*0.5 - pLogo->getContentSize().width / 2-20, winSize.height-200 - delta);
		pLogo->setPosition(logoPos);
		this->addChild(pLogo, 0);
		auto fun0=RepeatForever::create(Sequence::create(ScaleTo::create(0.5f,0.95f),ScaleTo::create(0.5f,0.85f),NULL));
		pLogo->runAction(fun0);
	}
	
	float fontSize = 45;

	//版本号
	LabelTTF *ltfVersion = LabelTTF::create(String::createWithFormat("%s%s",ConfigMgr::instance()->text("display_DuangDong.xml", "t1620"),ConfigMgr::instance()->text("display_config.xml", "t5023"))->getCString(), "", fontSize);
	ltfVersion->setPosition(Vec2(SCREEN_WIDTH/2, 50));
	ltfVersion->setFontName("");
	ltfVersion->setFontSize(28);
	ltfVersion->setFontFillColor(ccc3(154,131,123));
	this->addChild(ltfVersion, 0);

#ifdef VER_QIANBO
	ltfVersion->setFontFillColor(ccc3(255,255,255));
#endif

#ifdef VER_369
	ltfVersion->setFontFillColor(ccc3(255,255,255));
#endif

	float delta1 = 25;
	float delta2 = 20;
	float delta3 = 30;
	float height=414;//pLogo->getContentSize().height;
	Sprite *m_pAccountsBk = Sprite::createWithSpriteFrame(spriteFrame("home_05.png"));
	m_pAccountsBk->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - 120 - height - delta1 - delta * 1.5f));
	m_pAccountsBk->setAnchorPoint(Vec2(0.5f,1));
	this->addChild(m_pAccountsBk,0);

	//背景
	Sprite *m_pPassWordBk = Sprite::createWithSpriteFrame(spriteFrame("home_05.png"));
	m_pPassWordBk->setPosition(Vec2(SCREEN_WIDTH/2, m_pAccountsBk->getPositionY() -	m_pAccountsBk->getContentSize().height-delta2));
	m_pPassWordBk->setAnchorPoint(Vec2(0.5f,1));
	this->addChild(m_pPassWordBk,0);

	//帐号
	float accountStartX = SCREEN_WIDTH /2- m_pAccountsBk->getContentSize().width / 2 + 4;
	Sprite *accountsTitle =  Sprite::createWithSpriteFrame(spriteFrame("home_07.png"));
	accountsTitle->setPosition(Vec2(accountStartX+10, m_pAccountsBk->getPositionY()-m_pAccountsBk->getContentSize().height/2));
	accountsTitle->setColor(ccc3(0,0,0));
	accountsTitle->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(accountsTitle, 0);

	//密码
	Sprite *passwordTitle =  Sprite::createWithSpriteFrame(spriteFrame("home_08.png"));
	passwordTitle->setPosition(Vec2(accountStartX+10, m_pPassWordBk->getPositionY()-m_pPassWordBk->getContentSize().height/2));
	passwordTitle->setColor(ccc3(0,0,0));
	passwordTitle->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(passwordTitle, 0);

	fontSize = 50;
	int placeHolderFontSize = 45;
	float passwordInputStartX = accountStartX+50;
	float passwordInputWidth = SCREEN_WIDTH - passwordInputStartX - (SCREEN_WIDTH/2 - m_pAccountsBk->getContentSize().width / 2);
	cocos2d::Color3B clBack = ccc3(112,112,112);
	cocos2d::Color3B clFontBack = ccc3(0,0,0);

#ifdef VER_QIANBO  //其它版本
	int tempDelta = 30;
	accountsTitle->setPosition(Vec2(accountStartX + 10 + tempDelta, m_pAccountsBk->getPositionY()-m_pAccountsBk->getContentSize().height/2));
	passwordTitle->setPosition(Vec2(accountStartX + 10 + tempDelta, m_pPassWordBk->getPositionY()-m_pPassWordBk->getContentSize().height/2));
	accountsTitle->setColor(ccc3(255,255,255));
	passwordTitle->setColor(ccc3(255,255,255));
	passwordInputStartX += tempDelta * 3;
	passwordInputWidth -= tempDelta * 3;
	clBack = ccc3(255, 255, 255);
	clFontBack = ccc3(0, 103, 69);
#endif

#ifdef VER_369  //其它版本
	int tempDelta = 30;
	accountsTitle->setPosition(Vec2(accountStartX + 10 + tempDelta, m_pAccountsBk->getPositionY()-m_pAccountsBk->getContentSize().height/2));
	passwordTitle->setPosition(Vec2(accountStartX + 10 + tempDelta, m_pPassWordBk->getPositionY()-m_pPassWordBk->getContentSize().height/2));
	accountsTitle->setColor(ccc3(255,255,255));
	passwordTitle->setColor(ccc3(255,255,255));
	passwordInputStartX += tempDelta * 3;
	passwordInputWidth -= tempDelta * 3;
#endif

	//密码
	passwordInput = EditBox::create(cocos2d::Size(passwordInputWidth,60), "blank.png");
	passwordInput->setPosition(Vec2(passwordInputStartX, passwordTitle->getPositionY()));
	passwordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	passwordInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	passwordInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行
	passwordInput->setMaxLength(20);
	passwordInput->setAnchorPoint(Vec2(0,0.5f));
	passwordInput->setFontColor(clFontBack);
	passwordInput->setFontName("");
	passwordInput->setFontSize(placeHolderFontSize);
	passwordInput->setColor(clBack);
	passwordInput->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t5"));
	passwordInput->setPlaceholderFontColor(ccc3(160,160,160));
	passwordInput->setPlaceholderFontSize(placeHolderFontSize);
	//用户名
	userInput = EditBox::create(cocos2d::Size(passwordInputWidth,60), "blank.png");
	userInput->setPosition(Vec2(passwordInputStartX, accountsTitle->getPositionY()));
	userInput->setTag(FLTAG_AccountsTex);
	userInput->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	userInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	userInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行
	userInput->setAnchorPoint(Vec2(0,0.5f));
	userInput->setFontColor(clFontBack);
	userInput->setFontName("");
	userInput->setFontSize(placeHolderFontSize);
	userInput->setColor(clBack);
	userInput->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t4"));
	userInput->setPlaceholderFontColor(ccc3(160,160,160));
	userInput->setPlaceholderFontSize(placeHolderFontSize);

	this->addChild(userInput);
	this->addChild(passwordInput);

	//登录按钮
	Sprite * denglNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("home_03.png"));
	Sprite * denglSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("home_06.png"));
	Sprite * denglDisSprite = Sprite::createWithSpriteFrame(spriteFrame("home_03.png"));
	denglDisSprite->setColor(ccc3(100,100,100));
	m_pDengluItemSprite = MenuItemSprite::create(denglNormalSprite, denglSelectSprite,denglDisSprite, CC_CALLBACK_1(LoginLayer::onLogin,this));
	m_pLoginMenu = Menu::create(m_pDengluItemSprite,NULL);
	m_pLoginMenu->setPosition(Vec2(SCREEN_WIDTH/2, m_pPassWordBk->getPositionY() - m_pPassWordBk->getContentSize().height -
		denglNormalSprite->getContentSize().height/2-delta3));
	this->addChild(m_pLoginMenu);

	auto fun=RepeatForever::create(Sequence::create(ScaleTo::create(0.5f,1.05f),ScaleTo::create(0.5f,0.95f),NULL));
	m_pDengluItemSprite->runAction(fun);

	//
	Sprite * registNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("home_04.png"));
	Sprite * registSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("home_04_2.png"));
	Sprite* guestNormal = Sprite::createWithSpriteFrame(spriteFrame("home_04_3.png"));
	Sprite* guestSelect = Sprite::createWithSpriteFrame(spriteFrame("home_04_4.png"));//
	MenuItemSprite*regist = MenuItemSprite::create(registNormalSprite,registSelectSprite, CC_CALLBACK_1(LoginLayer::toRememberPsw,this));	
	MenuItemSprite* guestLogon = MenuItemSprite::create(guestNormal, guestSelect,CC_CALLBACK_1(LoginLayer::toLosePsw,this));	
	m_pRegisterMenu = Menu::create(regist,guestLogon,NULL);
	m_pRegisterMenu->alignItemsHorizontallyWithPadding(160);
	m_pRegisterMenu->setPosition(Vec2(SCREEN_WIDTH / 2, m_pLoginMenu->getPositionY() / 2));
	this->addChild(m_pRegisterMenu);

#ifdef VER_QIANBO
	m_pRegisterMenu->setPosition(Vec2(SCREEN_WIDTH / 2, m_pLoginMenu->getPositionY() * 2 / 3));
#endif // VER_QIANBO

#ifdef VER_369
	m_pRegisterMenu->setPosition(m_pLoginMenu->getPosition() + Vec2(0, 5));
	m_pRegisterMenu->alignItemsHorizontallyWithPadding(200);
	m_pLoginMenu->setPositionY(m_pRegisterMenu->getPositionY() - 110);
#endif


	bool isSave = UserDefault::getInstance()->getBoolForKey("SAVE_PSW",false);
	if(isSave)
	{
		regist->selected();
	}

	std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
	std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
    bool auto_login = UserDefault::getInstance()->getBoolForKey("AUTO_LOGIN", false);
	//std::string userAccount = DEFAULT_ACCOUNT;
	//std::string userPassword = DEFAULT_PASSWORD;
	if(userAccount.length() > 1 &&userPassword.length() > 1)
	{
		// add by hxh 20160804
// #ifdef WIN32
// 		MyNSString::GBKToUTF8(userAccount);
// #endif
		userInput->setText(userAccount.c_str());
		passwordInput->setText(userPassword.c_str());
	}

    //this->schedule(schedule_selector(LoginLayer::checkIfAutoLogin), 1, true, 0.5);
	this->setKeypadEnabled(true);

	//下雨效果
	ParticleSystem* ps1 = ParticleRain::create();
	ps1->setTexture(Director::getInstance()->getTextureCache()->addImage("ball.png"));
	ps1->setPosition(Point(SCREEN_WIDTH/2,SCREEN_HEIGHT));
	this->addChild(ps1,10);

	ParticleSystem* ps = ParticleRain::create();
	ps->setTexture(Director::getInstance()->getTextureCache()->addImage("ball.png"));
	ps->setPosition(Point(SCREEN_WIDTH/2,0));
	ps->setRotation(180.0f);
	this->addChild(ps,10);

	return true;
}

void LoginLayer::checkIfAutoLogin(float dt)
{
    bool auto_login = UserDefault::getInstance()->getBoolForKey("AUTO_LOGIN", false);
    if (auto_login == true) {
        this->unschedule(schedule_selector(LoginLayer::checkIfAutoLogin));
        this->onLogin(NULL);
        UserDefault::getInstance()->setBoolForKey("AUTO_LOGIN", false);
        //            UserDefault::getInstance()->flush();
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

	Dictionary* dic = (Dictionary* )obj;
	if(dic == nullptr) return;

	Integer* Error_Code = (Integer *)dic->objectForKey("lErrorCode");
	String* desc = (String *)dic->objectForKey("szDescribeString");

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(desc->getCString(),Vec2(winSize.width * 0.5,winSize.height * 0.5));
	this->addChild(layer, 255);	
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
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t36"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer);
		return;
	}	

	//先关掉之前的登陆，否则会在断线重连后无法登陆
	EntityMgr::instance()->getDispatch()->closeLoginSocket();//这句不能被注释掉，否则会导致bug：启动app切换到后台一段时间后再切换到前台，此时点击登录按钮无法登录

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if (!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t26"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer);
		return;
	}
	else
	{
		// add by hxh 20160804
		string account=user;
#ifdef WIN32
		MyNSString::UTF8ToGBK(account);
#endif
		EntityMgr::instance()->login()->setUserLogin(account, password);
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
		event->stopPropagation();
	}
}

void LoginLayer::onAnsLoginFinish(Object* obj)
{
	if(EntityMgr::instance()->getDispatch()->m_bValidate == 0)
	{
		onValidated(nullptr);
	}else
	{
		//开始验证		
		ValidateLayer::show(EntityMgr::instance()->getDispatch()->m_pPhoneNum.c_str(), 0);
	}	
}

void LoginLayer::onValidated(Object *obj)
{
	m_pDengluItemSprite->setEnabled(true);
	UserDefault::getInstance()->setBoolForKey("SHOWTIPS",false);	
	
	Integer* tempInt = (Integer *)obj;
	if(tempInt != nullptr && tempInt->getValue() == -1)
	{
		EntityMgr::instance()->getDispatch()->SendExitGame();//退出游戏
		return;
	}

	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	Scene* scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LoginLayer::onFindPassword(Object* obj)
{
	playButtonSound();
	tagGetPassObj* pObj = (tagGetPassObj*)obj;
	CCLOG((const char*)pObj->szErrorDescribe);
	if(pObj->lErrCode == 0)  //
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypePasswordFind);
		box->setPromptText(String::createWithFormat("%s%s",ConfigMgr::instance()->text("t164"),pObj->szPassword)->getCString());
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
	//修改密码   现在的安全中心
	playButtonSound();
	if(m_bLogin)
	{
		Director::getInstance()->replaceScene(ModifyPassWord::scene());
	}
	else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t455"), Vec2(427, SCREEN_WIDTH/2));
		this->addChild(layer);
	}

}

std::string LoginLayer::createAccount()
{
	//此方法不可用  创建用户
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
	//创建密码
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

void LoginLayer::toRememberPsw(Object *obj)
{
	//记录密码
	auto remember = (MenuItemSprite *)obj;
	bool isSelected = UserDefault::getInstance()->getBoolForKey("SAVE_PSW",false);
	isSelected = !isSelected;
	if(isSelected)
	{
		remember->selected();
	}else
	{
		remember->unselected();
	}
	UserDefault::getInstance()->setBoolForKey("SAVE_PSW",isSelected);
}

void LoginLayer::toLosePsw(Object *obj)
{
	//忘记密码
	playButtonSound();

	if(kefuStr == nullptr)
	{
		kefuStr = String::createWithFormat("%s", ConfigMgr::instance()->text("display_config.xml", "t5000"));
	}

	Application::getInstance()->openURL(BetLimit::GetStrWithoutSpace(kefuStr->getCString()));
}

void LoginLayer::getKeFuUrl(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	if(kefuStr) kefuStr->release();
	Integer* result = (Integer *)dic->objectForKey("result");
	kefuStr = (String* )dic->objectForKey("desc");	
	kefuStr->retain();
}

void LoginLayer::getCaiZhongList(Object *obj)
{
	Array* tempArr = (Array *)obj;	
	if(tempArr == nullptr) return;
	tempArr->retain();
	MyBetNumber::getInstance()->saveCaiZhong(tempArr);
	tempArr->release();
}