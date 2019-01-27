//
//  roomLayer.cpp
//  Game
//
//  Created by zhouwei on 13-6-17.
//
//

#include "RoomLayer.h"
#include "GBEventIDs.h"
#include "TableViewLayer.h"
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
#include "GetCharge.h"
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
//#include "xy28First.h"
#include "GuessCoinSide.h"
#include "JsonHelper.h"
#include "MyNSString.h"

#include "ChongQingGambleLayer.h"
#include "LotterySettingView.h"
#include "LotteryMyLot.h"
#include	"LotteryGameResult.h"
#include "LotterySceneControl.h"
#include "LotteryUserInfo.h"
#include "LotteryMemberShipManagement.h"
#include "LotteryKindScrollView.h"
#include "LotteryActiveCenter.h"

#include "LotteryUpdate.h"
#include "HotUpdateLayer.h"//by hxh
#include "ChongQingGambleLayer.h"
#include "GuangDong11Xuan5Layer.h"
#include "bjpk10Layer.h"
#include "FC3dpl3Layer.h"
#include "BJK8GambleLayer.h"
#include "ScrollAdd.h"
#include "BJLSceneControl.h"//<by hxh>
#include "SceneView.h"

#include "VersionControl.h"
//百家乐
#ifdef USE_BJL
#include "SceneView.h"//by hxh
#endif
//dzpk
#ifdef  USE_DZPK
#include "DZPKLayer.h"
#endif
//dzz
#ifdef  USE_DZZ
#include "DDZSceneControl.h"
#endif

#ifdef USE_FISHBEAT
#include "GameScene.h"
#endif // USE_FISHBEAT

#ifdef USE_PPC
#include "PPCGameScene.h"
#endif

#ifdef USE_BRNN
#include "BRNNGameScene.h"
#endif

#ifdef USE_SGLY
#include "SGLYGameScene.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppController.h"
#//import "ConfigHeader.h"
#endif

#include "network/HttpClient.h"
#include "LotteryMailBox.h"
#include "LotteryMessageCenter.h"

using namespace cocos2d;

//static静态变量
int	RoomLayer::m_nowIndex=0;

Scene* RoomLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    Layer *layer = RoomLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

RoomLayer::RoomLayer()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //[YouMiNewSpot showYouMiSpotAction:nil];
#endif
    pWarningLayer=NULL;
    isGoingRoomLayer = 0;
	m_iRankCount = 0;

	m_bIsShowExitDialog = false;

    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::showDailyGetCoins),MSG_UI_ANS_CANNELRECHANGER , NULL);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(RoomLayer::showDailyGetCoinsResult),MSG_UI_ANS_GETSAVECOINS,NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onlandcardResult),MSG_UI_ANS_LANDCARDRESULT , NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onPhoneCardResult),MSG_UI_ANS_PHONECARDRESULT , NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onChange),MSG_UI_ANS_CHARGE, NULL);
    NotificationCenter::getInstance()->postNotification("CancelHttpRequest");
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::updateArticle), "UpdateUserPacket", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::updateRankList), "UpdateRankList", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::getCaiZhongList), "GetCaiZhongList", NULL);
	//站内信
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::getMessageCountRet), "GetMessageCountRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::toConnectGameRoomServer),MSG_UI_ANS_TOCONNECTGAMEROOMSERVER, NULL);//by hxh
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::toUpdateDownload),MSG_UI_ANS_UPDATEDOWNLOAD, NULL);//by hxh
}

RoomLayer::~RoomLayer()
{
   clear();
}

void RoomLayer::updateArticle(Object *obj)
{
    Array *data = (Array *)obj;
    for(int i = 0 ; i < data->count(); i+=2)
    {
        Integer *number1 = (Integer *)data->objectAtIndex(i);
        Integer *number2 = (Integer *)data->objectAtIndex(i+1);
        int key = number1->getValue();
        int value = number2->getValue();
        EntityMgr::instance()->getDispatch()->m_wArticleNum[key-2] = value;
    }
}

void RoomLayer::updateRankList(Object* obj)
{
	Array* data = (Array *)obj;
	if(data == nullptr || data->count() == 0) return;

	m_strRank.clear();

	//排行 
	auto myNode1 = (LabelTTF *)this->getChildByTag(8888);
	auto myNode2 = (LabelTTF *)this->getChildByTag(8889);

#ifdef VER_369
	myNode1 = (LabelTTF *)this->getChildByTag(1011)->getChildByTag(1010)->getChildByTag(8888);
	myNode2 = (LabelTTF *)this->getChildByTag(1011)->getChildByTag(1010)->getChildByTag(8889);
#endif

	if(myNode1 == nullptr || myNode2 == nullptr) return;

	myNode1->stopAllActions();
	myNode2->stopAllActions();
	myNode1->setPositionX(SCREEN_WIDTH);//位置
	myNode2->setPositionX(SCREEN_WIDTH);//位置

	for(int i = 0;i < data->count();i ++)
	{
		Dictionary* dic = (Dictionary *)data->objectAtIndex(i);
		String* newsStr = (String *)dic->objectForKey("s_t_news");		

		//接收到数据		
		m_strRank.append(newsStr->getCString());

		if(i == data->count() / 2 - 2)
		{
			myNode1->setString(m_strRank.c_str());
			m_strRank.clear();
		} 
		else if(i == data->count() - 3)
		{
			myNode2->setString(m_strRank.c_str());
			m_strRank.clear();
		}

		//保存数据

	}

	//清理
	data->removeAllObjects();
	
	//停止滚动	
	myNode2->setPositionX(myNode1->getPositionX() + myNode1->getContentSize().width);

	//滚动
	myNode1->runAction(RepeatForever::create(Sequence::create(
		CallFuncN::create([=](Ref* ){
			if(myNode2->getPositionX() < 0) //把node1加到node2后面
			{				
				myNode1->setPositionX(myNode2->getPositionX() + myNode2->getContentSize().width);
			}
			if(myNode1->getPositionX() < 0) //把node2加到node1后面
			{				
				myNode2->setPositionX(myNode1->getPositionX() + myNode1->getContentSize().width);
			}
		}),
		Spawn::createWithTwoActions(MoveBy::create(1.0f,cocos2d::Vec2(-100,0)),TargetedAction::create(myNode2,MoveBy::create(1.0f,cocos2d::Vec2(-100,0)))),
		nullptr)
		));
	//重置
	m_iRankCount = 0;	

	//过几秒后重发消息
	this->scheduleOnce([=](float delta){
		//
		EntityMgr::instance()->getDispatch()->SendPacketWithGetRankList();

	},300.0f,"RankList");

}

