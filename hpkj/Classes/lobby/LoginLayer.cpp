#include "LoginLayer.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "Login.h"
#include "RoomLayer.h"
#include "AnsString.h"
#include "GBEvent.h"
#include "GBEventIDs.h"
#include "utf-8.h"
#include "MyNSString.h"
#include "ConfigMgr.h"
#include "GroupSprite.h"
#include "RegistLayer.h"
#include "ModifyPassWord.h"
#include "MovingLabelLayer.h"
#include "RegistLayer.h"
#include "RoomOption.h"
#include "PromptBox.h"
#include "FreeLayer.h"
#include "packet.h"
#include "DBHandler.h"
#include "SceneBackDialogLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

CCScene* LoginLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	LoginLayer *layer = LoginLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

LoginLayer::LoginLayer()
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginLayer::onAnsLoginFinish), MSG_UI_ANS_LOGINFINISH, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginLayer::onLoginFaild), MSG_UI_ANS_LOGINFAIL, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginLayer::onFindPassword), MSG_UI_ANS_FINDPASS, NULL);
}

LoginLayer::~LoginLayer()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_LOGINFINISH);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_FINDPASS);
}

void LoginLayer::setLogin(bool loginBool)
{
	m_bLogin = loginBool;
}

// on "init" you need to initialize your instance
bool LoginLayer::init(bool bLogin)
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	EntityMgr::instance();
	winSize = CCDirector::sharedDirector()->getWinSize();
	m_bLogin = true;
	m_from = false;

	auto rootNode=CSLoader::createNode("logon/Login.csb");
	rootNode->setScale(winSize.width/ rootNode->getContentSize().width, winSize.height/ rootNode->getContentSize().height);
	this->addChild(rootNode);

	//普通登录
	auto userlogon=reinterpret_cast<Button*>(rootNode->getChildByName("Button_1"));
	userlogon->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonEventWithUserLogin, this));

	//游客登录
	auto visitor=reinterpret_cast<Button*>(rootNode->getChildByName("Button_2"));
	visitor->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonEventWithVisitor, this));

	//第三方登录
	auto thirdPlatform=reinterpret_cast<Button*>(rootNode->getChildByName("Button_3"));
	thirdPlatform->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonEventWithThirdPlatformLogin, this));

	setKeypadEnabled(true);

	if (!bLogin)
	{
		return  true;
	}

	return true;
}

