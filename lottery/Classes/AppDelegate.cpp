#include "cocos2d.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "TableViewLayer.h"
#include "LoadingLayer.h"
#include "MyNSString.h"
#include "SceneControl.h"
#include "DBHandler.h"
#include "SoundControl.h"
#include "MyConfig.h"//by hxh
//#include "C2DXShareSDK.h"
using namespace CocosDenshion;
//using namespace cn::sharesdk;
#include "MovingLabelLayer.h"
#include "VersionControl.h"

#ifdef USE_DDZ
	#include "DDZTimeCheckControl.h"
#endif

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::end();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("QiXing", Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
		//需要显示的大小必须放到designresolution之前
		glview->setFrameSize(480,720); 
#else
		glview = GLViewImpl::create("QiXing");
#endif
		director->setOpenGLView(glview);
	}

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	
	// Set the design resolution	
	glview->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, ResolutionPolicy::EXACT_FIT);	
    //��ʼ��ShareSDK
    //C2DXShareSDK::open(String::create("5b021ac0b64a"), false);
    
    //��ʼ���罻ƽ̨��Ϣ
    //this -> initPlatformConfig();

	
	// turn on display FPS
#if defined(WIN32)						// windows 平台
	//director->setDisplayStats(true);
#endif

	MyNSString::doSomeThing();

	FileUtils::getInstance()->addSearchPath("res/");
    FileUtils::getInstance()->addSearchPath("RoomList/");
    FileUtils::getInstance()->addSearchPath("LotteryImage/");
    FileUtils::getInstance()->addSearchPath("background/");
    FileUtils::getInstance()->addSearchPath("hlddz_particle/");
    FileUtils::getInstance()->addSearchPath("music/");
    FileUtils::getInstance()->addSearchPath("soundeffect/");
    FileUtils::getInstance()->addSearchPath("xml/");
    FileUtils::getInstance()->addSearchPath("xyx_music/");
    FileUtils::getInstance()->addSearchPath("ShopImage/");
	FileUtils::getInstance()->addSearchPath("common/");

	MyConfig& myConfig=MyConfig::Instance();
	int platform=(myConfig.m_platform>=0 && myConfig.m_platform<MAX_PLATFORM_NUM?myConfig.m_platform:0);
	log("platform=%d",platform);

#ifdef USE_BJL
	//baijiale
	string bjlpath[2]={"",""};
	bjlpath[1]=(platform==0?"bjl":"bjl1");
	bjlpath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][2];
	for(int i=0;i<2;i++)
	{
		string path=bjlpath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/background/");
		FileUtils::getInstance()->addSearchPath(path+"/BJL/");
		FileUtils::getInstance()->addSearchPath(path+"/hlddz_particle/");
		FileUtils::getInstance()->addSearchPath(path+"/ui_card/");
		FileUtils::getInstance()->addSearchPath(path+"/xml/");
		FileUtils::getInstance()->addSearchPath(path+"/xyx_music/");
		FileUtils::getInstance()->addSearchPath(path+"/soundeffect/");
	}
#endif

#ifdef USE_DZPK
	//dzpk
	string dzpkpath[2]={"",""};
	dzpkpath[1]=(platform==0?"dzpk":"dzpk1");
	dzpkpath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][0];
	for(int i=0;i<2;i++)
	{
		string path=dzpkpath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/plist/");
		FileUtils::getInstance()->addSearchPath(path+"/soundeffect/");
		FileUtils::getInstance()->addSearchPath(path+"/xml/");
	}
#endif

#ifdef USE_DDZ
	//ddz
	string ddzpath[2]={"",""};
	ddzpath[1]=(platform==0?"ddz":"ddz1");
	ddzpath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][1];
	for(int i=0;i<2;i++)
	{
		string path=ddzpath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/background/");
		FileUtils::getInstance()->addSearchPath(path+"/fonts/");
		FileUtils::getInstance()->addSearchPath(path+"/hlddz_particle/");
		FileUtils::getInstance()->addSearchPath(path+"/music/");
		FileUtils::getInstance()->addSearchPath(path+"/xml/");
		FileUtils::getInstance()->addSearchPath(path+"/soundeffect/");
		FileUtils::getInstance()->addSearchPath(path+"/soundeffect/man/");
		FileUtils::getInstance()->addSearchPath(path+"/soundeffect/woman/");
	}
#endif

#ifdef USE_PPC
	string ppcpath[2]={"",""};
	ppcpath[1]=(platform==0?"ppc":"ppc1");
	ppcpath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][4];
	for(int i=0;i<2;i++)
	{
		string path=ppcpath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/plist/");
		FileUtils::getInstance()->addSearchPath(path+"/fonts/");
	    FileUtils::getInstance()->addSearchPath(path+"/bg/");
		FileUtils::getInstance()->addSearchPath(path+"/xml/");
		FileUtils::getInstance()->addSearchPath(path+"/dialog/");
	    FileUtils::getInstance()->addSearchPath(path+"/sounds/bgm/");
		FileUtils::getInstance()->addSearchPath(path+"/sounds/effect/");
	}
