#include "DZPKLayer.h"
#include "RoomFrame.h"
#include "UserItem.h"
#include "GlobalFrame.h"
#include "CMD_GameServer.h"
#include "PromptBox.h"
#include "MovingLabelLayer.h"//by hxh
#include "GBEventIDs.h"
#include "GlobalDef.h"
#include "DZPKGameLogic.h"//by hxh
#include "GMFashionConfig.h"
#include "SimpleAudioEngine.h"
#include "SoundControl.h"//by hxh
#include "MyNSString.h"
#include "DDZRes.h"//by hxh
#include "DZPKTools.h"
#include "MyConfig.h"//by hxh
#include "DZPKDialog.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JniHelper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppController.h"
#endif

using namespace CocosDenshion;

#define MSG_OPERALAYER_BTN_PRESSED	"MSG_OPERALAYER_BTN_PRESSED"       //操作面板按钮已按下，取消定时器

using namespace cocos2d;
DZPKLayer::DZPKLayer()
{
	EntityMgr::instance()->getDispatch()->setGameStatus(true);

}

Scene* DZPKLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		DZPKLayer *layer = DZPKLayer::create();
		CC_BREAK_IF(! layer);
		layer->setTag(DZPKLayerTag);//by hxh
		scene->addChild(layer);
	} while (0);
	return scene;
}

DZPKLayer::~DZPKLayer()
{
	RemoveObject();
	CCLOG("DDZLayer－－－－－－~DZPKLayer－－－－－－－－");
}

void DZPKLayer::AddObject()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::onUserStart),"GameStart" , NULL);//<add by hxh 20160825>
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::onShutNetwork), MSG_UI_ANS_NETWORKSHUT, NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::onUpdateTable),MSG_UI_ANS_PLAYERUPDATE,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::onMessageGame),GBEVENT_UI_GAMEMESSAGE,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::onMessageScene),GBEVENT_UI_SCENEMESSAGE,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::onMessageMatchPlayer),MSG_UI_ANS_MATCH,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::onLoginConnectFaild),MSG_UI_ANSLoginConnectFaild,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DZPKLayer::OtherStarya),MSG_UI_ANSLoginOtherSFaild,NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::onLoginRoomFailed), MSG_UI_ANS_ROOMLOGINFAIL, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::onSitFaild), MSG_UI_ANS_SITFAIL, NULL);
	// add by hxh 20160919
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::updateTest), "updateTest", NULL);
#ifdef USE_PRINTTEST
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::PrintTest), "PrintTest", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::PrintTest1), "PrintTest1", NULL);
#endif
#ifdef USE_PRINTSCORE
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DZPKLayer::PrintScore1), "PrintScore1", NULL);
#endif
}

void DZPKLayer::RemoveObject()
{
	NotificationCenter::getInstance()->removeObserver(this, "GameStart");//<add by hxh 20160825>
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORKSHUT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PLAYERUPDATE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_GAMEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_SCENEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_MATCH);
	NotificationCenter::getInstance()->removeObserver(this, MSG_OPERALAYER_BTN_PRESSED);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANSLoginConnectFaild);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANSLoginOtherSFaild);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_ROOMLOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_SITFAIL);
	// add by hxh 20160919
	NotificationCenter::getInstance()->removeObserver(this, "updateTest");
#ifdef USE_PRINTTEST
	NotificationCenter::getInstance()->removeObserver(this, "PrintTest");
	NotificationCenter::getInstance()->removeObserver(this, "PrintTest1");
#endif
#ifdef USE_PRINTSCORE
	NotificationCenter::getInstance()->removeObserver(this, "PrintScore1");
#endif
}

//dzpk-jhy
void DZPKLayer::LoadRes(){
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dzpk.plist","dzpk.png");// 新的图片<by hxh 20160906>
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("DZPKImage.plist","DZPKImage.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("DZPKImageAdd.plist","DZPKImageAdd.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pic_show.plist","pic_show.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dzpk_raise_slider.plist","dzpk_raise_slider.png");
}

void DZPKLayer::dzpk_ToPortrait()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	GLView* eglView = Director::getInstance()->getOpenGLView();	
	eglView->setViewName("QiXing");
	eglView->setFrameSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	eglView->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, kResolutionExactFit);
#endif
	//切换竖屏代码 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	cocos2d::Size frameSize = pEGLView->getFrameSize();
	JniMethodInfo minfo;
	if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","changedActivityOrientation","(I)V") )
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,2);
	}
	pEGLView->setFrameSize(frameSize.height,frameSize.width);
	pEGLView->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT,kResolutionExactFit);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//竖屏
	[AppController changeRootViewControllerV];
	//重新适配
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	//获取屏幕分辨率
	CGRect tempRect = [UIScreen mainScreen].bounds;
	CGFloat scale_screen = [UIScreen mainScreen].scale;
	pEGLView->setFrameSize(tempRect.size.width * scale_screen,tempRect.size.height * scale_screen);
	pEGLView->setDesignResolutionSize(SCREEN_WIDTH,SCREEN_HEIGHT,kResolutionExactFit);
#endif
}

void DZPKLayer::dzpk_ToLandscape()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	MyConfig::Instance().LoadData("MyConfig.xml");
	GLView* eglView = Director::getInstance()->getOpenGLView();
	eglView->setViewName("Baccarat");
	eglView->setFrameSize(SCREEN_HEIGHT*0.8,SCREEN_WIDTH*0.8);
	eglView->setDesignResolutionSize(SCREEN_HEIGHT,SCREEN_WIDTH,kResolutionExactFit);
#endif
	//切换横屏代码
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	cocos2d::Size frameSize = pEGLView->getFrameSize(); 
	JniMethodInfo minfo;
	if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","changedActivityOrientation","(I)V") )
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,1);
	}
	pEGLView->setFrameSize(frameSize.height,frameSize.width);
	pEGLView->setDesignResolutionSize(SCREEN_HEIGHT,SCREEN_WIDTH,kResolutionExactFit);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//横屏
	[AppController changeRootViewControllerH];
	//重新适配
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	//获取屏幕分辨率
	CGRect tempRect = [UIScreen mainScreen].bounds;
	CGFloat scale_screen = [UIScreen mainScreen].scale;
	pEGLView->setFrameSize(tempRect.size.width * scale_screen,tempRect.size.height * scale_screen);
	pEGLView->setDesignResolutionSize(SCREEN_HEIGHT,SCREEN_WIDTH,kResolutionExactFit);
#endif
}

void DZPKLayer::ExitDZPK()
{
	EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
	EntityMgr::instance()->getDispatch()->closeGameSocket();
	EntityMgr::instance()->getDispatch()->SendReConnect();//dzpk-jhy
	EntityMgr::instance()->roomFrame()->reset();
	CocosDenshion::SimpleAudioEngine::getInstance()->end(); //dzpk-jhy
	DZPKLayer::dzpk_ToPortrait();//dzpk-jhy
	EntityMgr::instance()->getDispatch()->setGameStatus(false);
	Scene * scene = RoomLayer::scene();
	Director::getInstance()->replaceScene(scene);
}

Menu * DZPKLayer::createExitMenu()
{
	MyConfig& myConfig=MyConfig::Instance();
	Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_exit_1));
	Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_exit_2));
	pbuttonNormalClose->setScale(myConfig.m_ExitScale);
	pbuttonSelectClose->setScale(myConfig.m_ExitScale);
	MenuItemSprite *pButtonClose = MenuItemSprite::create(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(DZPKLayer::LeaveGameResume, this));
	pButtonClose->setTag(ExitTag);
	pbuttonSelectClose->setAnchorPoint(ccp(-0.5/9,-0.5/9));
	Menu *pMenuClose = Menu::create(pButtonClose,NULL);
	pMenuClose->setPosition(ccp(myConfig.m_DZPKClosePos[0],myConfig.m_DZPKClosePos[1]));
	return pMenuClose;
}

