//
//  roomLayer.cpp
//  Game
//
//  Created by zhouwei on 13-6-17.
//
//

#include "roomLayer.h"
#include "GBEventIDs.h"
#include "tableViewLayer.h"
#include "ConfigMgr.h"
#include "utf-8.h"
#include "GBEvent.h"
#include "PlayerInfoLayer.h"
#include "RoomOption.h"
#include "EmailLayer.h"
#include "ActivityLayer.h"
#include "ExitLayer.h"
#include "CustomerServiceLayer.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "SceneControl.h"
#include "BJLRes.h"
#include "GeTCHARge.h"
#include "GroupSprite.h"
#include "TaskInfo.h"
#include "RuningBar.h"
#include "CShaderSprite.h"
#include "FreeLayer.h"
#include "packet.h"
#include "CMD_GameServer.h"
#include "FreeCoins.h"
#include "MissionLayer.h"
#include "FriendSystemLayer.h"
#include "HelpLayer.h"
#include "ShopSystemLayer.h"
#include "MissionLayer.h"
#include "ExchangerLayer.h"
#include "LuckyDraw.h"
#include "LoginLayer.h"
#include "VIPSystemLayer.h"
#include "RankSystemLayer.h"
#include "BJLSceneControl.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

using namespace cocos2d;
using namespace cocos2d::ui;

CCScene* RoomLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	CCLayer *layer = RoomLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

RoomLayer::RoomLayer()
{
	pWarningLayer=NULL;
	isGoingRoomLayer = 0;
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onUpdateScoreAndGoldEgg),MSG_UI_ANS_UPDATESCOREANDGOLDEGG , NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::showDailyGetCoins),MSG_UI_ANS_CANNELRECHANGER , NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(RoomLayer::showDailyGetCoinsResult),MSG_UI_ANS_GETSAVECOINS,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onlandcardResult),MSG_UI_ANS_LANDCARDRESULT , NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onPhoneCardResult),MSG_UI_ANS_PHONECARDRESULT , NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onRefshTotalNum),MSG_UI_ANS_REFRESHTOTALNUM, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onChange),MSG_UI_ANS_CHARGE, NULL);
	CCNotificationCenter::sharedNotificationCenter()->postNotification("CancelHttpRequest");
}

RoomLayer::~RoomLayer()
{
	GBEVENT_NOARG_UNREGISTER(GBEVENT_UI_LOGINFINISH, RoomLayer::onLoginFinish);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_UI_ANS_REGNFAIL);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_UI_ANS_CANNELRECHANGER);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_UI_ANS_GETSAVECOINS);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_UPDATESCOREANDGOLDEGG);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_LANDCARDRESULT);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_PHONECARDRESULT);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_CHARGE);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_REFRESHTOTALNUM);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_GAMELINK);
}