void RoomLayer::getCaiZhongList(Object *obj)
{
	Array* tempArr = (Array *)obj;
	if(tempArr == nullptr) return;
	tempArr->retain();
	MyBetNumber::getInstance()->saveCaiZhong(tempArr);
	tempArr->release();
}

void RoomLayer::getMessageCountRet(Object *obj)
{
	//return;

	//nResult
	Dictionary* dic = (Dictionary *)obj;
	if(dic == nullptr) return;
	Integer* I_count = (Integer *)dic->objectForKey("nResult");

	auto lbMessage = (LabelTTF *)this->getChildByTag(9527);
	if(lbMessage == nullptr) return;
	auto sptMessage = (Sprite *)this->getChildByTag(9528);
	if(sptMessage == nullptr) return;

	if(I_count->getValue() == 0)
	{		
		sptMessage->setVisible(false);
		lbMessage->setVisible(false);
	} else
	{
		sptMessage->setVisible(true);
		lbMessage->setVisible(true);
		lbMessage->setString(String::createWithFormat("%d", I_count->getValue())->getCString());
	}
}

void RoomLayer::toConnectGameRoomServer(Object* obj)
{
	// add by hxh 20161214
#ifdef USE_ROOM_LIST
	__Array *arr=(__Array *)obj;
	if(arr->count()>=2)
	{
		int idx=((Integer *)arr->getObjectAtIndex(0))->getValue();
		int tag=idx+GAME_KIND_SPRITE_TAG+1;
		int serverId=((Integer *)arr->getObjectAtIndex(1))->getValue();
		LotteryKindScrollView* pView=static_cast<LotteryKindScrollView* >(pHelpView);
		pView->connectGameRoomServerEx(tag,serverId);
	}
#else
	int idx = ((Integer *)obj)->getValue();
	int tag=idx+GAME_KIND_SPRITE_TAG+1;
	LotteryKindScrollView* pView=static_cast<LotteryKindScrollView* >(pHelpView);
	pView->connectGameRoomServer(tag);
#endif
}

void RoomLayer::toUpdateDownload(Object* obj)
{
	__Array *arr=(__Array *)obj;
	if(arr->count()>=2)
	{
		int idx=((Integer *)arr->getObjectAtIndex(0))->getValue();
		int serverId=((Integer *)arr->getObjectAtIndex(1))->getValue();
#ifdef USE_HOT_UPDATE
		HotUpdateLayer *layer=HotUpdateLayer::create(idx,serverId);
		layer->ignoreAnchorPointForPosition(false);
		layer->setOpacity(125);
		layer->setPosition(Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
		addChild(layer,255);
#else
#ifdef USE_ROOM_LIST
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_TOCONNECTGAMEROOMSERVER,arr);
#else
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_TOCONNECTGAMEROOMSERVER,Integer::create(idx));
#endif
#endif
	}
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
                onLoginFinish(NULL);
            }
            else
            {
                std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
                std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
                //                userAccount = DEFAULT_ACCOUNT;
                //                userPassword = DEFAULT_PASSWORD;
                if(userAccount.length() > 1 &&userPassword.length() > 1)
                {
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onLoginFaild), MSG_UI_ANS_LOGINFAIL, NULL);
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onLoginFinish), MSG_UI_ANS_LOGINFINISH, NULL);
                    //                    GBEVENT_ARG_REGISTER(GBEVENT_UI_LOGINFINISH, RoomLayer::onLoginFinish);
                    
                    bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
                    if(!bRet)
                    {
                        MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t26"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
                       this->addChild(layer,100);
                    }
                    else
                    {
                        EntityMgr::instance()->login()->setUserLogin(userAccount.c_str(), userPassword);
                    }
                }
                else
                {
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onRegisterFaild), MSG_UI_ANS_REGNFAIL, NULL);
                    
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onRegistSucc), MSG_UI_ANS_REGSUCC, NULL);
                    //û���˺� ע���ο��˺�
                    int nGender = 0;
                    int nFaceId = 0;
                    string szAccount = createAccount();
                    string szPassword = createPassword();
                    
                    bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
                    if (!bRet)
                    {
                        PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
                       this->addChild(box,100);
                    }
                    EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender,1);
                    
                }
            }
        }
        else
        {
            PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
           this->addChild(box,100);
        }
    }
}

void RoomLayer::onLoginFinish(Object *obj)
{
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFINISH);
    showLoginAward();
}

void RoomLayer::onLoginFaild(Object* obj)
{
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
    CCLOG("login faied");
    
    PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLoginFaild);
   this->addChild(box,100);
}