Menu * DZPKLayer::createStartLeaveMenu()
{
	MyConfig& myConfig=MyConfig::Instance();
	Sprite * Start1 = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_start_button1));
	Sprite * Start2 = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_start_button2));
	Sprite * exit_1 = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_exit_button1));
	Sprite * exit_2 = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_exit_button2));
	MenuItemSprite *StartItem = MenuItemSprite::create(Start1,Start2,CC_CALLBACK_1(DZPKLayer::LeaveGameResume, this));
	StartItem->setScale(myConfig.m_StartLeaveScale);
	StartItem->setTag(StartTag);
	MenuItemSprite *LeaveItem = MenuItemSprite::create(exit_1,exit_2,CC_CALLBACK_1(DZPKLayer::LeaveGameResume, this));
	LeaveItem->setScale(myConfig.m_StartLeaveScale);
	LeaveItem->setTag(LeaveTag);
	StartItem->setPosition(ccp(myConfig.m_StartItemPos[0],myConfig.m_StartItemPos[1]));
	LeaveItem->setPosition(ccp(myConfig.m_LeaveItemPos[0],myConfig.m_LeaveItemPos[1]));
	Menu *StartLeave= Menu::create(StartItem,LeaveItem,NULL);
	StartLeave->setPosition(ccp(0,0));

	return StartLeave;
}

bool DZPKLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! Layer::init());
		LoadRes();//dzpk-jhy
		m_WinSize = Director::getInstance()->getWinSize();	
		//setKeypadEnabled(true); //dzpk-jhy
		setTouchEnabled(true);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

		MyConfig& myConfig=MyConfig::Instance();

		Sprite* layerBG =Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_dzpklayerBG));//Sprite::create(texture_name::s_dzpklayerBG);
		layerBG->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height*0.5f));
		addChild(layerBG);

		// add by hxh 20160919
#ifdef USE_PRINTTEST
		m_Test[0]=LabelTTF::create("","",16);
		m_Test[0]->setAnchorPoint(ccp(0,0));
		m_Test[0]->setColor(ccc3(255,255,255));
		m_Test[0]->setPosition(ccp(m_WinSize.width*0.1,m_WinSize.height*0.95));
		this->addChild(m_Test[0]);

		m_Test[1]=LabelTTF::create("","",16);
		m_Test[1]->setAnchorPoint(ccp(0,0));
		m_Test[1]->setColor(ccc3(255,255,255));
		m_Test[1]->setPosition(ccp(m_WinSize.width*0.6,m_WinSize.height*0.95));
		this->addChild(m_Test[1]);
#endif
#ifdef USE_PRINTSCORE

		m_Score[1]=LabelTTF::create("","",16);
		m_Score[1]->setAnchorPoint(ccp(0.5,0.5));
		m_Score[1]->setColor(ccc3(255,255,0));
		m_Score[1]->setPosition(ccp(m_WinSize.width*0.5,m_WinSize.height*0.3));
		this->addChild(m_Score[1]);

		m_Score[2]=LabelTTF::create("","",16);
		m_Score[2]->setAnchorPoint(ccp(0.5,0.5));
		m_Score[2]->setColor(ccc3(255,255,255));
		m_Score[2]->setPosition(ccp(m_WinSize.width*0.4,m_WinSize.height*0.6));
		this->addChild(m_Score[2]);

		m_Score[3]=LabelTTF::create("","",16);
		m_Score[3]->setAnchorPoint(ccp(0.5,0.5));
		m_Score[3]->setColor(ccc3(255,255,255));
		m_Score[3]->setPosition(ccp(m_WinSize.width*0.5,m_WinSize.height*0.6));
		this->addChild(m_Score[3]);

		m_Score[4]=LabelTTF::create("","",16);
		m_Score[4]->setAnchorPoint(ccp(0.5,0.5));
		m_Score[4]->setColor(ccc3(255,255,255));
		m_Score[4]->setPosition(ccp(m_WinSize.width*0.6,m_WinSize.height*0.6));
		this->addChild(m_Score[4]);

		//	隐藏调试信息
		m_Score[1]->setVisible(false);
		m_Score[2]->setVisible(false);
		m_Score[3]->setVisible(false);
		m_Score[4]->setVisible(false);

#endif

		Vec2 originPoint = Director::getInstance()->getVisibleOrigin();
		m_roomLight = RoomLight::create();
		m_roomLight->setTag(8);
		m_roomLight->setAnchorPoint(ccp(0, 0));
		m_roomLight->setPosition(ccp(0+originPoint.x,0+originPoint.y));
		addChild(m_roomLight);

		m_pChipPot = DZPKChipPot::create();
		addChild(m_pChipPot);
		//m_pChipPot->setPosition(Vec2(m_WinSize.width * 0.5, m_WinSize.height * 0.73));
		int platform=(myConfig.m_platform>=0 && myConfig.m_platform<MAX_PLATFORM_NUM?myConfig.m_platform:0);
		m_pChipPot->setPosition(Vec2(m_WinSize.width * myConfig.m_ChipPotPos[platform][0], m_WinSize.height * myConfig.m_ChipPotPos[platform][1]));

		m_pOperaLayer =DZPKOperaLayer::create();
		addChild(m_pOperaLayer, EDZPKLayerZOrder_Control);

		//m_pTrusteeship = Trusteeship::create();
		//addChild(m_pTrusteeship, EDZPKLayerZOrder_Trusteeship);

		Menu *pMenuClose=createExitMenu();
		this->addChild(pMenuClose, EDZPKLayerZOrder_Control);

		m_pCommunityCards = DZPKCommunityCards::create();
		addChild(m_pCommunityCards, EDZPKLayerZOrder_Card);
		m_pCommunityCards->setPosition(Vec2(m_WinSize.width * 0.5, m_WinSize.height * 0.55));

		initData();
		initPlayerView();
		AddObject();

		bRet = true;
	} while (0);
	return bRet;
}

void DZPKLayer::onEnter()
{
	Node::onEnter();
	//SoundControl::sharedSoundControl()->playBackGroundMusic(sound_name::s_bgMusic,true);
}

void DZPKLayer::onExit()
{
	Node::onExit();
	//SoundControl::sharedSoundControl()->coloseBackGroundMusic();
}

void DZPKLayer::updateTest(Object* obj)
{
	__Array *arr=(__Array *)obj;
	if(arr->count()>=3)
	{
		int iViewID=((Integer *)arr->getObjectAtIndex(0))->getValue();
		const char* szName=((__String *)arr->getObjectAtIndex(1))->getCString();
		int idx=((Integer *)arr->getObjectAtIndex(2))->getValue();
		m_mPlayerList[iViewID]->updateTest(szName,idx);
	}
}

void DZPKLayer::setScore1(LONGLONG lCellScore)
{
	string strScore1=String::createWithFormat("盲注:%lld/%lld",lCellScore,lCellScore*2)->getCString();
	MyNSString::GBKToUTF8(strScore1);
	NotificationCenter::getInstance()->postNotification("PrintScore1", String::create(strScore1));
}

void DZPKLayer::setScore2(LONGLONG lTurnMaxScore)
{
	string strScore=String::createWithFormat("最大下注:%lld",lTurnMaxScore)->getCString();
	MyNSString::GBKToUTF8(strScore);
	m_Score[2]->setString(strScore);
}