void RoomLayer::automaticLogin(bool automatic,bool overResgist /* = false */)
{
	if(automatic)
	{

		bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
		if(bRet)
		{
			if(EntityMgr::instance()->login()->getAccout().length() > 0 && !overResgist)
			{
				onLoginFinish();
			}
			else
			{
				std::string userAccount = CCUserDefault::sharedUserDefault()->getStringForKey("NEWZJD_ACCOUNT");
				std::string userPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NEWZJD_PASSWORD");
				userAccount = DEFAULT_ACCOUNT;
				userPassword = DEFAULT_PASSWORD;
				if(userAccount.length() > 1 &&userPassword.length() > 1)
				{
					CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoomLayer::onLoginFaild), MSG_UI_ANS_LOGINFAIL, NULL);
					GBEVENT_NOARG_REGISTER(GBEVENT_UI_LOGINFINISH, RoomLayer::onLoginFinish);

					bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
					if(!bRet)
					{
						MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t26"),ccp(winSize.width * 0.5,winSize.height * 0.5));
						addChild(layer,100);
					}
					else
					{
						EntityMgr::instance()->login()->setUserLogin(userAccount.c_str(), userPassword);
					}	
				}
				else
				{
					CCNotificationCenter::sharedNotificationCenter()->addObserver(
						this, callfuncO_selector(RoomLayer::onRegisterFaild), MSG_UI_ANS_REGNFAIL, NULL);

					CCNotificationCenter::sharedNotificationCenter()->addObserver(
						this, callfuncO_selector(RoomLayer::onRegistSucc), MSG_UI_ANS_REGSUCC, NULL);
					//没有账号 注册游客账号
					int nGender = 0;
					int nFaceId = 0;
					string szAccount = createAccount();
					string szPassword = createPassword();

					bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
					if (!bRet)
					{
						PromptBox* box = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
						addChild(box,100);
					}
					EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender,1);

				}
			}
		}
		else
		{
			PromptBox* box = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
			addChild(box,100);
		}
	}
	else
	{
		string szText0;
		if(strlen(g_GlobalUnits.getGolbalUserData()->szNickName)>0)
		{
			CCLOG("have nick name");
			szText0 = g_GlobalUnits.getGolbalUserData()->szNickName;
		}
		else
		{
			CCLOG("not have nick name");
			szText0 = EntityMgr::instance()->login()->getAccout();
		}
		long scoreNumber = g_GlobalUnits.getGolbalUserData()->lScore;
		string szText1 = CCString::createWithFormat("%d",scoreNumber)->getCString();//addCommaToNumber(scoreNumber);
		string szText2 = CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString();//addCommaToNumber(telephonePointNumber);
		szText0 = ConfigMgr::instance()->text("t1002")+szText0;
		szText1 = ConfigMgr::instance()->text("t1003")+szText1;
		szText2 = ConfigMgr::instance()->text("t1004")+szText2;

		CCLabelTTF* userNmae;
		if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
		{
			userNmae = CCLabelTTF::create(szText0.c_str(),"",20);
		}
		else
		{
			userNmae = CCLabelTTF::create(CCString::createWithFormat("%s%s%s", ConfigMgr::instance()->text("t1002"),ConfigMgr::instance()->text("t161"),szText0.c_str())->getCString(),"",20);
		}
		m_pUserScore = CCLabelTTF::create(szText1.c_str(),"",20);
		m_pUserPhonePoint = CCLabelTTF::create(szText2.c_str(),"",20);
		userNmae->setColor(cocos2d::Color3B::WHITE);
		m_pUserScore->setColor(cocos2d::Color3B::YELLOW);
		m_pUserPhonePoint->setColor(cocos2d::Color3B::WHITE);
		userNmae->setPosition(ccp(winSize.width * 0.15f,winSize.height * 0.965f));
		m_pUserScore->setPosition(ccp(winSize.width * 0.15f,winSize.height * 0.9f));
		m_pUserPhonePoint->setPosition(ccp(winSize.width * 0.28f,winSize.height * 0.965f));
		userNmae->setAnchorPoint(ccp(0.0f,0.5f));
		m_pUserScore->setAnchorPoint(ccp(0.0f,0.5f));
		m_pUserPhonePoint->setAnchorPoint(ccp(0.0f,0.5f));
		addChild(userNmae);
		addChild(m_pUserScore);
		addChild(m_pUserPhonePoint,0,301);

		CCSprite *pPlayerNormalSprite;
		CCSprite *pPlayerSelectSprite;
		if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
		{
			pPlayerNormalSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_10.png"));
			pPlayerSelectSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_10.png"));
		}
		else
		{
			pPlayerNormalSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_8.png"));
			pPlayerSelectSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_8.png"));
		}

		return;
	}
}

void RoomLayer::onLoginFinish()
{
	GBEVENT_NOARG_UNREGISTER(GBEVENT_UI_LOGINFINISH, RoomLayer::onLoginFinish);
	//登陆完成
	string szText0;
	if(strlen(g_GlobalUnits.getGolbalUserData()->szNickName)>0)
	{
		CCLOG("have nick name");
		szText0 = g_GlobalUnits.getGolbalUserData()->szNickName;
	}
	else
	{
		CCLOG("not have nick name");
		szText0 = EntityMgr::instance()->login()->getAccout();
	}
	long scoreNumber = g_GlobalUnits.getGolbalUserData()->lScore;
	string szText1 = CCString::createWithFormat("%d",scoreNumber)->getCString();//addCommaToNumber(scoreNumber);
	long telephonePointNumber = g_GlobalUnits.getGolbalUserData()->lGoldEggs;
	string szText2 = CCString::createWithFormat("%d",telephonePointNumber)->getCString();//addCommaToNumber(telephonePointNumber);

	CCLabelTTF* userNmae;
	if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
	{
		userNmae = CCLabelTTF::create(szText0.c_str(),"",20);
	}
	else
	{
		userNmae = CCLabelTTF::create(CCString::createWithFormat("%s%s",ConfigMgr::instance()->text("t161"),szText0.c_str())->getCString(),"",20);
	}

	m_pUserScore = CCLabelTTF::create(szText1.c_str(),"",17);
	m_pUserPhonePoint = CCLabelTTF::create(szText2.c_str(),"",17);
	userNmae->setColor(ccc3(0,0,0));
	m_pUserScore->setColor(ccc3(0,0,0));
	m_pUserPhonePoint->setColor(ccc3(0,0,0));
	userNmae->setPosition(ccp(winSize.width * 0.1f,winSize.height * 0.948f));
	m_pUserScore->setPosition(ccp(winSize.width * 0.265f,winSize.height * 0.925f));
	m_pUserPhonePoint->setPosition(ccp(winSize.width * 0.265f,winSize.height * 0.875f));
	userNmae->setAnchorPoint(ccp(0.0f,0.0f));
	m_pUserScore->setAnchorPoint(ccp(1.0f,0.5f));
	m_pUserPhonePoint->setAnchorPoint(ccp(1.0f,0.5f));
	addChild(userNmae);
	addChild(m_pUserScore);
	addChild(m_pUserPhonePoint,0,301);

	CCSprite *pPlayerNormalSprite;
	CCSprite *pPlayerSelectSprite;
	if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
	{
		pPlayerNormalSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_10.png"));
		pPlayerSelectSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_10.png"));
	}
	else
	{
		pPlayerNormalSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_8.png"));
		pPlayerSelectSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_8.png"));
	}

	pPlayerMenuSprite->setNormalImage(pPlayerNormalSprite);
	pPlayerMenuSprite->setSelectedImage(pPlayerSelectSprite);
	showLoginAward();
}