void RoomLayer::onEnter()
{
    if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        playBGM("roomback.mp3");
    }

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ddzCardtable.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ddz_resources.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("face.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ddz_popups_summary.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ddz_pokers_img.plist");

	//站内信条数需要更新
	EntityMgr::instance()->getDispatch()->SendPacketWithGetMessageCount();

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
    mMenuCloseOrOpen = false;
    isGoingRoomLayer =0;

    winSize = Director::getInstance()->getWinSize();

	//背景色 白色
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
    this->addChild(layer);
    
     //顶部的颜色部分
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame("beark2.png"));
	bk1->setAnchorPoint(Vec2(0,1)); 
	//bk1->setColor(ccc3(47, 41, 40));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);	
	bk1->setScaleY(156 /(bk1->getContentSize().height));
	this->addChild(bk1,1);
    
	//人物头像 左上角
    Sprite *pPlayerNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("room_02.png"));
    Sprite *pPlayerSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("room_02.png"));
    MenuItemSprite *pPlayerMenuSprite = MenuItemSprite::create(pPlayerNormalSprite,pPlayerSelectSprite,CC_CALLBACK_1(RoomLayer::showPlayerInformation, this));
	pPlayerMenuSprite->setAnchorPoint(Vec2(0,1));
	pPlayerMenuSprite->setPosition(Vec2(20, SCREEN_HEIGHT - 50));
    m_pPlayerMenu= Menu::create(pPlayerMenuSprite,NULL);
    this->addChild(m_pPlayerMenu,EHALLLayerZOrder_PlayerMenu);
    m_pPlayerMenu->setPosition(Vec2::ZERO);

	//静音按钮
	bool hasEffect = UserDefault::getInstance()->getBoolForKey("hasEffect", true);
	ui::Button* btMute = ui::Button::create("yx_08.png","yx_07.png","yx_07.png",ui::Widget::TextureResType::LOCAL); //静音
	btMute->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btMute->setPosition(Vec2(SCREEN_WIDTH - 50,SCREEN_HEIGHT - 120));
	this->addChild(btMute, EHALLLayerZOrder_PlayerMenu);
	ui::Button* btSound = ui::Button::create("yx_07.png","yx_08.png","yx_08.png",ui::Widget::TextureResType::LOCAL);//声音
	btSound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btSound->setPosition(btMute->getPosition());
	this->addChild(btSound, EHALLLayerZOrder_PlayerMenu);
	btMute->setVisible(!hasEffect);
	btSound->setVisible(hasEffect);	

	//
	ui::Button* btMessage = ui::Button::create("recharge_22.png","recharge_22.png","recharge_22.png",ui::Widget::TextureResType::LOCAL); //静音
	btMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btMessage->setPosition(Vec2(SCREEN_WIDTH - 110,SCREEN_HEIGHT - 120));
	this->addChild(btMessage, 5);

	//信息条数
	Sprite* sptMessage = Sprite::createWithSpriteFrame(spriteFrame("recharge_22_1.png"));
	sptMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptMessage->setTag(9528);
	sptMessage->setPosition(Vec2(btMessage->getPositionX() + btMessage->getContentSize().width / 2,SCREEN_HEIGHT - 120 + btMessage->getContentSize().height / 2));
	this->addChild(sptMessage, 6);

	LabelTTF* lbMessage = LabelTTF::create("0", "", 20, sptMessage->getContentSize());
	lbMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lbMessage->setPosition(sptMessage->getPosition());
	lbMessage->setFontFillColor(ccc3(161,47,47));
	lbMessage->setTag(9527);
	this->addChild(lbMessage, 6);

	lbMessage->setVisible(false);
	sptMessage->setVisible(false);
    
    //九亿娱乐平台图片
    float fontSize = 38;
    Sprite *title = Sprite::createWithSpriteFrame(spriteFrame("beark3.png"));
	title->setAnchorPoint(Vec2(0.5f,0.5f));
	title->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 78 - 20));   
    this->addChild(title, 1);   

	//顶部滚动图片 高286
	pScrollAdd = ScrollAdd::create();
	pScrollAdd->setAnchorPoint(Vec2(0,1));
	pScrollAdd->setPosition(Vec2(0, SCREEN_HEIGHT - 156 - 286));
	this->addChild(pScrollAdd, 1);

#ifdef VER_369
	bk1->setScaleY(158 / bk1->getContentSize().height);
	pScrollAdd->setPositionY(SCREEN_HEIGHT - 156 - 280);
	layer->setColor(ccc3(200,200,200));