void DZPKLayer::setScore3(LONGLONG lTurnLessScore)
{
	string strScore=String::createWithFormat("最小下注:%lld",lTurnLessScore)->getCString();
	MyNSString::GBKToUTF8(strScore);
	m_Score[3]->setString(strScore);
}

void DZPKLayer::setScore4(LONGLONG lAddLessScore)
{
	string strScore=String::createWithFormat("加最小注:%lld",lAddLessScore)->getCString();
	MyNSString::GBKToUTF8(strScore);
	m_Score[4]->setString(strScore);
}

void DZPKLayer::onShutNetwork(Object* obj)
{
	PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(m_WinSize.width * 0.5, m_WinSize.height * 0.5),mPromptTypeGameServerShut);
	addChild(promptBox,EDZPKLayerZOrder_Dialog);
}

void DZPKLayer::onUpdateTable(Object* obj)
{
	tagUserBuffer* tag = (tagUserBuffer*)obj;
	DWORD uiUserID = tag->dwUserID;
	CCLOG("tagUserBuffer == %d",sizeof(tagUserBuffer));
	//获取人物信息
	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(uiUserID);
	if (!pUserItem){
		return ;
	} 
	tagUserData* pUserDate = pUserItem->getUserData();
	CCLOG("tagUserData == %d",sizeof(tagUserData));

	if (!pUserDate){
		return ;
	}

	upDatePlayer(*pUserDate);
	//游戏结束停止刷新玩家信息
}

//场景消息接收
void DZPKLayer::onMessageScene(Object* obj)
{
	tagSceneBuffer* tag = (tagSceneBuffer*)obj;
	onSceneMessage(tag->cbState,tag->dateBuffer,tag->nDataSize);
}

//场景消息处理
bool DZPKLayer::onSceneMessage(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize)
{
	CCLOG("DZPKLayer::onSceneMessage cbStation=%d,wDataSize=%d",cbStation,wDataSize);
	switch(cbStation)
	{
	case GAME_STATUS_FREE://0
		{
			CCLOG("GAME_STATUS_FREE 0");
			return OnGameSceneFree(pBuffer,wDataSize);
		}
	case GAME_STATUS_PLAY://100
		{
			CCLOG("GAME_STATUS_PLAY 100");
			return OnGameScenePlay(pBuffer,wDataSize);
		}
	default:
		{
			return false;
		}
	}
}

//游戏消息接收
void DZPKLayer::onMessageGame(Object* obj)
{
	tagGameBuffer* tag = (tagGameBuffer*)obj;
	int Applicead = tag->nDataSize;
	//CCLog("tagGameBuffer = %d",Applicead);
	onGameMessage(tag->wSubCmdID,tag->dateBuffer,tag->nDataSize);
}

//游戏消息处理
bool DZPKLayer::onGameMessage(unsigned short wSubCmdID, const void* pBuffer, unsigned short wDataSize)
{
	CCLOG("DZPKLayer game message: wSubCmdID=%d,wDataSize=%d",wSubCmdID,wDataSize);
	switch(wSubCmdID)
	{
	case SUB_S_GAME_START:    //游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//加注结果
		{
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:			//用户放弃 102
		{
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:         //发送扑克
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_CARD:		//用户开牌
		{
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	}
}

void DZPKLayer::onMessageMatchPlayer(Object* obj)
{
	/*MatchError* match = MatchError::MatchErrorWith();
	addChild(match,101);
	CCLOG("DZPKLayer: match player error");*/
}

void DZPKLayer::onLoginRoomFailed(Object* obj)
{
	CCLOG("DZPKLayer::onLoginRoomFailed");
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	tagRoomLogonFailObj* pObj = (tagRoomLogonFailObj*)obj;
	int DataSize=128;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
	if (getChildByName("DZPKLayer::onLoginRoomFailed"))
	{
		return;
	}
	PromptBox* pPromptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGetError);
	this->addChild(pPromptBox);
	pPromptBox->setPromptText(str);
	pPromptBox->setName("DZPKLayer::onLoginRoomFailed");
}

void DZPKLayer::onSitFaild(Object* obj)
{
	CCLOG("DZPKLayer::onSitFaild");
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	tagRequestFailureObj* pObj = (tagRequestFailureObj*)obj;
	int DataSize=256;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
	if (getChildByName("DZPKLayer::onSitFaild"))
	{
		return;
	}
	log("DZPKLayer::onSitFaild %d,%d,str=%s",pObj->cbFailureCode,pObj->cbCommandCode,str);
	PromptBox* pPromptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGetError);
	this->addChild(pPromptBox);
	pPromptBox->setPromptText(str);
	pPromptBox->setName("DZPKLayer::onSitFaild");
}

// 由于您长时间未进行操作，您已自动离开游戏！
void DZPKLayer::CloseRoundOpera(float t)
{
	MyConfig& myConfig=MyConfig::Instance();
	unschedule(schedule_selector(DZPKLayer::CloseRoundOpera));

	Scene* pScene = Director::getInstance()->getRunningScene();
	EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
	DialogTimeOutQuit* pTimeOut = DialogTimeOutQuit::create();
	pScene->addChild(pTimeOut, EDZPKLayerZOrder_Dialog);
}

void DZPKLayer::restartGame(float t)
{
	unschedule(schedule_selector(DZPKLayer::restartGame));
	dealPlayerReady();
	// 通知举手
	EntityMgr::instance()->getDispatch()->sendSocketDate(MDM_GF_FRAME,SUB_GF_USER_READY);
}

//物理椅子号->视图椅子号
unsigned int DZPKLayer::switchViewChairID(unsigned int nChairID)
{
	int nSelfChairID = EntityMgr::instance()->getDispatch()->m_wChairID;
	unsigned int nViewChairID = (nChairID+DZPKGAME_PLAYER-nSelfChairID);
	switch (DZPKGAME_PLAYER)
	{
	case 2: { nViewChairID+=1; break; }
	case 3: { nViewChairID+=1; break; }
	case 4: { nViewChairID+=2; break; }
	case 5: { nViewChairID+=2; break; }
	case 6: { nViewChairID+=3; break; }
	case 7: { nViewChairID+=3; break; }
	case 8: { nViewChairID+=3; break; }
	case 9: { nViewChairID+=4; break; }
	}
	return nViewChairID%DZPKGAME_PLAYER;
}

void DZPKLayer::studioUI(Node* pParent, Node* pChild)
{
	pParent->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(([=]{
		auto ws = Director::getInstance()->getWinSize();

		auto layer = LayerColor::create();
		Director::getInstance()->getRunningScene()->addChild(layer);

		auto sp = Sprite::create("dt_guanbibig.png");
		layer->addChild(sp);
		sp->setPosition(ws/2);
		layer->setName("studioUI");

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch* touch, Event* event){
			if(!pChild){
				return false;
			}
			auto target = event->getCurrentTarget();
			if(!target){
				return false;
			}
			Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
			cocos2d::Size s = target->getContentSize();
			cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);

			if (rect.containsPoint(locationInNode))
			{
				return true;
			}
			return false;
		};
		listener->onTouchMoved = [=](Touch* touch, Event* event){
			pChild->setPosition(pChild->getPosition() + touch->getDelta());
			Vec2 vPos = pChild->getPosition();
			cocos2d::Size sParentSize = pParent->getContentSize();
			float fPercentX = vPos.x / sParentSize.width;
			float fPercentY = vPos.y / sParentSize.height;
			log("pos  Vec2(%f,%f);", vPos.x, vPos.y);
			log("percent	(X = %f, Y = %f)", fPercentX, fPercentY);
		};
		listener->onTouchEnded = [=](Touch* touch, Event* event){
			pChild->setPosition(pChild->getPosition() + touch->getDelta());
			Vec2 vPos = pChild->getPosition();
			cocos2d::Size sParentSize = pParent->getContentSize();
			float fPercentX = vPos.x / sParentSize.width;
			float fPercentY = vPos.y / sParentSize.height;
			log("pos  Vec2(%f,%f);", vPos.x, vPos.y);
			log("percent	(X = %f, Y = %f)", fPercentX, fPercentY);
		};
		layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sp);

	})), nullptr));
}