void RoomLayer::onUpdateScoreAndGoldEgg(CCObject* obj)
{
	long scoreNumber = g_GlobalUnits.getGolbalUserData()->lScore;
	string szText1 = CCString::createWithFormat("%d",scoreNumber)->getCString();//addCommaToNumber(scoreNumber);
	long telephonePointNumber = g_GlobalUnits.getGolbalUserData()->lGoldEggs;
	string szText2 = CCString::createWithFormat("%d",telephonePointNumber)->getCString();//addCommaToNumber(telephonePointNumber);
	m_pUserScore->setString(szText1.c_str());
	m_pUserPhonePoint->setString(szText2.c_str());
}

void RoomLayer::onLoginFaild(CCObject* obj)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
	CCLOG("login faied");

	PromptBox* box = PromptBox::PromptBoxWith(CCPointMake(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLoginFaild);
	addChild(box,100);
}

void RoomLayer::onEnter()
{
	if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		playBGM("music/roomback.mp3");
	}
	BaseLayer::onEnter();
}

void RoomLayer::onExit()
{
	BaseLayer::onExit();
}

bool RoomLayer::init()
{
	// 1. super init first
	if ( !BaseLayer::init() )
	{
		return false;
	}
	roomLevel = ROOM_LAYER_LEVEL_HOBBY;
	mMenuCloseOrOpen = false;
	isGoingRoomLayer =0;

	//背景
	winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto rootNode = CSLoader::createNode("PlazzScene_Src/Plazz_Layer.csb");
	rootNode->setScale(winSize.width/ rootNode->getContentSize().width, winSize.height/ rootNode->getContentSize().height);
    this->addChild(rootNode);

	m_rootNode=rootNode;

	//用户昵称
    auto nicknode=reinterpret_cast<Node*>(rootNode->getChildByName("username"));
    nicknode->removeFromParent();

    //用户分数
    auto scorenode=reinterpret_cast<Node*>(rootNode->getChildByName("usercoincount"));
    scorenode->removeFromParent();

    Button *headBtn = static_cast<Button *>(rootNode->getChildByName("btn_heard"));
    if(nullptr != headBtn)
    {       
        headBtn->addTouchEventListener(CC_CALLBACK_2(RoomLayer::buttonEventWithTouchUser, this));
    }

	//添加游戏列表
    addGameList(rootNode);

	setKeypadEnabled(true);

	return  true;
}

void RoomLayer::addGameList(Node * node)
{
    //游戏列表
    _list = ListView::create();
    _list->setBounceEnabled(true);
    _list->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _list->setContentSize(Size(1136, 376));
    _list->setAnchorPoint(Vec2(.5, .5));
    _list->setPosition(Vec2(568, 345));
    _list->setItemsMargin(25.0f);
    node->addChild(_list);
    
	int kindids[]={122,715};
    int nGameCount = sizeof(kindids)/sizeof(kindids[0]);
    for (int i = 0; i < nGameCount; ++i)
    {
        Button *list = (Button*)(this->initOneListByKind(kindids[i]));
        
        if (nullptr == list)
        {
            continue;
        }
        int eTag = (int)list->getTag();
        
        list->addTouchEventListener([=](Ref *ref,cocos2d::ui::Widget::TouchEventType type)
                                    {
                                        if (cocos2d::ui::Widget::TouchEventType::ENDED == type)
                                        {
                                            //不检查连环夺宝资源
                                            if(eTag==715||eTag==122 && 1){
                                                //加载游戏
                                                this->loadingGame(eTag);
                                                return;
                                            }
                                        }
                                    });
        _list->pushBackCustomItem(list);
    }
    
}