#endif

	//新加滚动条
	const int heightDelta = 40;

	//滚动条背景和文字
	Sprite* sptRank = Sprite::createWithSpriteFrame(spriteFrame("blank.png"));
	sptRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptRank->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - 442 - heightDelta / 2));
	sptRank->setColor(ccc3(254, 225, 180));
	sptRank->setScaleX(SCREEN_WIDTH / sptRank->getContentSize().width);
	sptRank->setScaleY(heightDelta / sptRank->getContentSize().height);
	this->addChild(sptRank, 1);

	LabelTTF* ltfRank1 = LabelTTF::create();
	ltfRank1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfRank1->setPosition(Vec2(SCREEN_WIDTH,sptRank->getPositionY()));
	ltfRank1->setFontName("");
	ltfRank1->setFontSize(27);
	ltfRank1->setTag(8888);
	ltfRank1->setHorizontalAlignment(TextHAlignment::LEFT);
	ltfRank1->setFontFillColor(ccc3(255, 69, 55));	

	LabelTTF* ltfRank2 = LabelTTF::create();
	ltfRank2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfRank2->setPosition(Vec2(SCREEN_WIDTH,sptRank->getPositionY()));
	ltfRank2->setFontName("");
	ltfRank2->setFontSize(27);
	ltfRank2->setTag(8889);
	ltfRank2->setHorizontalAlignment(TextHAlignment::LEFT);
	ltfRank2->setFontFillColor(ccc3(255, 69, 55));	

	//menu
	Sprite *sptGameNormal = Sprite::createWithSpriteFrame(spriteFrame("yx_01.png"));
	Sprite *sptGameSelect = Sprite::createWithSpriteFrame(spriteFrame("yx_02.png"));
	Sprite *sptLotteryNormal = Sprite::createWithSpriteFrame(spriteFrame("yx_03.png"));
	Sprite *sptLotterySelect = Sprite::createWithSpriteFrame(spriteFrame("yx_04.png"));
	Sprite *sptPhysicNormal = Sprite::createWithSpriteFrame(spriteFrame("yx_05.png"));
	Sprite *sptPhysicSelect = Sprite::createWithSpriteFrame(spriteFrame("yx_06.png"));	
	MenuItemSprite *gameItem = MenuItemSprite::create(sptGameNormal,sptGameSelect,CC_CALLBACK_1(RoomLayer::toGame, this));
	MenuItemSprite *lotteryItem = MenuItemSprite::create(sptLotteryNormal,sptLotterySelect,CC_CALLBACK_1(RoomLayer::toLottery, this));
	MenuItemSprite *phyItem = MenuItemSprite::create(sptPhysicNormal,sptPhysicSelect,CC_CALLBACK_1(RoomLayer::toPhysic, this));
	selectMenu = Menu::create(gameItem,lotteryItem,phyItem,NULL);	
	selectMenu->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - 484 - heightDelta)); //
	selectMenu->alignItemsHorizontallyWithPadding(0);//直接对齐
	this->addChild(selectMenu,2);

	//下划线
	//Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	//gap1->setAnchorPoint(Vec2(0,0.5));
	//gap1->setPosition(Vec2(0, SCREEN_HEIGHT - 526 - heightDelta));    //526    
	//this->addChild(gap1,2);  

	//框列表 修改成可以根据字符串和个数来动态加载图片的框  高604 
	pHelpView = LotteryKindScrollView::create();
	pHelpView->setPosition(Vec2(0,150 - heightDelta + 5));
	this->addChild(pHelpView, 1);


#ifdef VER_369
	//屏蔽彩票游戏选择
	selectMenu->setVisible(false);

	pHelpView->setPositionY(pHelpView->getPositionY() + 30);

	//游戏的layer:TOP 10 MSG
	Sprite* sptTopBack = Sprite::createWithSpriteFrame(spriteFrame("beting55.png"));
	sptTopBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptTopBack->setTag(1011);
	sptTopBack->setPosition(Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 + 65));
	this->addChild(sptTopBack,2);

	//其它的图片加到这个sprite上面
	//理财宝
	Sprite* sptLiCaiBao = Sprite::createWithSpriteFrame(spriteFrame("yx_02_02.png"));
	sptLiCaiBao->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptLiCaiBao->setPosition(Vec2(sptTopBack->getContentSize().width / 4,sptLiCaiBao->getContentSize().height / 2 + 10));
	sptTopBack->addChild(sptLiCaiBao);
	
	//猜涨跌 
	Sprite* sptCaiZhangDie = Sprite::createWithSpriteFrame(spriteFrame("yx_02_01.png"));
	sptCaiZhangDie->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptCaiZhangDie->setPosition(Vec2(sptTopBack->getContentSize().width * 3 / 4,sptCaiZhangDie->getContentSize().height / 2 + 10));
	sptTopBack->addChild(sptCaiZhangDie);

	auto temp = RadioButtonGroup::create();
	temp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	temp->setPosition(Vec2(80,sptTopBack->getContentSize().height / 2 + 10));		
	temp->setAllowedNoSelection(false);
	temp->setSwallowTouches(true);
	sptTopBack->addChild(temp);

	//上面的导航条 5个球  顺序为棋牌游戏，时时彩，11选5，快乐彩， 3d
	for(int i = 0;i < 5;i ++)
	{
		auto tempRadio = RadioButton::create(String::createWithFormat("yx_01_%02d.png",i * 2 + 2)->getCString(),String::createWithFormat("yx_01_%02d.png",i * 2 + 1)->getCString());
		tempRadio->setPosition(Vec2(22+127 * i,0));	
		tempRadio->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		tempRadio->setTag(i + 1000);
		temp->addRadioButton(tempRadio);
		temp->addChild(tempRadio);

		//默认选中第二个
		if(i == m_nowIndex)
		{
			temp->setSelectedButton(tempRadio);
		}

		//添加事件处理
		tempRadio->addClickEventListener([=](Ref *obj){

			int iTag = tempRadio->getTag() - 1000;

			//判断是不是已经在当前页了
			if(iTag == m_nowIndex)
				return;
			m_nowIndex = iTag;			//这个值需要保存为全局
			
			std::string name = "";
			int count = 0;			

			if(iTag == 0)
			{
				name = "game_kind_";
				count = MAX_GAME;				
			}else
			{
				name = "lottery_kind_";
				count = LOTTERY_COUNT;
			}			

			LotteryKindScrollView* pView=static_cast<LotteryKindScrollView* >(pHelpView);
			pView->resetTable(name.c_str(), count, iTag);

		});
	}

	//加一个框
	auto sclRank = ui::ScrollView::create();
	sclRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sclRank->setPosition(Vec2(SCREEN_WIDTH / 2 + 48 , sptTopBack->getContentSize().height - 35));
	sclRank->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	sclRank->setSize(cocos2d::Size(535,100));
	sclRank->setTag(1010);
	sptTopBack->addChild(sclRank);
	sclRank->setEnabled(false);	

	//公告滚动效果
	
	ltfRank1->setPosition(Vec2(ltfRank1->getContentSize().width,sclRank->getContentSize().height / 2));
	ltfRank2->setPosition(Vec2(ltfRank2->getContentSize().width,sclRank->getContentSize().height / 2));
	sclRank->addChild(ltfRank1,4);
	sclRank->addChild(ltfRank2,4);	

	//----------------------


	//彩票背景
	Sprite* sptBottomBack = Sprite::createWithSpriteFrame(spriteFrame("room_00.png"));
	sptBottomBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptBottomBack->setPosition(sptTopBack->getPosition() + Vec2(0,-sptTopBack->getContentSize().height- 90));
	sptBottomBack->setScaleX(sptTopBack->getContentSize().width / sptBottomBack->getContentSize().width);
	sptBottomBack->setScaleY(435 / sptBottomBack->getContentSize().width);
	this->addChild(sptBottomBack,0);