void DZPKLayer::showDebugTips(string sText)
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	Scene* pScene = Director::getInstance()->getRunningScene();
	MyNSString::GBKToUTF8(sText);
	Label* pTips = Label::create();
	pScene->addChild(pTips);
	pTips->setPosition(Vec2(sVSize / 2));
	pTips->setSystemFontSize(30);
	pTips->setString(sText);
	auto pSeq = Sequence::create(
		MoveBy::create(3.0f, Vec2(0, 60)),
		DelayTime::create(2.0f),
		RemoveSelf::create(),
		nullptr);
	pTips->runAction(pSeq);
}

void DZPKLayer::initData()
{
	m_lCellScore = 0;
	m_lTurnLessScore = 0;
	m_lTurnMaxScore = 0;
	m_lAddLessScore = 0;
	m_lCenterScore = 0;
}

void DZPKLayer::initPlayerView()
{
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		UserDZPK* pPlayer = UserDZPK::create();
		addChild(pPlayer);
		pPlayer->m_vChipPotPosition = m_pChipPot->getPosition();
		pPlayer->setViewID((EDZPKPlayerViewID)i);
		m_mPlayerList[i] = pPlayer;
	}
}

void DZPKLayer::resetData()
{
	m_lCellScore = 0;
	m_lTurnLessScore = 0;
	m_lTurnMaxScore = 0;
	m_lAddLessScore = 0;
	m_lCenterScore = 0;
}

void DZPKLayer::upDatePlayer(tagUserData sData)
{
	DWORD dwSelfUserID = EntityMgr::instance()->roomFrame()->getMeUserDate()->dwUserID;
	if (sData.dwUserID == dwSelfUserID)
	{
		if (sData.lScore < 10)
		{
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANSLoginOtherSFaild);
		}
	}
	UserDZPK* pUser = getPlayer(sData.dwUserID);
	if (pUser)
	{
		pUser->setPlayerInfor(sData);
	}
	else
	{
		int nViewID = switchViewChairID(sData.wChairID);
		m_mPlayerList[nViewID]->setPlayerInfor(sData);
	}

}

void DZPKLayer::updatePlayerGold(WORD iViewID,LONGLONG lAddScore)
{
	LONGLONG score=m_mPlayerList[iViewID]->getPlayerGold();
	score -= lAddScore;
	score = score<0?0:score;
	m_mPlayerList[iViewID]->setPlayerGold(score);
}

void DZPKLayer::showDPlayer(int nViewID)
{
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		if (i == nViewID)
		{
			m_mPlayerList[i]->showDPlayer(true);
		}
		else
		{
			m_mPlayerList[i]->showDPlayer(false);
		}
	}
}

//	显示盲注玩家
void DZPKLayer::showBlindPlayer(EDZPKBlindType eType, int nViewID)
{
	m_mPlayerList[nViewID]->showBlind(eType);
}

//	隐藏盲注玩家
void DZPKLayer::hideBlindPlayer()
{
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		m_mPlayerList[i]->hideBlind();
	}
}

void DZPKLayer::onPlayerBehavior(int nViewID, int nType)
{
	m_mPlayerList[nViewID]->onBehavior((EDZPKPlayerBehaviorType)nType);
}

void DZPKLayer::cancelCountdown(int nViewID)
{
	m_mPlayerList[nViewID]->cancelCountdown();
}

//	下注
void DZPKLayer::dealBet(int nViewID, LONGLONG llValue)
{
	m_mPlayerList[nViewID]->m_pPlayerChip->bet(llValue);
	updatePlayerGold(nViewID, llValue);
}

//	汇总
void DZPKLayer::dealCollect(int nViewID)
{
	m_mPlayerList[nViewID]->m_pPlayerChip->collect();
}

//	分发
void DZPKLayer::dealDistribute(int nViewID, LONGLONG llValue)
{
	//return;
	m_mPlayerList[nViewID]->m_pPlayerChip->changeChip(llValue);
	m_mPlayerList[nViewID]->m_pPlayerChip->distribute(llValue);
	//筹码回收音效
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_dachouma);
}

UserDZPK* DZPKLayer::getPlayer(LONGLONG llUserID)
{
	UserDZPK* pUser = nullptr;
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		DWORD dwTemp = m_mPlayerList[i]->getPlayerData().dwUserID;
		if (dwTemp == llUserID)
		{
			pUser = m_mPlayerList[i];
		}
	}
	return pUser;
}

void DZPKLayer::aniSendCard()
{
	int nPlayerCount = 0;	//	本局玩家数
	int nPlayingList[DZPKGAME_PLAYER] = {-1};	//	玩家视觉ID列表
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		if (m_mPlayerList[i]->getPlayerData().cbUserStatus == US_PLAYING)
		{
			nPlayingList[nPlayerCount] = i;
			nPlayerCount++;
		}
	}
	float fDelay = 0;
	//	发牌动画实现逻辑
	for (int i = 0, j = 0; i < nPlayerCount; )
	{
		int nViewID = nPlayingList[i];

		//	筹码池位置作为卡牌的发牌起始位置
		Vec2 vStartPos = m_pChipPot->getPosition();
		//	玩家所在位置作为终点位置
		Vec2 vEndPos = m_mPlayerList[nViewID]->getPosition();

		DZPKCard* pAniCard = DZPKCard::create();
		addChild(pAniCard, EDZPKLayerZOrder_Card);
		pAniCard->setScale(0.5);
		pAniCard->setPosition(vStartPos);

		//	发牌动作
		auto pDelay = DelayTime::create(fDelay);
		auto pMove = MoveTo::create(0.5f, vEndPos);
		auto pCall = CallFunc::create([=]()
		{
			if (nViewID == EDZPKPlayerViewID_3)
			{
				m_mPlayerList[nViewID]->m_pHandCard->aniTurnCard(j);
			}
			else
			{
				m_mPlayerList[nViewID]->m_pHandCard->setCardVisible(j, true);
			}
		});
		auto pRemove = RemoveSelf::create();
		auto pSeq = Sequence::create(pDelay, pMove, pCall, pRemove, nullptr);
		
		pAniCard->runAction(pSeq);

		i++;
		fDelay += 0.3f;

		if (i == nPlayerCount && j < EDZPKCardID_2)
		{
			//	j表示手牌的索引，总共2张手牌
			j++;
			i = 0;
		}
	}
}