Widget* RoomLayer::initOneListByKind(const int &nKind)
{
    auto button = Button::create();
    
    std::string filename=__String::createWithFormat("PlazzScene_Src/gamelist/game_%d.png",nKind)->getCString();
    
    button->loadTextures(filename,filename);
    button->setTag(nKind);
    char buf[32] = "";
    sprintf(buf, "game_kind_%d", nKind);
    button->setName(buf);

    return button;
}

//MARK::游戏入口
void RoomLayer::loadingGame(int game)
{
    EntityMgr::instance()->getDispatch()->m_dwKindID=game;
	onQuickStart(nullptr);
}

//MARK::按钮事件
void RoomLayer::buttonEventWithTouchUser(cocos2d::Ref *target, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {

    }
}

void RoomLayer::showLoginAward()
{
	isGoingRoomLayer++;
	forbidButtonClick();
	if(pWarningLayer==NULL)
	{
		pWarningLayer = WarningLayer::create();
		addChild(pWarningLayer,10);
		pWarningLayer->setVisible(false);
	}
	pWarningLayer->setVisible(true);
}
void RoomLayer::closeLoginAward(){

}

void RoomLayer::showSelectHead(CCObject* obj)
{
	CCLOG("select head");
}

void RoomLayer::onStart(ROOM_TYPE index)
{
	//   
	bool ret = EntityMgr::instance()->getDispatch()->connectGameServer((int)index);
	if (!ret)
	{
		PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
		addChild(box,20);
		return;

	}

	EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(6);
	EntityMgr::instance()->getDispatch()->setStartType(false);
}