#else
this->addChild(ltfRank2, 2);
this->addChild(ltfRank1, 2);
#endif
    
	//底部按钮
    Sprite *hobbyNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_04.png"));
    Sprite *hobbySelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_05.png"));
    Sprite *gambleNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_06.png"));
    Sprite *gambleSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_07.png"));
    Sprite *recordNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_08.png"));
    Sprite *recordSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_09.png"));
    Sprite *myLotteryNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_10.png"));
    Sprite *myLotterySelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_11.png"));
	Sprite *activeNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_add_12.png"));
	Sprite *activeSelectImage = Sprite::createWithSpriteFrame(spriteFrame("room_add_13.png"));
    MenuItemSprite *hobbyItem = MenuItemSprite::create(hobbyNormalImage,hobbySelectedImage,CC_CALLBACK_1(RoomLayer::toRoom,this));
    MenuItemSprite *gambleItem = MenuItemSprite::create(gambleNormalImage,gambleSelectedImage,CC_CALLBACK_1(RoomLayer::toGamble,this));
    MenuItemSprite *recordItem = MenuItemSprite::create(recordNormalImage,recordSelectedImage,CC_CALLBACK_1(RoomLayer::toRecord,this));
    MenuItemSprite *myLotteryItem = MenuItemSprite::create(myLotteryNormalImage,myLotterySelectedImage,CC_CALLBACK_1(RoomLayer::toMyLottery,this));
	MenuItemSprite *activItem = MenuItemSprite::create(activeNormalImage,activeSelectImage,CC_CALLBACK_1(RoomLayer::toActive, this));
    m_pButtonMenu = Menu::create(hobbyItem,gambleItem,recordItem,activItem,myLotteryItem,NULL);
 	m_pButtonMenu->setPosition(Vec2(SCREEN_WIDTH/2, 65));			//底部高130
    m_pButtonMenu->alignItemsHorizontallyWithPadding(0);			//底部直接对齐
    this->addChild(m_pButtonMenu,EHALLLayerZOrder_ButtonMenu);   

    gameToBankrupt(false);
	permitButtonClick();
	
	//EntityMgr::instance()->getDispatch()->SendPacketWithGetSystemTime();
	//this->schedule(schedule_selector(RoomLayer::getSystemTime), 10);

	static bool isFirstRun = true;

	//控制程序只运行一次
	if (isFirstRun) {
		sendHttpRequest();
		isFirstRun = false;
	}

	////初始化为彩票
	//resetSelect();
	//lotteryItem->selected();
	std::string name = "lottery_kind_";
	int count = LOTTERY_COUNT;
	if(m_nowIndex == 0)
	{
		name = "game_kind_";
		count = MAX_GAME;				
	}
	else
	{
		name = "lottery_kind_";
		count = LOTTERY_COUNT;
	}
	//count = MyBetNumber::getInstance()->getCaiZhongCount();
	static_cast<LotteryKindScrollView* >(pHelpView)->resetTable(name.c_str(), count, m_nowIndex);

	//添加点击事件
	auto funMute = [=](Ref *obj){
		bool hasEffect = UserDefault::getInstance()->getBoolForKey("hasEffect", true);
		//取反
		hasEffect = !hasEffect;
		//保存
		UserDefault::getInstance()->setBoolForKey("hasEffect", hasEffect);
		btMute->setVisible(!hasEffect);
		btSound->setVisible(hasEffect);
		if(hasEffect) playButtonSound();
	};
	btMute->addClickEventListener(funMute);
	btSound->addClickEventListener(funMute);

	btMessage->addClickEventListener([=](Ref *){

			Scene *scene = LotteryMessageCenter::scene();
            Director::getInstance()->pushScene(scene);
			Scene *scene1 = LotteryMailBox::scene();
			Director::getInstance()->pushScene(scene1);

			clear();
	});

	//滚动 排行榜
	EntityMgr::instance()->getDispatch()->SendPacketWithGetRankList();

    return  true;
}

void RoomLayer::getSystemTime(float dt)
{
	//EntityMgr::instance()->getDispatch()->SendPacketWithGetSystemTime();
}

void RoomLayer::showLoginAward()
{
 //   isGoingRoomLayer++;
 //   forbidButtonClick();
 //   if(pWarningLayer==NULL)
 //   {
 //       pWarningLayer = WarningLayer::create();
 //      this->addChild(pWarningLayer,10);
 //       pWarningLayer->setVisible(false);
 //   }
 //   pWarningLayer->setVisible(true);
}
void RoomLayer::closeLoginAward(){
    /*permitButtonClick();
     pWarningLayer->removeFromParentAndCleanup(true);*/
}

void RoomLayer::showPlayerInformation(Object *obj){
    playButtonSound();
    Scene *scene = Scene::create();
    LotteryUserInfo *layer = LotteryUserInfo::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));    
}

/**���²˵�����*/
void RoomLayer::toRoom(Object* obj)
{
   playButtonSound();
}