void DZPKLayer::showAllPlayerCard(char cHandCard[][2])
{
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		int nTmpViewID = switchViewChairID(i);
		if (cHandCard[i][EDZPKCardID_1] != 0 && cHandCard[i][EDZPKCardID_2] != 0)
		{
			if (m_mPlayerList[nTmpViewID]->getBehaviorState() == EDZPKPlayerBehaviorType_Fold)
			{
				//	弃牌玩家不显示
				continue;
			}
			m_mPlayerList[nTmpViewID]->m_pHandCard->setHandCardImg(EDZPKCardID_1, cHandCard[i][EDZPKCardID_1]);
			m_mPlayerList[nTmpViewID]->m_pHandCard->aniTurnCard(EDZPKCardID_1);

			m_mPlayerList[nTmpViewID]->m_pHandCard->setHandCardImg(EDZPKCardID_2, cHandCard[i][EDZPKCardID_2]);
			m_mPlayerList[nTmpViewID]->m_pHandCard->aniTurnCard(EDZPKCardID_2);

			m_mPlayerList[nTmpViewID]->m_pHandCard->setScale(DZPK_HAND_CARD_PUBLIC_SCALE);
		}
	}
}

void DZPKLayer::setAllCardDark(char cHandCard[][2])
{
	//	将所有手牌并变灰暗
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		int nTmpViewID = switchViewChairID(i);
		if (cHandCard[i][EDZPKCardID_1] != 0 && cHandCard[i][EDZPKCardID_2] != 0)
		{
			m_mPlayerList[nTmpViewID]->m_pHandCard->m_pImgHandCard[EDZPKCardID_1]->showCardDark();
			m_mPlayerList[nTmpViewID]->m_pHandCard->m_pImgHandCard[EDZPKCardID_2]->showCardDark();
		}
	}
	//	将公共牌变灰暗
	for (int i = 0; i < MAX_CENTERCOUNT; i++)
	{
		m_pCommunityCards->m_pImgComunityCard[i]->showCardDark();
	}
}

//	显示高亮卡牌
void DZPKLayer::showCardHighLight(int nViewID, char cLastCard[])
{
	char cHandCard1 = m_mPlayerList[nViewID]->m_pHandCard->getHandCardData(EDZPKCardID_1);
	char cHandCard2 = m_mPlayerList[nViewID]->m_pHandCard->getHandCardData(EDZPKCardID_2);
	char* pComCards = m_pCommunityCards->getComunityCardData();

	//	将赢牌变高亮
	for (int i = 0; i < MAX_CENTERCOUNT; i++)
	{
		if (cHandCard1 == cLastCard[i])
		{
			m_mPlayerList[nViewID]->m_pHandCard->m_pImgHandCard[EDZPKCardID_1]->showCardHighLight();
		}
		if (cHandCard2 == cLastCard[i])
		{
			m_mPlayerList[nViewID]->m_pHandCard->m_pImgHandCard[EDZPKCardID_2]->showCardHighLight();
		}
		for (int j = 0; j < MAX_CENTERCOUNT; j++)
		{
			if (pComCards[i] == cLastCard[j])
			{
				m_pCommunityCards->m_pImgComunityCard[i]->showCardHighLight();
			}
		}
	}
}

void DZPKLayer::dealPlayerReady()
{
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		m_mPlayerList[i]->dealReady();
	}
	m_pCommunityCards->resetCommunityCards();
}

//空闲状态<by hxh>
bool DZPKLayer::OnGameSceneFree(const void *pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_StatusFree_dzpk)) return false;
	CMD_S_StatusFree_dzpk* pStatusPlay = (CMD_S_StatusFree_dzpk*)pBuffer;
	setScore1(pStatusPlay->lCellMinScore);//by hxh
	NotificationCenter::getInstance()->postNotification("PrintTest", String::createWithFormat("DZPKLayer::OnGameSceneFree lCellMinScore=%lld,lCellMaxScore=%lld",pStatusPlay->lCellMinScore,pStatusPlay->lCellMaxScore));
	return true;
}

//游戏状态<by hxh>
bool DZPKLayer::OnGameScenePlay(const void *pBuffer, WORD wDataSize)
{
	CCLOG("sizeof(CMD_S_StatusPlay_dzpk)=%d,wDataSize=%d",sizeof(CMD_S_StatusPlay_dzpk),wDataSize);
	//效验数据
	if (wDataSize!=sizeof(CMD_S_StatusPlay_dzpk)) return false;
	CMD_S_StatusPlay_dzpk* pStatusPlay = (CMD_S_StatusPlay_dzpk*)pBuffer;
#if 1
	WORD myChairID = getMeChairID();   //获取椅子号
	unsigned int uiDwChairID = switchViewChairID(pStatusPlay->wDUser);
	unsigned int uiCurrViewChairID =switchViewChairID(pStatusPlay->wCurrentUser);
	string str=String::createWithFormat("myChairID=%d,wCurentUser=%d(uiViewChairID=%d),wDUser=%d(uiDwChairID=%d),lCenterScore=%lld,lCellScore=%lld,lTurnMaxScore=%lld,lTurnLessScore=%lld,lAddLessScore=%lld,m_iHandCardData[0]=%d(%s),m_iHandCardData[1]=%d(%s)",
		myChairID,
		pStatusPlay->wCurrentUser,
		uiCurrViewChairID,
		pStatusPlay->wDUser,
		uiDwChairID,
		pStatusPlay->lCenterScore,
		pStatusPlay->lCellScore,
		pStatusPlay->lTurnMaxScore,
		pStatusPlay->lTurnLessScore,
		pStatusPlay->lAddLessScore,
		pStatusPlay->cbHandCardData[myChairID],
		DZPKGetCardName(pStatusPlay->cbHandCardData[myChairID]),
		pStatusPlay->cbHandCardData[myChairID],
		DZPKGetCardName(pStatusPlay->cbHandCardData[myChairID]))->getCString();
	MyNSString::GBKToUTF8(str);
	CCLOG(str.c_str());
#endif
	EntityMgr::instance()->roomFrame()->setGameIsStart(true);
	EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_SIT);

	m_lAddLessScore = pStatusPlay->lAddLessScore;
	m_lCellScore	= pStatusPlay->lCellScore;
	m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;
	m_lTurnLessScore= pStatusPlay->lTurnLessScore;
	m_lCenterScore	= pStatusPlay->lCenterScore;
	BYTE m_lCenterScoregjfg = pStatusPlay->cbBalanceCount;
#ifdef SHOW_BET_XZ_BTN
	m_pOperaLayer->showBtn(DZPKOperaLayer::BET_XZ_BTN,true);
#endif
	m_pOperaLayer->showBtn(DZPKOperaLayer::CKPX_BTN,true);
	setScore2(pStatusPlay->lTurnMaxScore);//by hxh
	setScore3(pStatusPlay->lTurnLessScore);//by hxh
	setScore4(pStatusPlay->lAddLessScore);//by hxh
	if(m_lCenterScore != 0)
	{
		m_pChipPot->updatePot(m_lCenterScore);
		setScore1(pStatusPlay->lCellScore);//by hxh
	}
	//人物状态
	for(int iChairID = 0;iChairID<DZPKGAME_PLAYER;iChairID++)
	{
		unsigned short id = switchViewChairID(iChairID);
		m_mPlayerList[id]->updateTest("",LABTEST2_IDX);
		char cState = pStatusPlay->cbPlayStatus[iChairID];
		if (cState != 1)
		{
			continue;
		}
		m_mPlayerList[id]->setPlayerGold(pStatusPlay->lUserMaxScore[iChairID]);

		m_lCenterScore += pStatusPlay->lTableScore[iChairID];
		dealBet(id, pStatusPlay->lTableScore[iChairID]);
	}
	int nCardCount = 3 + ((int)pStatusPlay->cbBalanceCount - 1);
	BYTE cComCardData[5];
	memset(cComCardData, 0, sizeof(cComCardData));
	memcpy(cComCardData, pStatusPlay->cbCenterCardData, sizeof(BYTE) * nCardCount);

	m_pCommunityCards->showCommunityCard((char*)cComCardData);

	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		char cState = pStatusPlay->cbPlayStatus[i];
		int nViewID = switchViewChairID(i);
		if (cState != 1)
		{
			continue;
		}

		m_mPlayerList[nViewID]->m_pHandCard->setCardVisible(EDZPKCardID_1, true);
		m_mPlayerList[nViewID]->m_pHandCard->setCardVisible(EDZPKCardID_2, true);
	}
	OnActionThink(uiCurrViewChairID);
	return true;
}