void RoomLayer::showPlayerInformation(CCObject *obj){
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	CCScene *pScene = CCScene::create();
	PlayerInfoLayer *pPlayInfoLayer = PlayerInfoLayer::create();
	pScene->addChild(pPlayInfoLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

/**底下菜单函数*/
void RoomLayer::toFriend(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	FriendSystemLayer *tabLayer = FriendSystemLayer::create();

	CCScene *scene = CCScene::create();
	scene->addChild(tabLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void RoomLayer::toShop(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	ShopSystemLayer *tabLayer = ShopSystemLayer::create();

	CCScene *scene = CCScene::create();
	scene->addChild(tabLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

//快速游戏
void RoomLayer::onQuickStart(CCObject *pSender)
{
	bool ret = EntityMgr::instance()->getDispatch()->connectGameServer();
	if (!ret)
	{
		PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
		addChild(box,20);
		return;

	}
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
	EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(6);
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
	EntityMgr::instance()->getDispatch()->setStartType(true);

	Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_game,true));
}

void RoomLayer::mission(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	MissionLayer *missionLayer = MissionLayer::create();

	CCScene *scene = CCScene::create();
	scene->addChild(missionLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void RoomLayer::toRank(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	RankSystemLayer *tabLayer = RankSystemLayer::create();

	CCScene *scene = CCScene::create();
	scene->addChild(tabLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

/**中间菜单*/
void RoomLayer::toExchange(CCObject* obj)	
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	ExchangerLayer *missionLayer = ExchangerLayer::create();

	CCScene *scene = CCScene::create();
	scene->addChild(missionLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}	

void RoomLayer::toLuckyDraw(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	LuckyDraw *layer = LuckyDraw::create();
	addChild(layer,10);
}

void RoomLayer::toVIP(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	pWarningLayer = VIPSystemLayer::create();
	addChild(pWarningLayer,10);
}

/**上面菜单函数*/

void RoomLayer::rechange(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	FreeCoins* freeCoins = FreeCoins::create();
	CCScene* pScene = CCScene::create();
	pScene->addChild(freeCoins);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void RoomLayer::signIn(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	pWarningLayer = WarningLayer::create();
	addChild(pWarningLayer,10);
}

void RoomLayer::setting(CCObject* obj)
{
	playButtonSound();
	if (!mMenuCloseOrOpen)
	{
		mMenuLayer = MenuLayer::create();
		addChild(mMenuLayer,2);
		mMenuCloseOrOpen = true;
	}else{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
	}
}

void RoomLayer::message(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	CCUserDefault::sharedUserDefault()->setBoolForKey("MESSAGESCAN",true);
	CCScene* scene = CCScene::create();
	EmailLayer* layer = EmailLayer::create();
	scene->addChild(layer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void RoomLayer::toHelp(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	CCScene *scene = CCScene::create();
	HelpLayer *pHelplayer = HelpLayer::create();
	scene->addChild(pHelplayer);
	CCDirector::sharedDirector()->replaceScene(scene); 
}

void RoomLayer::pressBack(CCObject* obj)
{
	if(roomLevel == ROOM_LAYER_LEVEL_HOBBY)
	{
		LoginLayer *layer = LoginLayer::create();
		CCScene *scene = CCScene::create();
		scene->addChild(layer);
		CCDirector::sharedDirector()->replaceScene(scene);
	}else if(roomLevel == ROOM_LAYER_LEVEL_GAMETYPE)
	{
		this->gameBackButton(NULL);
	}else if(roomLevel == ROOM_LAYER_LEVEL_GAMEROOM)
	{
		this->matchBackButton(NULL);
	}
}

void RoomLayer::toMenu(CCObject* obj)
{
	playButtonSound();
	if (!mMenuCloseOrOpen)
	{
		mMenuLayer = MenuLayer::create();
		addChild(mMenuLayer);
		mMenuCloseOrOpen = true;
	}else{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
	}
}

void RoomLayer::rechangeLittleButton(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	if(mMenuCloseOrOpen){
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//低等级 = 1 中等级 = 2 高等级 = 3 有X = 1 没X = 2
	if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[(((1-1) % 3 ) << 1) + 1])
	{
		showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","5");
	}
	else if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[(((2-1) % 3 ) << 1) + 1])
	{
		showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"2","5");
	}
	else
	{
		showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"3","5");
	}

#endif
}

void RoomLayer::huodong(cocos2d::CCObject *obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	if(mMenuCloseOrOpen){
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
	}
	CCScene *scene = CCScene::create();
	ActivityLayer * pActivityLayer = ActivityLayer::create();
	scene->addChild(pActivityLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void RoomLayer::geTCHARge(cocos2d::CCObject *obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	if(mMenuCloseOrOpen){
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
	}
}

void RoomLayer::DDZItemCallback( CCObject* pSender )
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	this->setRoomLayerLevel(ROOM_LAYER_LEVEL_GAMETYPE);
}

void RoomLayer::ShakeItemCallback( CCObject* pSender )
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
}

void RoomLayer::Lucky28ItemCallback( CCObject* pSender )
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
}

void RoomLayer::NiuniuItemCallback( CCObject* pSender )
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
}

void RoomLayer::ZajinHuaItemCallback( CCObject* pSender )
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
}

void RoomLayer::toNormalGame(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	m_nGameType = 0;
	this->setRoomLayerLevel(ROOM_LAYER_LEVEL_GAMEROOM);
}

void RoomLayer::toOtherGame(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	m_nGameType = 1;
	this->setRoomLayerLevel(ROOM_LAYER_LEVEL_GAMEROOM);
}

void RoomLayer::gameBackButton(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	this->setRoomLayerLevel(ROOM_LAYER_LEVEL_HOBBY);
}

void RoomLayer::matchBackButton(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	playButtonSound();
	this->setRoomLayerLevel(ROOM_LAYER_LEVEL_GAMETYPE);
}

void RoomLayer::setRoomLayerLevel(ROOM_LAYER_LEVEL level)
{
	return;
	if(level == ROOM_LAYER_LEVEL_HOBBY)
	{
		roomLevel = ROOM_LAYER_LEVEL_HOBBY;
		m_pMiddleMenu->setVisible(false);
		m_pMiddleMenu->setEnabled(false);

		m_pMiddleMenu2->setVisible(false);
		m_pMiddleMenu2->setEnabled(false);

		m_pMiddleHobbyMenu->setTouchEnabled(true);
		m_pMiddleHobbyMenu->setVisible(true);
		m_pMiddleHobbyMenu->setEnabled(true);
	}else if(level == ROOM_LAYER_LEVEL_GAMETYPE)
	{
		roomLevel = ROOM_LAYER_LEVEL_GAMETYPE;
		m_pMiddleMenu2->setVisible(false);
		m_pMiddleMenu2->setEnabled(false);
		m_pMiddleMenu->setVisible(true);
		m_pMiddleMenu->setEnabled(true);
		m_pMiddleHobbyMenu->setTouchEnabled(false);
		m_pMiddleHobbyMenu->setVisible(false);
		m_pMiddleHobbyMenu->setEnabled(false);
	}else if(level == ROOM_LAYER_LEVEL_GAMEROOM)
	{
		roomLevel = ROOM_LAYER_LEVEL_GAMEROOM;
		m_pMiddleMenu2->setVisible(true);
		m_pMiddleMenu2->setEnabled(true);
		m_pMiddleMenu->setVisible(false);
		m_pMiddleMenu->setEnabled(false);
		m_pMiddleHobbyMenu->setTouchEnabled(false);
		m_pMiddleHobbyMenu->setVisible(false);
		m_pMiddleHobbyMenu->setEnabled(false);
	}
}

void RoomLayer::toPrimaryGame(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	long lksdnb = ConfigMgr::instance()->m_Config.nServerEnter[3];
	if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[3])
	{
		PromptBox* box = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptStartFaild);
		box->setPromptText(ConfigMgr::instance()->text("t451"));
		addChild(box,20);
		return;
	}
	onStart(ROOM_TYPE_Primary);
}

void RoomLayer::toMiddleGame(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	long nefiugbv = ConfigMgr::instance()->m_Config.nServerEnter[2];
	onStart(ROOM_TYPE_Middle);
}

void RoomLayer::toSeniyGame(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[1])
	{
		MovingLabelLayer* moveingLayer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t451"),ccp(427,240));
		addChild(moveingLayer,20);
		return;
	}
	onStart(ROOM_TYPE_Seniy);
}

void RoomLayer::toSuperGame(CCObject* obj)
{
	if (mMenuCloseOrOpen)
	{
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
		return;
	}
	if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[0])
	{
		MovingLabelLayer* moveingLayer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t451"),ccp(427,240));
		addChild(moveingLayer,20);
		return;
	}
	onStart(ROOM_TYPE_Super);
}

void RoomLayer::toPhoneKefu(CCObject* obj)
{
	playButtonSound();
	isGoingRoomLayer++;
	forbidButtonClick();
	if(mMenuCloseOrOpen){
		mMenuLayer->removeFromParentAndCleanup(true);
		mMenuCloseOrOpen=false;
	}
	pCustomerService = CustomerServiceLayer::create();
	addChild(pCustomerService,50);		
}

void RoomLayer::permitButtonClick()
{
	m_pPlayerMenu->setTouchEnabled(true);			
	m_pButtonMenu->setTouchEnabled(true);			
	m_pStartMenu->setTouchEnabled(true);			
	m_pTopMenu->setTouchEnabled(true);				
	m_pMiddleMenu->setTouchEnabled(true);
	m_pMiddleMenu2->setTouchEnabled(true);
	m_pOtherMenu->setTouchEnabled(true);
	m_pMiddleHobbyMenu->setTouchEnabled(true);
}

void RoomLayer::forbidButtonClick()
{
	m_pPlayerMenu->setTouchEnabled(false);			
	m_pButtonMenu->setTouchEnabled(false);			
	m_pStartMenu->setTouchEnabled(false);			
	m_pTopMenu->setTouchEnabled(false);				
	m_pMiddleMenu->setTouchEnabled(false);
	m_pMiddleMenu2->setTouchEnabled(false);
	m_pOtherMenu->setTouchEnabled(false);
	m_pMiddleHobbyMenu->setTouchEnabled(false);
}

string RoomLayer::createAccount()
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

string RoomLayer::createPassword()
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

void RoomLayer::onRegistSucc(CCObject* obj)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_UI_ANS_REGSUCC);
	string szText0;
	if(strlen(g_GlobalUnits.getGolbalUserData()->szNickName)>0)
	{
		CCLOG("have nick name");
		szText0 = g_GlobalUnits.getGolbalUserData()->szNickName;
	}
	else
	{
		CCLOG("not have nick name");
		szText0 = EntityMgr::instance()->login()->getAccout();
	}
	long scoreNumber = g_GlobalUnits.getGolbalUserData()->lScore;
	string szText1 = CCString::createWithFormat("%d",scoreNumber)->getCString();//addCommaToNumber(scoreNumber);
	long telephonePointNumber = g_GlobalUnits.getGolbalUserData()->lGoldEggs;
	string szText2 = CCString::createWithFormat("%d",telephonePointNumber)->getCString();//addCommaToNumber(telephonePointNumber);


	CCLabelTTF* userNmae;
	if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
	{
		userNmae = CCLabelTTF::create(szText0.c_str(),"",20);
	}
	else
	{
		userNmae = CCLabelTTF::create(CCString::createWithFormat("%s%s",ConfigMgr::instance()->text("t161"),szText0.c_str())->getCString(),"",20);
	}
	m_pUserScore = CCLabelTTF::create(szText1.c_str(),"",17);
	m_pUserPhonePoint = CCLabelTTF::create(szText2.c_str(),"",17);
	userNmae->setColor(ccc3(0,0,0));
	m_pUserScore->setColor(ccc3(0,0,0));
	m_pUserPhonePoint->setColor(ccc3(0,0,0));
	userNmae->setPosition(ccp(winSize.width * 0.1f,winSize.height * 0.948f));
	m_pUserScore->setPosition(ccp(winSize.width * 0.265f,winSize.height * 0.925f));
	m_pUserPhonePoint->setPosition(ccp(winSize.width * 0.265f,winSize.height * 0.875f));
	userNmae->setAnchorPoint(ccp(0.0f,0.0f));
	m_pUserScore->setAnchorPoint(ccp(1.0f,0.5f));
	m_pUserPhonePoint->setAnchorPoint(ccp(1.0f,0.5f));
	addChild(userNmae);
	addChild(m_pUserScore);
	addChild(m_pUserPhonePoint,0,301);

	CCSprite *pPlayerNormalSprite;
	CCSprite *pPlayerSelectSprite;
	if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
	{
		pPlayerNormalSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_10.png"));
		pPlayerSelectSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_10.png"));
	}
	else
	{
		pPlayerNormalSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_8.png"));
		pPlayerSelectSprite = CCSprite::createWithSpriteFrame(spriteFrame("dt_register_8.png"));
	}

	pPlayerMenuSprite->setNormalImage(pPlayerNormalSprite);
	pPlayerMenuSprite->setSelectedImage(pPlayerSelectSprite);
	showLoginAward();
}