void LoginLayer::onEnter()
{
	CCLayer::onEnter();
}
void LoginLayer::onExit()
{
	CCLayer::onExit();
}
void LoginLayer::onLoginFaild(CCObject* obj)
{
	CCLOG("login failed");
	PromptBox* box = PromptBox::PromptBoxWith(CCPointMake(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLoginFaild);
	addChild(box,100);
}
void LoginLayer::setIsfaildLogin(bool mIsFaildLogin)
{

}
void LoginLayer::onRegist(cocos2d::CCObject *obj)
{
	playButtonSound();
	CCScene* scene = CCScene::create();
	RegistLayer * pRegisterLayer = RegistLayer::create();
	pRegisterLayer->setRegistLayerFrom(1);
	scene->addChild(pRegisterLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LoginLayer::onLogin(cocos2d::CCObject *obj)
{
	CCLOG("onLogin");
	playButtonSound();
	m_pDengluItemSprite->setEnabled(true);
	char* user = const_cast<char*>( userInput->getText());
	char* password = const_cast<char*>(passwordInput->getText());

	if(strlen(user) == 0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t36"),ccp(winSize.width * 0.5,winSize.height * 0.5));
		addChild(layer);
		return;
	}

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if (!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t26"),ccp(winSize.width * 0.5,winSize.height * 0.5));
		addChild(layer);
		return;
	}
	else
	{
		EntityMgr::instance()->login()->setUserLogin(user, password);
	}
}
void LoginLayer::GuestLogon(cocos2d::CCObject *obj)
{
	playButtonSound();
	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if (!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t26"),ccp(winSize.width * 0.5,winSize.height * 0.5));
		addChild(layer);
		return;
	}
	else
	{
		EntityMgr::instance()->login()->setUserRegister(createAccount().c_str(), createPassword().c_str(),0,1,0);
	}
}

void LoginLayer::setBoolFrom(bool b){
	m_from = b;
}

void LoginLayer::buttonEventWithUserLogin(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if(type==Widget::TouchEventType::ENDED){
        
        {
            auto layout = Layout::create();
            layout->setContentSize(winSize);
            layout->setTag(TAG_USERLOGIN_LAYER);
            this->addChild(layout);
            
            auto rootNode=CSLoader::createNode("logon/UserLogon.csb");
            rootNode->setPosition(winSize/2);
            layout->addChild(rootNode);
            
            //绑定登录回调
            Button* loginBtn = static_cast<Button*>(rootNode->getChildByName("logon_menu"));
            loginBtn->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonEventWithLogin, this));
            
            //绑定关闭按钮
            Button* closeBtn = static_cast<Button*>(rootNode->getChildByName("close_menu"));
            closeBtn->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonEventWithUserLoginClose, this));
            
            //绑定注册按钮
            Button* InsetBtn = static_cast<Button*>(rootNode->getChildByName("insert_menu"));
            //InsetBtn->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonEventWithRegist, this));
            
            //帐号EditBox
            auto account = static_cast<Sprite*>(rootNode->getChildByName("inputeidtbox_5"));
            m_pAccountEdit = ui::EditBox::create(cocos2d::Size(370, 58), "");
			m_pAccountEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
            m_pAccountEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
            m_pAccountEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
            m_pAccountEdit->setPosition(cocos2d::Point(account->getPositionX()-account->getContentSize().width/2, account->getPositionY()));
            m_pAccountEdit->setMaxLength(LEN_ACCOUNTS);
            m_pAccountEdit->setFontSize(26);
            m_pAccountEdit->setPlaceholderFontColor(Color3B::BLACK);
            m_pAccountEdit->setPlaceHolder(G2U("请输入您的游戏帐号"));
            m_pAccountEdit->setFontColor(Color3B::BLACK);
            rootNode->addChild(m_pAccountEdit);
            
            //密码EditBox
            auto password =static_cast<Sprite*>(rootNode->getChildByName("inputeidtbox_6"));
            m_pPasswordEdit = ui::EditBox::create(cocos2d::Size(370, 58), "");
            m_pPasswordEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
            m_pPasswordEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
            m_pPasswordEdit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
            m_pPasswordEdit->setPosition(cocos2d::Point(password->getPositionX()-password->getContentSize().width/2, password->getPositionY()));
            m_pPasswordEdit->setFontSize(26);
            m_pPasswordEdit->setPlaceholderFontColor(cocos2d::Color3B(214,246,255));
            m_pPasswordEdit->setPlaceHolder(G2U("请输入您的游戏密码"));
            m_pPasswordEdit->setFontColor(Color3B::BLACK);
            m_pPasswordEdit->setMaxLength(LEN_PASSWORD);
            rootNode->addChild(m_pPasswordEdit);
        }
        
    }
}

void LoginLayer::buttonEventWithUserLoginClose(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    auto node=reinterpret_cast<Node*>(this->getChildByTag(TAG_USERLOGIN_LAYER));
    node->removeFromParent();
}

//登录
void LoginLayer::buttonEventWithLogin(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
     if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
     {     
         std::string account = std::string(m_pAccountEdit->getText());
         std::string password = std::string(m_pPasswordEdit->getText());
 
		 bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
		 if (!bRet)
		 {
			 MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t26"),ccp(winSize.width * 0.5,winSize.height * 0.5));
			 addChild(layer);
			 return;
		 }
		 else
		 {
			 EntityMgr::instance()->login()->setUserLogin(account, password);
		 }
         
     }
}

//游客登录
void LoginLayer::buttonEventWithVisitor(Ref* target,cocos2d::ui::Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    { 
        	CCNode *pNode=getChildByTag(331);
			if(pNode)
			{
				pNode->setVisible(true);
			}
			else
			{
				auto pLayer = SceneBackDialogLayer::create(1.0f);
				pLayer->setPosition((SCREEN_WIDTH-pLayer->getContentSize().width)/2, (SCREEN_HEIGHT-pLayer->getContentSize().height)/2);
				pLayer->setTag(331);
				this->addChild(pLayer);
			}
    }
}