//游戏开始
bool DZPKLayer::OnSubGameStart( const void * pBuffer, WORD wDataSize )
{
	CCLOG("wDataSize=%d,sizeof(CMD_S_GameStart_dzpk)=%d",wDataSize,sizeof(CMD_S_GameStart_dzpk));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_GameStart_dzpk));
	if (wDataSize!=sizeof(CMD_S_GameStart_dzpk)) return false;
	resetData();
	//牌局开始
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_b_ready);
	//变量定义
	CMD_S_GameStart_dzpk* pGameStart = (CMD_S_GameStart_dzpk*)pBuffer;
	WORD myChairID = getMeChairID();   //获取椅子号
	unsigned int uiDwChairID = switchViewChairID(pGameStart->wDUser);
	unsigned int uiMinwChairID = switchViewChairID(pGameStart->wMinChipInUser);//by hxh
	unsigned int uiViewChairID =switchViewChairID(pGameStart->wCurrentUser);
	unsigned int uiMaxwChairID =switchViewChairID(pGameStart->wMaxChipInUser);
#if 1	
	string str=String::createWithFormat("myChairID=%d,wCurentUser=%d(uiViewChairID=%d),wDUser=%d(uiDwChairID=%d),wMaxChipInUser=%d(uiMaxwChairID=%d),lCellScore=%lld,lTurnMaxScore=%lld,lTurnLessScore=%lld,lAddLessScore=%lld,m_iHandCardData[0]=%d(%s),m_iHandCardData[1]=%d(%s)",
		myChairID,
		pGameStart->wCurrentUser,
		uiViewChairID,
		pGameStart->wDUser,
		uiDwChairID,
		pGameStart->wMaxChipInUser,
		uiMaxwChairID,
		pGameStart->lCellScore,
		pGameStart->lTurnMaxScore,
		pGameStart->lTurnLessScore,
		pGameStart->lAddLessScore,
		pGameStart->cbCardData[myChairID][0],
		DZPKGetCardName(pGameStart->cbCardData[myChairID][0]),
		pGameStart->cbCardData[myChairID][1],
		DZPKGetCardName(pGameStart->cbCardData[myChairID][1]))->getCString();
	MyNSString::GBKToUTF8(str);
	CCLOG(str.c_str());
#endif	
	//设置变量
	m_lAddLessScore = pGameStart->lAddLessScore;
	m_lTurnLessScore = pGameStart->lTurnLessScore;
	m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	m_lCellScore = pGameStart->lCellScore;
	//人物状态
	for(int iChairID = 0;iChairID<DZPKGAME_PLAYER;iChairID++)
	{
		unsigned short id = switchViewChairID(iChairID);
		m_mPlayerList[id]->updateTest("",LABTEST2_IDX);
		if (pGameStart->cbPlayStatus[iChairID]==1)
		{
			m_mPlayerList[id]->setPlayerGold(pGameStart->lUserMaxScore[iChairID]);
		}
	}
	m_mPlayerList[EDZPKPlayerViewID_3]->setHandCard(EDZPKCardID_1, pGameStart->cbCardData[myChairID][0]);
	m_mPlayerList[EDZPKPlayerViewID_3]->setHandCard(EDZPKCardID_2, pGameStart->cbCardData[myChairID][1]);
	aniSendCard();
#ifdef SHOW_BET_XZ_BTN
	m_pOperaLayer->showBtn(DZPKOperaLayer::BET_XZ_BTN,true);
#endif
	m_pOperaLayer->showBtn(DZPKOperaLayer::CKPX_BTN,true);
	m_lCenterScore = m_lCellScore*3;

	dealBet(uiMinwChairID, m_lCellScore);
	dealBet(uiMaxwChairID, m_lCellScore * 2);
	setScore1(pGameStart->lCellScore);//by hxh
	setScore2(pGameStart->lTurnMaxScore);//by hxh
	setScore3(pGameStart->lTurnLessScore);//by hxh
	setScore4(pGameStart->lAddLessScore);//by hxh

	showDPlayer(uiDwChairID);
	showBlindPlayer(EDZPKBlindType_small, uiMinwChairID);
	showBlindPlayer(EDZPKBlindType_Big, uiMaxwChairID);

	m_pOperaLayer->m_pRaiseSlider->setMinRaiseValue(pGameStart->lAddLessScore);

	CCLOG("CMD_S_GameStart_dzpk uiMaxwChairID== MY_VIEW_CHAIR_ID");

	if (m_lCenterScore != 0)
	{
		m_pChipPot->updatePot(m_lCenterScore);
	}

	CCLOG("received SUB_S_GAME_START.");

	//倒计时组件
	if (pGameStart->wCurrentUser !=INVALID_CHAIR)
	{
		if(uiViewChairID == MY_VIEW_CHAIR_ID)
		{
			showOperaBtns();
		}
		OnActionThink(uiViewChairID);
	}

	return true;
}

/*
PC端下注类型按钮显示规则：
1、最小下注为0时，显示【过牌】；最小下注大于0时，隐藏【过牌】。
2、最小下注大于0且不等于剩余金币时，显示【跟注】；最小下注为0时，隐藏【跟注】。
3、加最小注小于或等于剩余金币时，显示【加注】；加最小注大于剩余金币时，隐藏【加注】；
4、【弃牌】、【梭哈】始终显示。
*/
void DZPKLayer::showOperaBtns()
{
	// 轮到我操作播放音效<add by hxh 20161011>
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_chupai);

	bool bShowGP=(m_lTurnLessScore == 0);
	m_pOperaLayer->showBtn(DZPKOperaLayer::CHECK_GP_BTN,bShowGP);
	bool bShowGZ=(m_lTurnLessScore > 0 && m_lTurnMaxScore > m_lTurnLessScore);
	m_pOperaLayer->showBtn(DZPKOperaLayer::CALL_GZ_BTN,bShowGZ);
	bool bShowJZ=(m_lAddLessScore <= m_lTurnMaxScore);
	m_pOperaLayer->showBtn(DZPKOperaLayer::RAISE_JZ_BTN,bShowJZ);
	m_pOperaLayer->showBtn(DZPKOperaLayer::FOLD_QP_BTN,true);
	m_pOperaLayer->showBtn(DZPKOperaLayer::ALLIN_QX_BTN,true);
}