void RoomLayer::toGamble(Object* obj)
{
    playButtonSound();
    LotteryMemberShipManagement *layer = LotteryMemberShipManagement::create();
    
    Scene *scene = Scene::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void RoomLayer::toRecord(Object* obj)
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
    LotteryGameResult *layer = LotteryGameResult::create();
    Scene *scene = Scene::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

int RoomLayer::getidx(int KindId)
{
	int idx=-1;
	switch (KindId)
	{
	case KindId_DZPK:
		idx=0;
		break;
	case KindId_DDZ:
		idx=1;
		break;
	case KindId_BJL:
		idx=2;
		break;
	case KindId_FISH:
		idx=3;
		break;
	case KindId_PPC:
		idx=4;
		break;
	case KindId_BRNN:
		idx=5;
		break;
	case KindId_SGLY:
		idx=6;
		break;
	default:
		break;
	}
	return idx;
}

int RoomLayer::getKindId(int idx)
{
	//int KindId=0;
	//switch (idx)
	//{
	//case 0:
	//	KindId=KindId_DZPK;
	//	break;
	//case 1:
	//	KindId=KindId_DDZ;
	//	break;
	//case 2:
	//	KindId=KindId_BJL;
	//	break;
	//case 3:
	//	KindId=KindId_FISH;
	//	break;
	//case 4:
	//	KindId=KindId_PPC;
	//	break;
	//case 5:
	//	KindId=KindId_BRNN;
	//	break;
	//default:
	//	break;
	//}
	//return KindId;
	if(idx >= MAX_GAME) return 0;

	return gameKindIdList[idx];
}

void RoomLayer::toGame(Object* obj)
{	
	playButtonSound();

	resetSelect();
	MenuItemSprite* temp = static_cast<MenuItemSprite*>(obj);
	temp->selected();

	std::string name = "game_kind_";
	int count = MAX_GAME;

	LotteryKindScrollView* pView=static_cast<LotteryKindScrollView* >(pHelpView);
	pView->resetTable(name.c_str(), count, 0);

	// 点击棋牌分页，自动连接上次被卡的房间<add by hxh 20161123>
	WORD ServerId=EntityMgr::instance()->getDispatch()->m_wLastServerID;
	WORD KindId=EntityMgr::instance()->getDispatch()->m_wLastKindID;
	if(ServerId>0 && KindId>0)
	{
		int idx=RoomLayer::getidx(KindId);
		if(idx>-1)
		{
			__Array *arr=__Array::create();
			arr->addObject(Integer::create(idx));
			arr->addObject(Integer::create(ServerId));
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATEDOWNLOAD,arr);
		}
		//switch (KindId)
		//{
		//case KindId_DZPK:
		//	pView->pressKindButton(GAME_KIND_SPRITE_TAG+1);
		//	break;
		//case KindId_DDZ:
		//	pView->pressKindButton(GAME_KIND_SPRITE_TAG+2);
		//	break;
		//case KindId_BJL:
		//	pView->pressKindButton(GAME_KIND_SPRITE_TAG+3);
		//	break;
		//case KindId_FISH:
		//	pView->pressKindButton(GAME_KIND_SPRITE_TAG+4);
		//	break;
		//case KindId_PPC:
		//	pView->pressKindButton(GAME_KIND_SPRITE_TAG+5);
		//	break;
		//default:
		//	break;
		//}

	}
	
}

void RoomLayer::toLottery(Object* obj)
{
	playButtonSound();

	resetSelect();
	MenuItemSprite* temp = static_cast<MenuItemSprite*>(obj);
	temp->selected();	

	std::string name = "lottery_kind_";
	int count = LOTTERY_COUNT;
	count = MyBetNumber::getInstance()->getCaiZhongCount();
	static_cast<LotteryKindScrollView* >(pHelpView)->resetTable(name.c_str(), count, 1);
}

void RoomLayer::toPhysic(Object* obj)
{
	playButtonSound();

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
	this->addChild(layer,20);

	return;

	resetSelect();
	MenuItemSprite* temp = static_cast<MenuItemSprite*>(obj);
	temp->selected();

	std::string name = ""; //目前尚未开放
	int count = 4;
	static_cast<LotteryKindScrollView* >(pHelpView)->resetTable(name.c_str(), count, 2);
}

void RoomLayer::resetSelect()
{
	int count = selectMenu->getChildrenCount();
	for(int i = 0;i < count;i ++)
	{
		MenuItemSprite* temp = (MenuItemSprite *)selectMenu->getChildren().at(i);
		if(temp != NULL)
			temp->unselected();
	}
}

void RoomLayer::refreshLayer()
{
	NotificationCenter::getInstance()->postNotification("NeedRefreshData");
}

void RoomLayer::toMyLottery(Object* obj)
{
    if (mMenuCloseOrOpen)
    {
        mMenuLayer->removeFromParentAndCleanup(true);
        mMenuCloseOrOpen=false;
        return;
    }
    playButtonSound();
    //isGoingRoomLayer++;
    //forbidButtonClick();
    LotteryMyLot *tabLayer = LotteryMyLot::create();
    //this->addChild(tabLayer,10);
    
    Scene *scene = Scene::create();
    scene->addChild(tabLayer);
    Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	clear();
}

void RoomLayer::toActive(Object* obj)
{
	playButtonSound();
	LotteryActiveCenter *layer = LotteryActiveCenter::create();

	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void RoomLayer::sendHttpRequest()
{	
	//宏定义手机平台
	string url = ConfigMgr::instance()->text("display_config.xml", "t5022"); 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	url = ConfigMgr::instance()->text("display_config.xml", "t5021"); 
#endif
	CCLOG("%s completed", url.c_str());
	network::HttpRequest* request = new network::HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, callfuncND_selector(RoomLayer::onHttpRequestCompleted));
	request->setTag("GET test1");	
	network::HttpClient::getInstance()->send(request); 
	network::HttpClient::getInstance()->setTimeoutForConnect(10);
	network::HttpClient::getInstance()->setTimeoutForRead(10);
	request->release();
}