void RoomLayer::onlandcardResult(CCObject* obj)
{
	CCString* str = (CCString*)obj;
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(),ccp(427,240));
	addChild(layer,20);
}

void RoomLayer::onPhoneCardResult(CCObject* obj)
{
	CCString* str = (CCString*)obj;
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(),ccp(427,240));
	addChild(layer,20);
}

void RoomLayer::onLinkResult(CCObject* obj)
{
	CCInteger* Interger = (CCInteger*)obj;

	switch (Interger->getValue())
	{
	case 0:			// 0为连接失败 
		break;

	case 1:			// 1为连接成功
		{

		}
		break;

	case 2:			// 2为没有服务器
		{
			PromptBox* box = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeMakeSureModifyNickName);
			box->setPromptText(ConfigMgr::instance()->text("t475"));
			addChild(box,100);
		}
		break;
	case 3:
		{
			//破产弹框
			gameToBankrupt(true);
		}

	default:
		break;
	}
}
void RoomLayer::setGoingRoomLayer(){
	isGoingRoomLayer++;
}

void RoomLayer::onRegisterFaild(CCObject* obj)
{
	//注册账号失败重新注册游客账号
	int nGender = 1;
	int nFaceId = 0;
	string szAccount = createAccount();
	string szPassword = createPassword();

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if (!bRet)
	{

	}
	EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender,1);
}