//用户下注
bool DZPKLayer::OnSubAddScore( const void * pBuffer, WORD wDataSize )
{
	CCLOG("sizeof(CMD_S_AddScore)=%d,wDataSize=%d",sizeof(CMD_S_AddScore),wDataSize);
	//效验数据
	if(wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;
	WORD myChairID = getMeChairID();   //获取椅子号
	unsigned int uiAddScorewChairID = switchViewChairID(pAddScore->wAddScoreUser);
	unsigned int uiViewChairID =switchViewChairID(pAddScore->wCurrentUser);
#if 1	
	string str=String::createWithFormat("myChairID=%d,wCurentUser=%d(uiViewChairID=%d),wAddScoreUser=%d(uiAddScorewChairID=%d),lAddScoreCount=%lld,lTurnMaxScore=%lld,lTurnLessScore=%lld,lAddLessScore=%lld",
		myChairID,
		pAddScore->wCurrentUser,
		uiViewChairID,
		pAddScore->wAddScoreUser,
		uiAddScorewChairID,
		pAddScore->lAddScoreCount,
		pAddScore->lTurnMaxScore,pAddScore->lTurnLessScore,
		pAddScore->lAddLessScore)->getCString();
	MyNSString::GBKToUTF8(str);
	CCLOG(str.c_str());
#endif	
	m_lTurnLessScore = pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lCenterScore += pAddScore->lAddScoreCount;
	m_lAddLessScore = pAddScore->lAddLessScore;
	//加注用户
	if (pAddScore->lAddScoreCount > 0)
	{
		dealBet(uiAddScorewChairID, pAddScore->lAddScoreCount);
	}
	m_pOperaLayer->m_pRaiseSlider->setMinRaiseValue(pAddScore->lAddLessScore);
	onPlayerBehavior(uiAddScorewChairID, pAddScore->cbJudgeAction);
	hideBlindPlayer();

	if ((pAddScore->wCurrentUser !=INVALID_CHAIR))
	{
		if (uiViewChairID == MY_VIEW_CHAIR_ID)
		{ 
			showOperaBtns();
		}
		OnActionThink(uiViewChairID);
	}
	return true;
}

void DZPKLayer::OnActionThink(int iViewID)
{
	if (m_roomLight)
	{
		m_roomLight->showLight();
		m_roomLight->LightAnimate(iViewID);
	}

	m_mPlayerList[iViewID]->showCountdown();
}

//用户放弃
bool DZPKLayer::OnSubGiveUp( const void * pBuffer, WORD wDataSize )
{
	CCLOG("sizeof(CMD_S_GiveUp)=%d,wDataSize=%d",sizeof(CMD_S_GiveUp),wDataSize);
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	int uiGiveUpwChairID=switchViewChairID(pGiveUp->wGiveUpUser);
	onPlayerBehavior(uiGiveUpwChairID, EDZPKPlayerBehaviorType_Fold);
	hideBlindPlayer();

	CCLOG("(uiGiveUpwChairID=%d)",uiGiveUpwChairID);
	NotificationCenter::getInstance()->postNotification("PrintTest1",
		String::createWithFormat("wGiveUpUser=%d(uiGiveUpwChairID=%d),lLost=lld%d",
		pGiveUp->wGiveUpUser,
		uiGiveUpwChairID,
		pGiveUp->lLost));

	return true;
}

//发牌消息
bool DZPKLayer::OnSubSendCard( const void * pBuffer, WORD wDataSize )
{
	CCLOG("sizeof(CMD_S_SendCard)=%d,wDataSize=%d",sizeof(CMD_S_SendCard),wDataSize);
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		dealCollect(i);
	}
	m_pChipPot->updatePot(m_lCenterScore);
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * cPublicPoker=(CMD_S_SendCard *)pBuffer;

	WORD  wCurrentUserDK =switchViewChairID(cPublicPoker->wCurrentUser);
	CCLOG("  ",cPublicPoker->cbCenterCardData);
	m_pCommunityCards->showCommunityCard((char*)cPublicPoker->cbCenterCardData);

	if (wCurrentUserDK != INVALID_CHAIR)
	{
		if ( wCurrentUserDK == MY_VIEW_CHAIR_ID)
		{
			m_pOperaLayer->showDXGZJZBtns(2);//【放弃】、【梭哈】、【过牌】、【加注】
		}
		OnActionThink(wCurrentUserDK);
	}
	return true;
}

//游戏结束
bool DZPKLayer::OnSubGameEnd( const void * pBuffer, WORD wDataSize )
{
	
	//效验数据 
	CCLOG("sizeof(CMD_S_GameEnd_dzpk)=%d,wDataSize=%d",sizeof(CMD_S_GameEnd_dzpk),wDataSize);
	if (wDataSize!=sizeof(CMD_S_GameEnd_dzpk)) return false;
	CMD_S_GameEnd_dzpk * pGameEnd=(CMD_S_GameEnd_dzpk *)pBuffer;

	MyConfig& myConfig=MyConfig::Instance();
	WORD myChairID = getMeChairID();   //获取椅子号
	BYTE	cbTotalEndk = pGameEnd->cbTotalEnd;							//强退标志
	LONGLONG  lGameTaxk= pGameEnd->lGameTax[DZPKGAME_PLAYER];				//游戏税收
	LONGLONG  lGameScorek =pGameEnd->lGameScore[DZPKGAME_PLAYER];			//游戏得分
	BYTE	cbCardDataks =pGameEnd->cbCardData[DZPKGAME_PLAYER][DZPK_HAND_CARD_COUNT];	//用户扑克
	for (int i = 0; i < DZPKGAME_PLAYER; i++)
	{
		m_mPlayerList[i]->m_pPlayerChip->clearChip();
	}

	//玩家比牌，胜利动画，筹码回收
	//根据分数判断输赢
	vector<int> winlist;
	for (int wChairID = 0;wChairID < DZPKGAME_PLAYER;wChairID++)
	{
		int id = DZPKLayer::switchViewChairID(wChairID);
		LONGLONG lGameScore=pGameEnd->lGameScore[wChairID];
		if (m_mPlayerList[id]->getPlayerData().cbUserStatus == US_PLAYING) //判断游戏状态
		{
			if (m_mPlayerList[id]->getBehaviorState() == EDZPKPlayerBehaviorType_Fold)
			{
				continue;
			}
			if(lGameScore>=0)
			{
				winlist.push_back(wChairID);
			}
		}
	}
	bool my_fail=true;
	float delay=0;
	int winerNum=winlist.size();
	int winerPot=1;
	int winerViewid=3;
	char cHandCardData[8][2];
	memcpy(cHandCardData, pGameEnd->cbCardData, sizeof(pGameEnd->cbCardData));

	if (cbTotalEndk == 1)
	{
		showAllPlayerCard(cHandCardData);
		setAllCardDark(cHandCardData);
	}
		

	for (int i=0;i<winerNum;i++)
	{
		winerPot=1;

		for (int j=0;j<winerPot;j++)
		{
			//赢者视图id
			winerViewid=DZPKLayer::switchViewChairID(winlist[i]);
			if (cbTotalEndk != 1 && winerViewid != EDZPKPlayerViewID_3)
			{
				m_mPlayerList[winerViewid]->m_pHandCard->setCardVisible(EDZPKCardID_1, false);
				m_mPlayerList[winerViewid]->m_pHandCard->setCardVisible(EDZPKCardID_2, false);
			}
			//筹码回收,发放给胜利玩家
			//	auto pCall = CallFunc::create(CC_CALLBACK_0(DZPKLayer::dealDistribute, this, winerViewid, pGameEnd->lGameScore[winlist[i]]));
			//runAction(Sequence::create(DelayTime::create(delay+1.0f),pCall,NULL));

			char cLastCardData[5];
			memcpy(cLastCardData, pGameEnd->cbLastCenterCardData[winlist[i]], sizeof(pGameEnd->cbLastCenterCardData[winlist[i]]));
			showCardHighLight(winerViewid, cLastCardData);
			char cCardType = cbTotalEndk == 1 ? pGameEnd->cbLastCardKind[winlist[i]] : 0;
			//胜利动画
			winAnimate(winerViewid,cCardType);
		}
	}	

	for(int i= 0;i<DZPKGAME_PLAYER;i++)
	{	
		int iViewID = switchViewChairID(i);
		if (m_mPlayerList[iViewID]->getPlayerData().cbUserStatus == US_PLAYING) //判断游戏状态
		{
			if (!cbTotalEndk)
			{
				//	值为0时属于强制退出，即没走完正常流程，无有效牌型，不显示
				break;
			}
			if (m_mPlayerList[iViewID]->getBehaviorState() == EDZPKPlayerBehaviorType_Fold)
			{
				//	弃牌玩家不显示牌型
				continue;
			}
			//	牌型
			m_mPlayerList[iViewID]->showCardGroupType(pGameEnd->cbLastCardKind[i]);

			string str1=String::createWithFormat("%s,%s,%s,%s,%s",
				DZPKGetCardName(pGameEnd->cbLastCenterCardData[i][0]),
				DZPKGetCardName(pGameEnd->cbLastCenterCardData[i][1]),
				DZPKGetCardName(pGameEnd->cbLastCenterCardData[i][2]),
				DZPKGetCardName(pGameEnd->cbLastCenterCardData[i][3]),
				DZPKGetCardName(pGameEnd->cbLastCenterCardData[i][4]))->getCString();
			//string str=String::createWithFormat("椅子号=%d,最后扑克=%s,牌型=%d",
			//	i,
			//	str1.c_str(),
			//	pGameEnd->cbLastCardKind[i])->getCString();
			//MyNSString::GBKToUTF8(str);
			//__Array *arr=__Array::create();
			//arr->addObject(Integer::create(iViewID));
			//arr->addObject(String::create(str));
			//arr->addObject(Integer::create(LABTEST2_IDX));
			//NotificationCenter::getInstance()->postNotification("updateTest",arr);
		}
	}

	resetData();
	m_pChipPot->restPot();
	m_roomLight->reset();//灯初始化<by hxh>
	m_pOperaLayer->dismissDXBtns();
	//	取消倒计时，这会导致行为状态置空，需要放最后
	for(int i= 0;i<DZPKGAME_PLAYER;i++)
	{
		cancelCountdown(i);
	}

	//当处于比赛场的时候执行
	m_StartLeave=createStartLeaveMenu(); 
	addChild(m_StartLeave, EDZPKLayerZOrder_Control);
	//30秒没执行则返回大厅
	schedule(schedule_selector(DZPKLayer::CloseRoundOpera),myConfig.m_t1);
	CCLOG("received SUB_S_GAME_END.");
	return true;
}