void RoomLayer::onHttpRequestCompleted(Node *sender, void *data)  
{  
	network::HttpResponse *response = (network::HttpResponse*)data;  

	if (!response)  
	{  
		return;  
	}  

	if (0 != strlen(response->getHttpRequest()->getTag()))   
	{  
		CCLOG("%s completed", response->getHttpRequest()->getTag());  
	}  

	int statusCode = response->getResponseCode();  
	char statusString[64] = {};  
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
	//m_labelStatusCode->setString(statusString);  
	CCLOG("response code: %d", statusCode);  

	if (!response->isSucceed())   
	{  
		CCLOG("response failed");  
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;  
	}  

	// dump data  
	std::vector<char> *buffer = response->getResponseData();
	if(buffer->size() < 3)
	{
		CCLOG("len_error");
		return;
	}
	std::string temp(buffer->begin() + 3,buffer->end());

	//释放内存
	std::vector<char>().swap(*buffer);
	
	//std::string temp(buffer->begin(),buffer->end());
    String* responseData=String::create(temp);	

	//Json::Value value = JsonHelper::getJsonFromData(dataBuffer, dataSize);	

	Json::Reader reader;//json解析  
    Json::Value value;//表示一个json格式的对象  
	if(reader.parse(responseData->getCString(),value))//解析出json放到json中区  
    {  		
		string vercode = value["version"].asString();
		string updateUrl = value["packageUrl"].asString();
	
		//获得当前版本号 直接从xml中读取版本号
		string currVercode = ConfigMgr::instance()->text("display_config.xml", "t5023"); 
		if (vercode.compare(currVercode) != 0)
		{			
			LotteryUpdate::showModal(this,updateUrl);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//			JniMethodInfo minfo;
//			if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","loadMainSo","()V") )
//			{
//				minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
//			}
//#endif
		}	
	}
} 

void RoomLayer::setting(Object* obj)
{
    playButtonSound();
    
    LotterySettingView *layer = LotterySettingView::create();
    
    Scene *scene = Scene::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void RoomLayer::permitButtonClick()
{
	m_bIsShowExitDialog = false;

	m_pPlayerMenu->setTouchEnabled(true);
	m_pButtonMenu->setTouchEnabled(true);
	pHelpView->setTouchEnabled(true);
	pScrollAdd->setTouchEnabled(true);
	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
}

void RoomLayer::forbidButtonClick()
{
	m_pPlayerMenu->setTouchEnabled(false);
	m_pButtonMenu->setTouchEnabled(false);
	pHelpView->setTouchEnabled(false);
	pScrollAdd->setTouchEnabled(false);
	this->setKeypadEnabled(true);
}

string RoomLayer::createAccount()
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

void RoomLayer::onRegistSucc(Object* obj)
{
    NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_REGSUCC);
    showLoginAward();
}

void RoomLayer::onlandcardResult(Object* obj)
{
    String* str = (String*)obj;
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(), Vec2(427, SCREEN_WIDTH/2));
   this->addChild(layer,20);
}

void RoomLayer::onPhoneCardResult(Object* obj)
{
    String* str = (String*)obj;
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(), Vec2(427, SCREEN_WIDTH/2));
   this->addChild(layer,20);
}

void RoomLayer::onLinkResult(Object* obj)
{
    Integer* Interger = (Integer*)obj;
    
    switch (Interger->getValue())
    {
        case 0:			// 0Ϊ����ʧ��
            break;
            
        case 1:			// 1Ϊ���ӳɹ�
        {
           // EntityMgr::instance()->getDispatch()->sendLoginPacket();
			// add by hxh 20160704
			DWORD m_dwKindID=EntityMgr::instance()->getDispatch()->m_dwKindID;
			if(m_dwKindID==KindId_BJL){
				// add by hxh 20160701
#ifdef USE_BJL
			    MyNSString::toLandscape();
				BJLSceneControl* pSceneControl = BJLSceneControl::sharedSceneControl();
				Scene* pScene = pSceneControl->getScene(SCENE_game,true);
				Director::getInstance()->replaceScene(pScene);
#endif
			}
#ifdef USE_DZPK
			//dzpk-jhy
			else if (m_dwKindID == KindId_DZPK)
			{
				MyNSString::toLandscape();
				//BJLSceneControl* pSceneControl = BJLSceneControl::sharedSceneControl();
				Scene* pScene = DZPKLayer::scene();//pSceneControl->getScene(SCENE_game,true);
				Director::getInstance()->replaceScene(pScene);
			}
#endif
#ifdef  USE_DZZ
			else if (m_dwKindID == KindId_DDZ)
			{
				// add by hxh 20160701
				MyNSString::toLandscape();
				DDZSceneControl* pSceneControl = DDZSceneControl::sharedDDZSceneControl();
				Scene* pScene = pSceneControl->getScene(SCENE_game,true);
				Director::getInstance()->replaceScene(pScene);
			}
#endif

#ifdef USE_FISHBEAT 
			else if (m_dwKindID == KindId_FISH)
			{
				//这里捕鱼游戏入口
				MyNSString::toLandscape();	

				Scene* pscene = CGameScene::createScene();
				Director::getInstance()->replaceScene(pscene);								
			}

#ifdef USE_PPC
			else if (m_dwKindID == KindId_PPC)
			{
				MyNSString::toLandscape();
				PPCGameScene* pScene = PPCGameScene::create();
				Director::getInstance()->replaceScene(pScene);
			}
#endif

#ifdef USE_BRNN
			else if (m_dwKindID == KindId_BRNN)
			{
				MyNSString::toLandscape();
				BRNNGameScene* pScene = BRNNGameScene::create();
				Director::getInstance()->replaceScene(pScene);
			}
#endif

#ifdef USE_SGLY
			else if (m_dwKindID == KindId_SGLY)
			{
				MyNSString::toDontShowInfo();
// 				SGLYGameScene* pScene = SGLYGameScene::create();
				Scene* pScene = SGLYGameScene::createScene();
				Director::getInstance()->replaceScene(pScene);
			}
#endif

#endif       
		}
		break;
            
        case 2:			// 2Ϊû�з�����
        {
            PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeMakeSureModifyNickName);
            box->setPromptText(ConfigMgr::instance()->text("t475"));
           this->addChild(box,100);
        }
            break;
        case 3:
        {
            //�Ʋ�����
            gameToBankrupt(true);
        }
            
        default:
            break;
    }
}
void RoomLayer::setGoingRoomLayer(){
    isGoingRoomLayer++;
}

void RoomLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		if(!m_bIsShowExitDialog)
		{		
			m_bIsShowExitDialog = true;

			forbidButtonClick();
			playButtonSound();
			PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptexitSelect);
			this->addChild(box,100,123);
		}
		event->stopPropagation();
	}
}

void RoomLayer::onRegisterFaild(Object* obj)
{
    //ע���˺�ʧ������ע���ο��˺�
    int nGender = 1;
    int nFaceId = 0;
    string szAccount = createAccount();
    string szPassword = createPassword();
    
    bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
    if (!bRet)
    {
        //cocos2d::Size winSize = Director::getInstance()->getWinSize();
        //PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeLoginConnectFaild);
        //this->addChild(promptBox);
        //return;
    }
    
    //char buf[50];
    //memset(buf, 0, 50);
    //utf::utf_gb_convert("utf-8", "gb2312", buf,50, const_cast<char*>(szAccount.c_str()), szAccount.length());
    
    ///EntityMgr::instance()->getActor()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender);
    EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender,1);
}

void RoomLayer::showMoveingLabelTips(const char* str)
{
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str,Vec2(winSize.width * 0.5,winSize.height * 0.5));
   this->addChild(layer,100);
}

void RoomLayer::onPesent(Object* obj)
{
    tagScoreGifObj* obj1 = (tagScoreGifObj*)obj;
    
    //��ʾ��ʾ��
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePasswordFind);
   this->addChild(promptBox,20);
    promptBox->setPromptText((const char*)obj1->szErrorDescribe);
}

void RoomLayer::onChange(Object* obj)
{
    tagChargeObj* obj1 = (tagChargeObj*)obj;
    
    //��ʾ��ʾ��
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePasswordFind);
   this->addChild(promptBox,20);
    promptBox->setPromptText((const char*)obj1->szErrorDescribe);
}

void RoomLayer::gameToBankrupt(bool isBankrupt)
{
    if(ConfigMgr::instance()->m_Config.m_isGameBankrupcy)
    {
        //С��Ϸ�Ʋ� �����Ʋ���
        ConfigMgr::instance()->m_Config.m_isGameBankrupcy = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","4");
#endif
        return;
    }
    
    //�Լ������Ʋ���
    if(isBankrupt)
    {
        //��ʾ�Ʋ���
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","4");
#endif
    }
}

void RoomLayer::showDailyGetCoins(Object* obj)
{
    if(g_GlobalUnits.getGolbalUserData()->nRelieveCount > 0)
    {
        bool ret = EntityMgr::instance()->getDispatch()->connectLoginServer();
        if (!ret)
        {
			PromptBox* box = PromptBox::PromptBoxWith(Vec2(427, SCREEN_WIDTH/2), mPromptTypeServerShut);
            this->addChild(box,20);
            return;
        }
        EntityMgr::instance()->getDispatch()->getRelieve(g_GlobalUnits.getGolbalUserData()->dwUserID);
    }
    else
    {
        Scene* scene = Scene::create();
        FreeLayer* layer = FreeLayer::create();
        scene->addChild(layer);
        Director::getInstance()->replaceScene(scene);
    }
}

void RoomLayer::showDailyGetCoinsResult(Object* obj)
{
    Integer* inter = (Integer*)obj;
    PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLackCoins);
    box->setPromptText(String::createWithFormat("%s%d%s%d%s",ConfigMgr::instance()->text("t476"),inter->getValue(),ConfigMgr::instance()->text("t477"),g_GlobalUnits.getGolbalUserData()->nRelieveCount,ConfigMgr::instance()->text("t478"))->getCString());
   this->addChild(box,50);
}

void RoomLayer::clear()
{
	//GBEVENT_ARG_UNREGISTER(GBEVENT_UI_LOGINFINISH, RoomLayer::onLoginFinish);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_REGNFAIL);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_CANNELRECHANGER);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_GETSAVECOINS);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_UPDATESCOREANDGOLDEGG);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LANDCARDRESULT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PHONECARDRESULT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_CHARGE);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_REFRESHTOTALNUM);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_GAMELINK);
	NotificationCenter::getInstance()->removeObserver(this, "UpdateUserPacket");
	NotificationCenter::getInstance()->removeObserver(this, "UpdateRankList");
	NotificationCenter::getInstance()->removeObserver(this, "GetCaiZhongList");
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_TOCONNECTGAMEROOMSERVER);//by hxh
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_UPDATEDOWNLOAD);//by hxh
	this->unschedule(schedule_selector(RoomLayer::getSystemTime));
	NotificationCenter::getInstance()->removeObserver(this, "GetMessageCountRet");
}