#endif // USE_PPC

#ifdef USE_BRNN
	string brnnpath[2]={"",""};
	brnnpath[1]=(platform==0?"brnn":"brnn1");
	brnnpath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][5];
	for(int i=0;i<2;i++)
	{
		string path=brnnpath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/plist/");
		FileUtils::getInstance()->addSearchPath(path+"/fonts/");
		FileUtils::getInstance()->addSearchPath(path+"/bg/");
		FileUtils::getInstance()->addSearchPath(path+"/xml/");
		FileUtils::getInstance()->addSearchPath(path+"/sounds/");
	}
#endif // USE_BRNN

#ifdef USE_SGLY
	string sglypath[2]={"",""};
	sglypath[1]=(platform==0?"sgly":"sgly1");
	sglypath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][6];
	for(int i=0;i<2;i++)
	{
		string path=sglypath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/plist/");
		FileUtils::getInstance()->addSearchPath(path+"/bg/");
		FileUtils::getInstance()->addSearchPath(path+"/xml/");
		FileUtils::getInstance()->addSearchPath(path+"/sound/");
	}

#endif // USE_SGLY


#ifdef USE_FISHBEAT
	string fishpath[2]={"",""};
	fishpath[1]=(platform==0?"fishbeat":"fishbeat1");
	fishpath[0]=FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][3];
	for(int i=0;i<2;i++)
	{
		string path=fishpath[i];
		FileUtils::getInstance()->addSearchPath(path+"/");
		FileUtils::getInstance()->addSearchPath(path+"/fishbeat/");
	}	
#endif

	//EntityMgr::instance()->createActor();
    bool hasMusic = UserDefault::getInstance()->getBoolForKey("hasMusic", false);
    if(hasMusic == true){
       //SoundControl::sharedSoundControl()->playBackGroundMusic("GAME_PLACKGROUND.mp3", true);
    }

	DBHandler *handler = new DBHandler();
	handler->execute(
		"create table if not exists UserInfo(dwUserID integer primary key, wFaceID integer, cbGender integer, cbMemberOrder integer, lScore integer, dwWinCount integer, dwLostCount integer, dwDrawCount integer, dwFleeCount integer, dwExperience integer, nickname TEXT)");
	handler->execute(
		"create table if not exists FriendInfo(dwUserID integer primary key, wFaceID integer, cbGender integer, cbMemberOrder integer, lScore integer, dwWinCount integer, dwLostCount integer, dwDrawCount integer, dwFleeCount integer, dwExperience integer, nickname TEXT)");
	handler->execute("create table if not exists FriendStatus(dwUserID integer primary key, status TEXT)");
	handler->execute("create table if not exists PersonalMsg(msg TEXT)");
	handler->execute("create table if not exists SystemMsg(msg TEXT)");
	handler->execute("create table if not exists FriendMsg(msg TEXT)");
	handler->execute(
		"create table if not exists TouZhuXXInfo(ID integer primary key autoincrement, time TEXT, dwUserID integer, gameType integer, gameKind integer, qiShu TEXT, betNumber TEXT, zhuShu integer, beiTou integer, moshi integer, sign integer, isEnd integer, zhuiHao integer, nIndex integer)");

	initGLContextAttrs();
	//开始等待消息
	EntityMgr::instance()->startSchedule();

	Scene *pScene = LoadingLayer::scene();
	director->runWithScene(pScene);
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	//if (ConfigMgr::instance()->m_Config.m_isGameRun==true)
	//{
	//	Director::getInstance()->replaceScene(SceneControl::sharedSceneControl()->getScene(1,false));
	//}
	Director::getInstance()->stopAnimation();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#ifdef USE_DDZ
	DDZTimeCheckControl::getInstance()->saveEnterBackgroundTime();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
 Director::getInstance()->startAnimation();

	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();	
#ifdef USE_DDZ
	DDZTimeCheckControl::getInstance()->saveEnterForegroundTime();
#endif

	//static bool isFirst = true;
	//if(!isFirst)
	//{	
	//	//重连
	//	EntityMgr::instance()->getDispatch()->SendExitGame();
	//	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith("re connet",Vec2(427,240));
	//	Director::getInstance()->getRunningScene()->addChild(layer,50);
	//}
	//isFirst = false;

	//EntityMgr::instance()->getDispatch()->SendReConnect();
}