//开牌消息
bool DZPKLayer::OnSubOpenCard( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;
	CMD_S_OpenCard * pOpenCard=(CMD_S_OpenCard *)pBuffer;
	return true;
}

void DZPKLayer::winAnimate(int iViewID,BYTE cardtype)
{
	int tag=300+iViewID;
	if (getChildByTag(tag) ||iViewID<0)
	{
		return;
	}

	MyConfig& myConfig=MyConfig::Instance();
	Vec2 pos=ccp(myConfig.m_headPos[iViewID][0],myConfig.m_headPos[iViewID][1]);

	static const char *szArr[9]={texture_name::s_room_win_label,
		texture_name::s_cardtype_yidui_2,
		texture_name::s_cardtype_liangdui_2,
		texture_name::s_cardtype_santiao_2,
		texture_name::s_cardtype_shunzi_2,
		texture_name::s_cardtype_tonghua_2,
		texture_name::s_cardtype_hulu_2,
		texture_name::s_cardtype_sitiao_2,
		texture_name::s_cardtype_tonghuashun_2};

	Sprite *wintag =Sprite::createWithSpriteFrame(spriteFrame((cardtype<1||cardtype>9)?szArr[0]:szArr[cardtype-1]));
	wintag->setAnchorPoint(ccp(0.5f,0.5f));
	wintag->setPosition(pos);
	wintag->setOpacity(0);
	wintag->setTag(tag);
	this->addChild(wintag,EDZPKLayerZOrder_Card);
	wintag->runAction(FadeIn::create(0.3f));
	wintag->runAction(Sequence::create(
		DelayTime::create(4.0f),
		CCFadeOut::create(1.0f),
		RemoveSelf::create(),
		NULL));

	CCParticleSystemQuad *emitter = CCParticleSystemQuad::create("dz_winer.plist");
	emitter->setPosition(pos);//设置发射粒子的位置
	emitter->setAutoRemoveOnFinish(true);
	addChild(emitter,10);
}

void DZPKLayer::LeaveGameResume( Object* obj )
{
	// 点击音效<add by hxh 20161011>
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_dianji);

	int tag = dynamic_cast<Node*>(obj)->getTag();
	switch (tag)
	{
	case StartTag:
		{
			m_StartLeave->setVisible(false);
			unschedule(schedule_selector(DZPKLayer::CloseRoundOpera));
			removeChild(m_StartLeave);
			schedule(schedule_selector(DZPKLayer::restartGame),0.1f);
		}
		break;	
	case LeaveTag:
		{
			m_StartLeave->setVisible(false);
			DZPKLayer::ExitDZPK();
		}
		break;	
	case ExitTag:
		{
			CCLOG("exit================================");
			DZPKLayer::ExitDZPK();
		}
		break;
	default:
		break;
	}
}

void DZPKLayer::CloseWinLose( float t )
{
	unschedule(schedule_selector(DZPKLayer::CloseWinLose));
	removeChildByTag(ApTage_03);
	removeChildByTag(ApTage_04);
}

void DZPKLayer::onUserStart(Object *pSender)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithRequestMyTableUserInfo(DZPKGAME_PLAYER);
	// 通知举手
	EntityMgr::instance()->getDispatch()->sendSocketDate(MDM_GF_FRAME,SUB_GF_USER_READY);
}

void DZPKLayer::onLoginConnectFaild( Object* obj )
{
	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if (!bRet)
	{
		PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(m_WinSize.width*0.5,m_WinSize.height * 0.5),mPromptTypeLoginFaild);
		addChild(promptBox);
	}
}

void DZPKLayer::OtherStarya( Object *obj )
{
	if (getChildByName("DZPKLayer::OtherStarya"))
	{
		return;
	}
	Scene* pScene = Director::getInstance()->getRunningScene();
	PromptBox* box = PromptBox::PromptBoxWith(Vec2(m_WinSize.width*0.5,m_WinSize.height * 0.5), mPromptTypeGetError);
	box->setPromptText(ConfigMgr::instance()->text("t7"));
	this->addChild(box, EDZPKLayerZOrder_Dialog);
	box->setName("DZPKLayer::OtherStarya");
}

// add by hxh 20160919
#ifdef USE_PRINTTEST
void DZPKLayer::PrintTest( Object *obj )
{
	m_Test[0]->setString(((String *)obj)->getCString());
	scheduleOnce(schedule_selector(DZPKLayer::PrintTest_callback_once),5);
}

void DZPKLayer::PrintTest_callback_once(float t)
{
	m_Test[0]->setString("");
}

void DZPKLayer::PrintTest1( Object *obj )
{
	m_Test[1]->setString(((String *)obj)->getCString());
	scheduleOnce(schedule_selector(DZPKLayer::PrintTest1_callback_once),5);
}

void DZPKLayer::PrintTest1_callback_once(float t)
{
	m_Test[1]->setString("");
}
#endif

// add by hxh 20160919
#ifdef USE_PRINTSCORE

void DZPKLayer::PrintScore1( Object *obj )
{
	m_Score[1]->setString(((String *)obj)->getCString());
	//scheduleOnce(schedule_selector(DZPKLayer::PrintScore1_callback_once),5);
}

void DZPKLayer::PrintScore1_callback_once(float t)
{
	m_Score[1]->setString("");
}
#endif