void RoomLayer::showMoveingLabelTips(const char* str)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str,ccp(winSize.width * 0.5,winSize.height * 0.5));
	addChild(layer,100);
}

void RoomLayer::onPesent(CCObject* obj)
{
	tagScoreGifObj* obj1 = (tagScoreGifObj*)obj;

	//显示提示框
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	PromptBox * promptBox = PromptBox::PromptBoxWith(CCPointMake(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePasswordFind);
	addChild(promptBox,20);
	promptBox->setPromptText((const char*)obj1->szErrorDescribe);
}

void RoomLayer::onRefshTotalNum(CCObject* obj)
{
	return;
	GroupSprite* m_pHappyNormalSprite = GroupSprite::GroupSpriteWith("dt_happygame.png",CCString::createWithFormat("%d",(int)((g_GlobalUnits.m_nOnlineCount + 5000)*1.8))->getCString(),GorupSpriteTypePhotoAndText0,5);
	GroupSprite* m_pHappySelectSprite = GroupSprite::GroupSpriteWith("dt_happygame.png",CCString::createWithFormat("%d",(int)((g_GlobalUnits.m_nOnlineCount + 5000)*1.8))->getCString(),GorupSpriteTypePhotoAndText0,5);

	//初级场
	GroupSprite * primaryNormalSprite = GroupSprite::GroupSpriteWith("dt_primarymatch.png",CCString::createWithFormat("%d",(int)(((g_GlobalUnits.m_nOnlineCount + 5000)*1.8)*0.44))->getCString(),GorupSpriteTypePhotoAndText1,1);
	GroupSprite * primarySelectSprite = GroupSprite::GroupSpriteWith("dt_primarymatch.png",CCString::createWithFormat("%d",(int)(((g_GlobalUnits.m_nOnlineCount + 5000)*1.8)*0.44))->getCString(),GorupSpriteTypePhotoAndText1,1);

	//中级场
	GroupSprite * middleNormalSprite = GroupSprite::GroupSpriteWith("dt_middlematch.png",CCString::createWithFormat("%d",(int)(((g_GlobalUnits.m_nOnlineCount + 5000)*1.8)*0.3))->getCString(),GorupSpriteTypePhotoAndText1,2);
	GroupSprite * middleSelectSprite = GroupSprite::GroupSpriteWith("dt_middlematch.png",CCString::createWithFormat("%d",(int)(((g_GlobalUnits.m_nOnlineCount + 5000)*1.8)*0.3))->getCString(),GorupSpriteTypePhotoAndText1,2);

	//高级场
	GroupSprite * seniyNormalSprite = GroupSprite::GroupSpriteWith("dt_seniymatch.png",CCString::createWithFormat("%d",(int)(((g_GlobalUnits.m_nOnlineCount + 5000)*1.8)*0.26))->getCString(),GorupSpriteTypePhotoAndText1,3);
	GroupSprite * seniySelectSprite = GroupSprite::GroupSpriteWith("dt_seniymatch.png",CCString::createWithFormat("%d",(int)(((g_GlobalUnits.m_nOnlineCount + 5000)*1.8)*0.26))->getCString(),GorupSpriteTypePhotoAndText1,3);

	m_pPrimaryS->setNormalImage(primaryNormalSprite);
	m_pPrimaryS->setNormalImage(primarySelectSprite);
	m_pMiddleS->setNormalImage(middleNormalSprite);
	m_pMiddleS->setNormalImage(middleSelectSprite);
	m_pSeniyS->setNormalImage(seniyNormalSprite);
	m_pSeniyS->setNormalImage(seniySelectSprite);
}

void RoomLayer::onChange(CCObject* obj)
{
	tagChargeObj* obj1 = (tagChargeObj*)obj;

	//显示提示框
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	PromptBox * promptBox = PromptBox::PromptBoxWith(CCPointMake(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePasswordFind);
	addChild(promptBox,20);
	promptBox->setPromptText((const char*)obj1->szErrorDescribe);
}

void RoomLayer::gameToBankrupt(bool isBankrupt)
{
	if(ConfigMgr::instance()->m_Config.m_isGameBankrupcy)
	{
		//小游戏破产 弹出破产框
		ConfigMgr::instance()->m_Config.m_isGameBankrupcy = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","4");
#endif
		return;
	}

	//自己弹出破产框
	if(isBankrupt)
	{
		//显示破产框
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","4");
#endif
	}
}

void RoomLayer::showDailyGetCoins(CCObject* obj)
{
	if(g_GlobalUnits.getGolbalUserData()->nRelieveCount > 0)
	{
		bool ret = EntityMgr::instance()->getDispatch()->connectLoginServer();
		if (!ret)
		{
			PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
			this->addChild(box,20);
			return;
		}
		EntityMgr::instance()->getDispatch()->getRelieve(g_GlobalUnits.getGolbalUserData()->dwUserID);
	}
	else
	{
		CCScene* scene = CCScene::create();
		FreeLayer* layer = FreeLayer::create();
		scene->addChild(layer);
		CCDirector::sharedDirector()->replaceScene(scene);
	}
}

void RoomLayer::showDailyGetCoinsResult(CCObject* obj)
{
	CCInteger* inter = (CCInteger*)obj;
	PromptBox* box = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLackCoins);
	box->setPromptText(CCString::createWithFormat("%s%d%s%d%s",ConfigMgr::instance()->text("t476"),inter->getValue(),ConfigMgr::instance()->text("t477"),g_GlobalUnits.getGolbalUserData()->nRelieveCount,ConfigMgr::instance()->text("t478"))->getCString());
	addChild(box,50);
}