//MARK::按钮回调
void LoginLayer::buttonEventWithThirdPlatformLogin(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    
    //vx login
    
    //ios login
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    
#endif
    //android login
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    
#endif
    
    return;
    
    if (type == Widget::TouchEventType::ENDED)
    {
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
        UMSocialSnsPlatform *snsplatform = [UMSocialSnsPlatformManager getSocialPlatformWithName:UMShareToSina];
        snsplatform.loginClickHandler(pApp.viewController, [UMSocialControllerService defaultControllerService], YES, ^(UMSocialResponseEntity *response){
            [[UMSocialDataService defaultDataService] requestSocialAccountWithCompletion:^(UMSocialResponseEntity *response) {
                 if (response.responseCode == UMSResponseCodeSuccess) {
                 NSMutableDictionary *ptest = [[response.data objectForKey:@"accounts"] objectForKey:UMShareToSina ];
                 if (!ptest)
                 {
                    return;
                 }
                 //头像url
                HallDataMgr::getInstance()->m_MethodHeadUrl = [[ptest objectForKey:@"icon"] UTF8String];
                 //用户昵称
                HallDataMgr::getInstance()->m_pNickName = [[ptest objectForKey:@"username"] UTF8String];
                HallDataMgr::getInstance()->m_cbGender = [[ptest objectForKey:@"gender"] intValue];
                 
                 //设置用户信息
                 HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
                 HallDataMgr::getInstance()->m_pAccounts = [[ptest objectForKey:@"usid"] UTF8String];
                 //性别  女0 男1
                 HallDataMgr::getInstance()->m_loadtype = Load_Sina;
                 this->loadLoading();    
                 NetworkMgr::getInstance()->sendMethodLogin(PLATFORM_Sina);
                     
         
                 }
                 else
                 {
                    log("%s_%d", "第三方登录失败", response.responseCode);
                 }
             }];
        });
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        DebugLog("第三方登录");
        JniMethodInfo minfo;
        jobject jobj;
        bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity","sinalogin","()V");
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
            minfo.env->DeleteLocalRef(minfo.classID);
        }
        DebugLog("第三方登录");
#endif
     }
}

void LoginLayer::back(cocos2d::CCObject *obj)
{
	playButtonSound();
	if(m_bLogin)
	{
		if(!m_from)
		{
			RoomOption* option = RoomOption::create();
			CCScene* pScene = CCScene::create();
			pScene->addChild(option);
			CCDirector::sharedDirector()->replaceScene(pScene);
		}else{
			RoomLayer *pRoomLayer = RoomLayer::create();
			pRoomLayer->automaticLogin(false);
			CCScene* pScene = CCScene::create();
			pScene->addChild(pRoomLayer);
			CCDirector::sharedDirector()->replaceScene(pScene);

		}
	}
	else
	{
		CCDirector::sharedDirector()->end();
	}
}

void LoginLayer::onAnsLoginFinish(CCObject* obj)
{
	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LoginLayer::onFindPassword(CCObject* obj)
{
	playButtonSound();
	tagGetPassObj* pObj = (tagGetPassObj*)obj;
	CCLOG((const char*)pObj->szErrorDescribe);
	if(pObj->lErrCode == 0)  //成功
	{
		PromptBox* box = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptTypePasswordFind);
		box->setPromptText(CCString::createWithFormat("%s%s",ConfigMgr::instance()->text("t164"),pObj->szPassword)->getCString());
		addChild(box,30);
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("NEWZJD_PASSWORD",pObj->szPassword);
		passwordInput->cleanup();
		passwordInput->setText(pObj->szPassword);

	}else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(CCString::createWithFormat("%s",(const char*)pObj->szErrorDescribe)->getCString(),ccp(winSize.width * 0.5,winSize.height * 0.5));
		addChild(layer);
	}

	userInput->setTouchEnabled(true);
	passwordInput->setTouchEnabled(true);
	m_pLoginMenu->setTouchEnabled(true);
	m_pRegisterMenu->setTouchEnabled(true);
}

void LoginLayer::changePassWord(CCObject* obj)
{
	playButtonSound();
	if(m_bLogin)
	{
		CCDirector::sharedDirector()->replaceScene(ModifyPassWord::scene());
	}
	else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t455"),ccp(427,240));
		addChild(layer);
	}

}
void LoginLayer::toFreeLayer(CCObject* obj){
	playButtonSound();
	if(m_bLogin)
	{
		CCDirector::sharedDirector()->replaceScene(FreeLayer::scene());
	}
}

string LoginLayer::createAccount()
{
	string szAccount;
	int t = 0;
	srand((unsigned)time(NULL));
	for (int i = 0; i < 4; i++)
	{
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

string LoginLayer::createPassword()
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