void AppDelegate::initPlatformConfig()
{/******************************************
    //����΢��
    Dictionary *sinaConfigDict = Dictionary::create();
    sinaConfigDict -> setObject(String::create("568898243"), "app_key");
    sinaConfigDict -> setObject(String::create("38a4f8204cc784f81f9f0daaf31e02e3"), "app_secret");
    sinaConfigDict -> setObject(String::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    //��Ѷ΢��
    Dictionary *tcConfigDict = Dictionary::create();
    tcConfigDict -> setObject(String::create("801307650"), "app_key");
    tcConfigDict -> setObject(String::create("ae36f4ee3946e1cbb98d6965b0b2ff5c"), "app_secret");
    tcConfigDict -> setObject(String::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTencentWeibo, tcConfigDict);
    
    //����
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
    
    //QQ�ռ�
    Dictionary *qzConfigDict = Dictionary::create();
    qzConfigDict -> setObject(String::create("100371282"), "app_id");
    qzConfigDict -> setObject(String::create("aed9b0303e3ed1e27bae87c33761161d"), "app_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //΢��
    Dictionary *wcConfigDict = Dictionary::create();
    wcConfigDict -> setObject(String::create("wx4868b35061f87885"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
    
    //QQ
    Dictionary *qqConfigDict = Dictionary::create();
    qqConfigDict -> setObject(String::create("100371282"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
    
    //Facebook
    Dictionary *fbConfigDict = Dictionary::create();
    fbConfigDict -> setObject(String::create("107704292745179"), "api_key");
    fbConfigDict -> setObject(String::create("38053202e1a5fe26c80c753071f0b573"), "app_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFacebook, fbConfigDict);
    
    //Twitter
    Dictionary *twConfigDict = Dictionary::create();
    twConfigDict -> setObject(String::create("mnTGqtXk0TYMXYTN7qUxg"), "consumer_key");
    twConfigDict -> setObject(String::create("ROkFqr8c3m1HXqS3rm3TJ0WkAJuwBOSaWhPbZ9Ojuc"), "consumer_secret");
    twConfigDict -> setObject(String::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTwitter, twConfigDict);
    
    //Google+
    Dictionary *gpConfigDict = Dictionary::create();
    gpConfigDict -> setObject(String::create("232554794995.apps.googleusercontent.com"), "client_id");
    gpConfigDict -> setObject(String::create("PEdFgtrMw97aCvf0joQj7EMk"), "client_secret");
    gpConfigDict -> setObject(String::create("http://localhost"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeGooglePlus, gpConfigDict);
    
    //������
    Dictionary *rrConfigDict = Dictionary::create();
    rrConfigDict -> setObject(String::create("226427"), "app_id");
    rrConfigDict -> setObject(String::create("fc5b8aed373c4c27a05b712acba0f8c3"), "app_key");
    rrConfigDict -> setObject(String::create("f29df781abdd4f49beca5a2194676ca4"), "secret_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeRenren, rrConfigDict);
    
    //������
    Dictionary *kxConfigDict = Dictionary::create();
    kxConfigDict -> setObject(String::create("358443394194887cee81ff5890870c7c"), "api_key");
    kxConfigDict -> setObject(String::create("da32179d859c016169f66d90b6db2a23"), "secret_key");
    kxConfigDict -> setObject(String::create("http://www.sharesdk.cn/"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeKaixin, kxConfigDict);
    
    //�ʼ�
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeMail, NULL);
    
    //��ӡ
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeAirPrint, NULL);
    
    //����
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeCopy, NULL);
    
    //�Ѻ�΢��
    Dictionary *shwbConfigDict = Dictionary::create();
    shwbConfigDict -> setObject(String::create("SAfmTG1blxZY3HztESWx"), "consumer_key");
    shwbConfigDict -> setObject(String::create("yfTZf)!rVwh*3dqQuVJVsUL37!F)!yS9S!Orcsij"), "consumer_secret");
    shwbConfigDict -> setObject(String::create("http://www.sharesdk.cn"), "callback_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSohuWeibo, shwbConfigDict);
    
    //����΢��
    Dictionary *neConfigDict = Dictionary::create();
    neConfigDict -> setObject(String::create("T5EI7BXe13vfyDuy"), "consumer_key");
    neConfigDict -> setObject(String::create("gZxwyNOvjFYpxwwlnuizHRRtBRZ2lV1j"), "consumer_secret");
    neConfigDict -> setObject(String::create("http://www.shareSDK.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatType163Weibo, neConfigDict);
    
    //����
    Dictionary *dbConfigDict = Dictionary::create();
    dbConfigDict -> setObject(String::create("02e2cbe5ca06de5908a863b15e149b0b"), "api_key");
    dbConfigDict -> setObject(String::create("9f1e7b4f71304f2f"), "secret");
    dbConfigDict -> setObject(String::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeDouBan, dbConfigDict);
    

	Dictionary *enConfigDict = Dictionary::create();
    enConfigDict -> setObject(String::create("sharesdk-7807"), "consumer_key");
    enConfigDict -> setObject(String::create("d05bf86993836004"), "consumer_secret");
    enConfigDict -> setObject(String::create("0"), "host_type");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeEvernote, enConfigDict);
    
    //LinkedIn
    Dictionary *liConfigDict = Dictionary::create();
    liConfigDict -> setObject(String::create("ejo5ibkye3vo"), "api_key");
    liConfigDict -> setObject(String::create("cC7B2jpxITqPLZ5M"), "secret_key");
    liConfigDict -> setObject(String::create("http://sharesdk.cn"), "redirect_url");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeLinkedIn, liConfigDict);
    
    //Pinterest
    Dictionary *piConfigDict = Dictionary::create();
    piConfigDict -> setObject(String::create("1432928"), "client_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypePinterest, piConfigDict);
    
    //Pocket
    Dictionary *poConfigDict = Dictionary::create();
    poConfigDict -> setObject(String::create("11496-de7c8c5eb25b2c9fcdc2b627"), "consumer_key");
    poConfigDict -> setObject(String::create("pocketapp1234"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypePocket, poConfigDict);
    
    //Instapaper
    Dictionary *ipConfigDict = Dictionary::create();
    ipConfigDict -> setObject(String::create("4rDJORmcOcSAZL1YpqGHRI605xUvrLbOhkJ07yO0wWrYrc61FA"), "consumer_key");
    ipConfigDict -> setObject(String::create("GNr1GespOQbrm8nvd7rlUsyRQsIo3boIbMguAl9gfpdL0aKZWe"), "consumer_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeInstapaper, ipConfigDict);
    
    //�е��Ʊʼ�
    Dictionary *ydConfigDict = Dictionary::create();
    ydConfigDict -> setObject(String::create("dcde25dca105bcc36884ed4534dab940"), "consumer_key");
    ydConfigDict -> setObject(String::create("d98217b4020e7f1874263795f44838fe"), "consumer_secret");
    ydConfigDict -> setObject(String::create("http://www.sharesdk.cn/"), "oauth_callback");
    ydConfigDict -> setObject(String::create("1"), "host_type");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeYouDaoNote, ydConfigDict);
    

	Dictionary *shkConfigDict = Dictionary::create(); 
    shkConfigDict -> setObject(String::create("e16680a815134504b746c86e08a19db0"), "app_key");
    shkConfigDict -> setObject(String::create("b8eec53707c3976efc91614dd16ef81c"), "app_secret");
    shkConfigDict -> setObject(String::create("http://sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSohuKan, shkConfigDict);
    
    //Flickr
    Dictionary *flickrConfigDict = Dictionary::create();
    flickrConfigDict -> setObject(String::create("33d833ee6b6fca49943363282dd313dd"), "api_key");
    flickrConfigDict -> setObject(String::create("3a2c5b42a8fbb8bb"), "api_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFlickr, flickrConfigDict);
    
    //Tumblr
    Dictionary *tumblrConfigDict = Dictionary::create();
    tumblrConfigDict -> setObject(String::create("2QUXqO9fcgGdtGG1FcvML6ZunIQzAEL8xY6hIaxdJnDti2DYwM"), "consumer_key");
    tumblrConfigDict -> setObject(String::create("3Rt0sPFj7u2g39mEVB3IBpOzKnM3JnTtxX2bao2JKk4VV1gtNo"), "consumer_secret");
    tumblrConfigDict -> setObject(String::create("http://sharesdk.cn"), "callback_url");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTumblr, tumblrConfigDict);
    
    //Dropbox
    Dictionary *dropboxConfigDict = Dictionary::create();
    dropboxConfigDict -> setObject(String::create("7janx53ilz11gbs"), "app_key");
    dropboxConfigDict -> setObject(String::create("c1hpx5fz6tzkm32"), "app_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeDropbox, dropboxConfigDict);
    
    //Instagram
    Dictionary *instagramConfigDict = Dictionary::create();
    instagramConfigDict -> setObject(String::create("ff68e3216b4f4f989121aa1c2962d058"), "client_id");
    instagramConfigDict -> setObject(String::create("1b2e82f110264869b3505c3fe34e31a1"), "client_secret");
    instagramConfigDict -> setObject(String::create("http://sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeInstagram, instagramConfigDict);
    
    //VK
    Dictionary *vkConfigDict = Dictionary::create();
    vkConfigDict -> setObject(String::create("3921561"), "application_id");
    vkConfigDict -> setObject(String::create("6Qf883ukLDyz4OBepYF1"), "secret_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeVKontakte, vkConfigDict);
	******************************************/
}