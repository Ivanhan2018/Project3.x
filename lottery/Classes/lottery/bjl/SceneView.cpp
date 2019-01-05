#include "SceneView.h"
#include "DDZRes.h"
#include "RoomLayer.h"//by hxh
#include "BJLSceneControl.h"
#include "MovingLabelLayer.h"//by hxh
#include "MyNSString.h"//by hxh
#include "MyConfig.h"//by hxh
//#include "BankerListLayer.h"//by hxh
#include "GameTaskCommon.h"//by hxh

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "../proj.android/jni/hellocpp/Jni_system.h"
//#include "../../../../cocos2dx/platform/android/jni/JniHelper.h"  //verysharp
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JniHelper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppController.h"
#endif

SceneView::SceneView(void)
{
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMessage),MESSAGE_TOVIEW,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::receiveMenuClickCallfunc),MESSAGE_MENUCLICKED,NULL);//监听按钮点击消息
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnBankerListAdd),"OnBankerListAdd",NULL);//<by hxh>
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnBankerListDelete),"OnBankerListDelete",NULL);//<by hxh>
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeApplyBankerSuc),"OnMeApplyBankerSuc",NULL);//<by hxh>
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeUpBankerSuc),"OnMeUpBankerSuc",NULL);//<by hxh>
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeCancelUpBanker),"OnMeCancelUpBanker",NULL);//<by hxh>
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeDownBanker),"OnMeDownBanker",NULL);//<by hxh>
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::Strameobjet), "Strameobjet", NULL);
	// by hxh 20160704
#if 1
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::onLoginFaild), MSG_UI_ANS_ROOMLOGINFAIL, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::CloseNetWorkShut), MSG_UI_ANS_NETWORKSHUT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::onSitFaild), MSG_UI_ANS_SITFAIL, NULL);
#endif
	m_userOutCardState = 3 ;//3为普通出牌
	m_wBankerUser = INVALID_CHAIR;
}


SceneView::~SceneView(void)
{
	NotificationCenter::getInstance()->removeObserver(this, MESSAGE_TOVIEW);
	NotificationCenter::getInstance()->removeObserver(this,MESSAGE_MENUCLICKED);
	NotificationCenter::getInstance()->removeObserver(this,"OnBankerListAdd");//<by hxh>
	NotificationCenter::getInstance()->removeObserver(this,"OnBankerListDelete");//<by hxh>
	NotificationCenter::getInstance()->removeObserver(this,"OnMeApplyBankerSuc");//<by hxh>
	NotificationCenter::getInstance()->removeObserver(this,"OnMeUpBankerSuc");//<by hxh>
	NotificationCenter::getInstance()->removeObserver(this,"OnMeCancelUpBanker");//<by hxh>
	NotificationCenter::getInstance()->removeObserver(this,"OnMeDownBanker");//<by hxh>
	NotificationCenter::getInstance()->removeObserver(this, "Strameobjet");
	// by hxh 20160704
#if 1
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_ROOMLOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORKSHUT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_SITFAIL);
#endif
}

void SceneView::ToPortrait()
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

void SceneView::ToLandscape()
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

bool SceneView::init()
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	bool ret =false;
	do 
	{
		winSize = Director::getInstance()->getWinSize();//<by hxh>
		
		// 加载游戏场景资源到内存<by hxh>
		LoadRes();

		ScoreUseidbool = false;
		Sprite * background = Sprite::create(texture_name::s_background);
		background->setAnchorPoint(Vec2::ZERO);
		this->addChild(background);

		MyConfig& myConfig=MyConfig::Instance();

		MenuItemImage * CloseCardlayerItem = MenuItemImage::create("BJL_08.png","BJL_08_2.png",CC_CALLBACK_1(SceneView::SetCloseCall, this));
		Menu *CloseMen = Menu::create(CloseCardlayerItem,NULL);
		CloseMen->setAnchorPoint(Vec2::ZERO);
		//CloseMen->setPosition(30,size.height*0.9);
		CloseMen->setPosition(myConfig.m_ClosePos[0],myConfig.m_ClosePos[1]);
		this->addChild(CloseMen, 10);

		// add by hxh 20160708
#if 1
		// 申请上庄
		Sprite *NormalImage1 = Sprite::createWithSpriteFrame(spriteFrame("BJL_07.png"));
		Sprite *SelectedImage1 = Sprite::createWithSpriteFrame(spriteFrame("BJL_07_2.png"));
		// 取消申请
		Sprite *NormalImage1b = Sprite::createWithSpriteFrame(spriteFrame("BJL_07_3.png"));
		Sprite *SelectedImage1b = Sprite::createWithSpriteFrame(spriteFrame("BJL_07_4.png"));
		// 我要下庄
		Sprite *NormalImage1c = Sprite::createWithSpriteFrame(spriteFrame("BJL_07_5.png"));
		Sprite *SelectedImage1c = Sprite::createWithSpriteFrame(spriteFrame("BJL_07_6.png"));
		// 上庄列表
		Sprite *NormalImage2 = Sprite::createWithSpriteFrame(spriteFrame("BJL_18.png"));
		Sprite *SelectedImage2 = Sprite::createWithSpriteFrame(spriteFrame("BJL_18_2.png"));
		Sprite *NormalImage2b = Sprite::createWithSpriteFrame(spriteFrame("BJL_18.png"));
		Sprite *SelectedImage2b = Sprite::createWithSpriteFrame(spriteFrame("BJL_18_2.png"));
		Sprite *NormalImage2c = Sprite::createWithSpriteFrame(spriteFrame("BJL_18.png"));
		Sprite *SelectedImage2c = Sprite::createWithSpriteFrame(spriteFrame("BJL_18_2.png"));
		MenuItemSprite * Item1 = MenuItemSprite::create(NormalImage1,SelectedImage1,CC_CALLBACK_1(SceneView::OnUpBanker,this));
		MenuItemSprite * Item1b = MenuItemSprite::create(NormalImage1b,SelectedImage1b,CC_CALLBACK_1(SceneView::OnCancelUpBanker,this));
		MenuItemSprite * Item1c = MenuItemSprite::create(NormalImage1c,SelectedImage1c,CC_CALLBACK_1(SceneView::OnDownBanker,this));
		MenuItemSprite * Item2 = MenuItemSprite::create(NormalImage2,SelectedImage2,CC_CALLBACK_1(SceneView::OnBankerList,this));
		MenuItemSprite * Item2b = MenuItemSprite::create(NormalImage2b,SelectedImage2b,CC_CALLBACK_1(SceneView::OnBankerList,this));
		MenuItemSprite * Item2c = MenuItemSprite::create(NormalImage2c,SelectedImage2c,CC_CALLBACK_1(SceneView::OnBankerList,this));
		m_Menu1 = Menu::create(Item1,Item2,NULL);
		//m_Menu1->setAnchorPoint(Vec2::ZERO);
		m_Menu1->setPosition(myConfig.m_UpBankerPos[0],myConfig.m_UpBankerPos[1]);
		m_Menu1->alignItemsHorizontallyWithPadding(myConfig.m_padding);
		this->addChild(m_Menu1, 10);

		m_Menu2 = Menu::create(Item1b,Item2b,NULL);
		//m_Menu2->setAnchorPoint(Vec2::ZERO);
		m_Menu2->setPosition(myConfig.m_UpBankerPos[0],myConfig.m_UpBankerPos[1]);
		m_Menu2->alignItemsHorizontallyWithPadding(myConfig.m_padding);
		this->addChild(m_Menu2, 10);

		m_Menu3 = Menu::create(Item1c,Item2c,NULL);
		//m_Menu3->setAnchorPoint(Vec2::ZERO);
		m_Menu3->setPosition(myConfig.m_UpBankerPos[0],myConfig.m_UpBankerPos[1]);
		m_Menu3->alignItemsHorizontallyWithPadding(myConfig.m_padding);
		this->addChild(m_Menu3, 10);
		ShowCurrentMenu(1);
#endif

		m_pSceneLogic = SceneLogic::create();	//--逻辑命令
		this->addChild(m_pSceneLogic);

		m_menuLayer = ViewMenuLayer::create();							//--时钟 标志
		this->addChild(m_menuLayer);

		m_gameTask=GameTaskCommon::create();					//任务表现
		this->addChild(m_gameTask);

		m_effectView =ViewCardEffect::create();						//--效果
		this->addChild(m_effectView);



		//m_settlementNode = Settlement::create();					//结算
		//m_settlementNode->setPosition(ccp(size.width/2,size.height/2+50.0f));
		//m_settlementNode->setVisible(false);
		//this->addChild(m_settlementNode);

		//m_face =VoiceAndFace::create();								//--表情
		//this->addChild(m_face);

		m_dialogLayer = Layer::create();
		this->addChild(m_dialogLayer);

		m_isAgency =false;												//--机器人
		m_isLandLord=true;												//--叫地主按钮和表现
		m_AgencyNum=0;

		m_gameStae =Game_null;
		m_timeKey=0;

		ret = true;
	} while (0);

	return ret;
}

void SceneView::onExit()
{
	Node::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("cardtable.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("gameNumber.plist");
}

void SceneView::LoadRes()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cardtable.plist","cardtable.png");// 新的筹码图片<by hxh 20160630>
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameNumber.plist","gameNumber.png");
}

void SceneView::SetCloseCall( Object *obj )
{
	if(m_pSceneLogic->GetGameState() == EBJLGameState_Idle)
	{
		SceneView::ToPortrait();
		Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_start,false));
		
		Scene* sen=Scene::create();
		RoomLayer* layer = RoomLayer::create();
		layer->automaticLogin(false);
		sen->addChild(layer);

		EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
		EntityMgr::instance()->getDispatch()->closeGameSocket();  //modify by hd
		EntityMgr::instance()->roomFrame()->reset();
		ConfigMgr::instance()->m_Config.m_isGameRun=false;
	}
	else
	{
		DialogLayer * dialog = DialogLayer::create(dialog_back);
		this->addChild(dialog,	10);
	}
}

void SceneView::OnUpBanker( Object *obj )
{
	BYTE m_cbStatus=EntityMgr::instance()->getDispatch()->m_cbStatus;
	WORD m_wChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
	if(m_wChairID!=INVALID_CHAIR && (m_cbStatus==US_PLAYING||m_cbStatus==US_SIT)){
	     EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_APPLY_BANKER);
	}
}


void SceneView::OnCancelUpBanker( Object *obj )
{
	BYTE m_cbStatus=EntityMgr::instance()->getDispatch()->m_cbStatus;
	WORD m_wChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
	if(m_wChairID!=INVALID_CHAIR && (m_cbStatus==US_PLAYING||m_cbStatus==US_SIT)){
	    EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CANCEL_BANKER);
	}
}

void SceneView::OnDownBanker( Object *obj )
{
	BYTE m_cbStatus=EntityMgr::instance()->getDispatch()->m_cbStatus;
	WORD m_wChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
	if(m_wChairID!=INVALID_CHAIR && (m_cbStatus==US_PLAYING||m_cbStatus==US_SIT)){
	    EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CANCEL_BANKER);
	}
}

void SceneView::OnBankerList( Object *obj )
{
	DWORD m_dwUserID=EntityMgr::instance()->getDispatch()->m_dwUserID;
	BYTE m_cbStatus=EntityMgr::instance()->getDispatch()->m_cbStatus;
	WORD m_wChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
	WORD m_wTableID=EntityMgr::instance()->getDispatch()->m_wTableID;
	NotificationCenter::getInstance()->postNotification("PrintTest2", String::createWithFormat("m_dwUserID=%d,m_wTableID=%d,m_wChairID=%d,m_cbStatus=%d",m_dwUserID,m_wTableID,m_wChairID,m_cbStatus));//<by hxh>
//	// 测试用	
//#ifdef USE_TEST_TABLEVIEW
//    vector<DWORD> v=m_gameTask->m_ApplyBankerList;
//	v.push_back(m_dwUserID);
//	m_gameTask->setData(v);
//#endif
	m_gameTask->runOutOrIn();
}

void SceneView::ShowCurrentMenu(int j)
{
	setCurrentMenu(j);
	int i=getCurrentMenu();
	if(i==1){
	   	m_Menu1->setVisible(true);
		m_Menu2->setVisible(false);
		m_Menu3->setVisible(false);
		return;
	}
	if(i==2){
		m_Menu1->setVisible(false);
		m_Menu2->setVisible(true);
		m_Menu3->setVisible(false);
	    return;
	}
	if(i==3){
		m_Menu1->setVisible(false);
		m_Menu2->setVisible(false);
		m_Menu3->setVisible(true);
		return;
	}
}

void SceneView::onBankrupt( Object * obj )
{
	/*
	tagBankruptObj* tag=(tagBankruptObj*)obj;
	if (tag->bBankrupt == false)	//第一次破产 送你5000金返回大厅
	{
	m_dialogLayer->removeAllChildren();
	DialogLayer * dialog = DialogLayer::create(dialog_poChanSongCoin);
	m_dialogLayer->addChild(dialog);

	}
	else //破产放回大厅
	{
	m_dialogLayer->removeAllChildren();
	DialogLayer * dialog = DialogLayer::create(dialog_poChan);
	m_dialogLayer->addChild(dialog);
	}
	*/
	Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_start,false));
}

void SceneView::OnMessage( Object* obj )
{
	Integer* _int=(Integer*)obj;
	switch(_int->getValue())
	{
	case msgToView_UserInfor:				//--用户星信息表现
		//logic->userInforDeal(_int);
		CCLOG("sfdgbfsdfbfd");
		break;
	case msgToView_addView:				//--游戏加倍表现
		addViewDeal(_int);
		break;
	case msgToView_backCard:				//--底牌变化
		onBackTaskDeal(_int);
		break;
	case msgToView_start:						//--准备明牌开始
		startGameDeal();
		break;
	case msgToView_startView:				//--明牌开始表现
		startGameViewDeal(_int);
		break;
	case msgToView_sendCard:				//--发牌表现
		sendCardDeal(_int);
		break;
	case msgToView_openCard:				//--明牌表现
		openCardDeal(_int);
		break;
	case msgToView_landLord:				//--抢地主表现
		landLordDeal(_int);
		break;
	case msgToView_time:						//--时钟变化
		timeConreolDral(_int);
		break;
	case 	msgToView_outCard:				//--出牌表现
		outCardDeal(_int);
		break;
	case 	msgToView_passCard:				//--过牌表现
		passCardDeal(_int);
		break;
	case msgToView_gameOver:				//--游戏结束表现
		gameOverDeal(_int);
		break;
	case msgToView_openCardNum:		//--手里明牌按钮倍数变化
		{
		}
		break;
	case msgToView_beiLv:					//--倍率变化表现
		{
			CCbeilv*_beiData=(CCbeilv*)_int;
			//	m_cardTableLayer->setMultiplyingPower(_beiData->m_base,_beiData->m_beilv);
			//	m_cardTableLayer->setDiFen(_beiData->m_base);
		}
		break;
	case msgToview_TimeCallBack:				//--时钟回调
		timeCallBackDeal(_int);
		break;
	case msgToView_OnLine:						//--在线表现
		{
			CConline* _onLineData =(CConline*)_int;
			//	m_cardTableLayer->setUserIsOnline(_onLineData->m_userID,_onLineData->m_isOnline);
		}
		break;
	case msgToView_Reconnect:					//--断线重连
		{
			//m_cardTableLayer->waitingMatching(false);
		}
		break;
	case msgToView_socketoff:					//--网络断开
		{
			if (m_dialogLayer->getChildrenCount()>0&&m_dialogLayer->getChildren().at(0))
			{
				DialogLayer * dialog = (DialogLayer *)m_dialogLayer->getChildren().at(0);
				if (dialog_dismissTable==dialog->m_type)
				{
					break;
				}
			}
			m_dialogLayer->removeAllChildren();
			DialogLayer * dialog = DialogLayer::create(dialog_hints);
			m_dialogLayer->addChild(dialog);

		}
		break;
	case 	msgToView_socketoffConnect:				//--自动重连
		{
			m_effectView->runEffect(Effect_LlinkOFF,1);
			schedule(schedule_selector(SceneView::onLlinkOFF),2.0f,1,2.0f);
		}
		break;
	case 	msgToView_VoiceAndFace:			//--语音变化
		{
			CCfaceAndVioce* _face=(CCfaceAndVioce*)_int;
			if(_face->m_Type == 2 && _face->m_KeyID < 0)
			{
				string voiceString = _face->voice;
				m_effectView->runVoice(voiceString, _face->m_UserID);
			}else
			{
				m_effectView->runVoiceAndFace(_face->m_Type,_face->m_KeyID,_face->m_UserID);
			}
		}
		break;
	case msgToview_LordView:					//--地主表现
		{
			CClandView* _landView=(CClandView*)_int;
			//	m_cardTableLayer->setLordIcon(_landView->m_landUserID);
		}
		break;
	case msgToView_SettmentUser:			//结算人物表现
		{
			//	m_settlementNode->setUserMessage(_int);
			//	m_effectView->runSettlementEffect(_int,g_tableConsume,m_cardTableLayer);

		}
		break;
	case msgToView_huafeidian:
		{
			CChuafeidian* _data=(CChuafeidian*)_int;
			m_effectView->runHfdEffect(_data);
		}
		break;
	case msgToView_Close:
		{
			m_dialogLayer->removeAllChildren();
			DialogLayer * dialog = DialogLayer::create(dialog_dismissTable);
			m_dialogLayer->addChild(dialog);
		}
		break;
	case msgToView_RoomLimit:
		{
			CCRoomLimit* _data=(CCRoomLimit*)_int;
			m_dialogLayer->removeAllChildren();
			dialog_type type=dialog_poChan;
			switch(_data->m_RoomLimitKey)
			{
			case 0:
				type=dialog_poChan;
				break;
			case 1:
				type=dialog_roomLimitMin;
				break;
			case 2:
				type=dialog_roomLimitMax;
				break;
			default:
				return;
			}
			DialogLayer * dialog = DialogLayer::create(type);
			m_dialogLayer->addChild(dialog);
		}
		break;
	}
}

void SceneView::startGameDeal()
{
	//	operlayer->showDXGZJZBtns();
}

void SceneView::startGameViewDeal( Integer* _data )
{
	CCopenStart* _openStart =(CCopenStart*)_data;
	m_effectView->runEffect(_openStart->m_bOpenCard?Effect_OpenStart:Effect_gameStart,_openStart->m_bCurrentUser);
}

void SceneView::onBackTaskDeal( Integer* _data )
{
	CCbackData* _backData=(CCbackData*)_data;
}

void SceneView::addViewDeal( Integer* _data )
{
	CCaddView* _addData=(CCaddView*)_data;
	switch(_addData->m_AddType)
	{
	case 1:	//--表现
		m_effectView->runEffect(Effect_jiaBei,_addData->m_addID);
		break;
	}
}

void SceneView::sendCardDeal( Integer* _data )
{
	//CCsendCard* _sendCardData=(CCsendCard*)_data;
	//if (_sendCardData->m_bEffect==true && _sendCardData->m_userID==1)		//--第一次发牌取消托管
	//{
	//	m_isAgency =false;
	//	m_effectView->runRobot(false);
	//}
	//((ViewHandCard*)m_handCardView)->setHardCard(_sendCardData->m_userID,_sendCardData->m_handCardData,
	//	_sendCardData->m_handCardNum,_sendCardData->m_bEffect,_sendCardData->m_bOpen);
	//if (_sendCardData->m_handCardNum==2||_sendCardData->m_handCardNum==1)
	//{
	//	m_effectView->runEffect(Effect_alarm,_sendCardData->m_userID);
	//	//SoundEffect::sharedSoundEffect()->playEffect(sound_name::s_warning,_sendCardData->m_userID);
	//}
	//m_menuLayer->notificationMenuBarAutoHints();
}

void SceneView::openCardDeal( Integer* _data )
{
	CCopenCard*_openCardData=(CCopenCard*)_data;
	m_effectView->runEffect(Effect_openCard,_openCardData->m_openID);
}

void SceneView::landLordDeal( Integer* _data )
{
	CClandLord* _landLordData = (CClandLord*) _data;
	EffectType type ;
	if (_landLordData->m_LandKey==1)
	{
		if (m_isLandLord)
		{
			type =Effect_NoLandLord;
		}
		else
		{
			type =Effect_NoRobLord;
		}
	}
	else if (_landLordData->m_LandKey==2)
	{
		if (m_isLandLord)
		{
			type =Effect_LandLord;
			m_isLandLord=false;
		}
		else
		{
			type =Effect_RobLord;
		}
	}else if (_landLordData->m_LandKey==3)
	{
		type =Effect_NoRobLord;
	}
	else if (_landLordData->m_LandKey==4)
	{
		type =Effect_RobLord;
	}

	m_effectView->runEffect(type,_landLordData->m_landID);
}

void SceneView::timeConreolDral( Integer* _data )
{
}

void SceneView::outCardDeal( Integer* _data )
{
	//	m_cardTableLayer->waitingMatching(false);//确保游戏中排队动画隐藏掉
}

void SceneView::passCardDeal( Integer* _data )
{
	CCpassCard* _passCard =(CCpassCard* )_data;
	m_effectView->runEffect(Effect_PassCard,_passCard->m_passUser);
}

void SceneView::gameOverDeal( Integer* _data )
{
}

void SceneView::timeCallBackDeal( Integer* _data )
{
}

void SceneView::receiveMenuClickCallfunc(Object * obj)
{
	Integer * integer = (Integer*)obj;
	switch(integer->getValue())
	{
	case menu_changeTable:	
		{
		}
		break;
	case menu_visibleStartMenuItem:	//明牌开始
		{
		}
		break;
	case menu_startMenuItem://普通开始
		{
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(magToLogc_gameStart));
		}
		break;
	case menu_rechargemenuItem://快速充值
		CCLOG("SceneView recive menu_rechargemenuItem click");
		NotificationCenter::getInstance()->postNotification(MESSAGE_JNICONTROL,Integer::create(MSG_JNI_Recharge));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		EntityMgr::instance()->getDispatch()->sendOrderInfo("tbf",2);
#endif
		break;
	case menu_outCardMenuItem://出牌
		CCLOG("SceneView recive menu_outCardMenuItem click");
		{
			CCLOG("ViewHandCard recive menu_outCardMenuItem click");
		}
		break;
	case menu_passOutCardMenuItem:	//不要
		{
		}
		break;
	case menu_hintMenuItem:	//提示
		{
		}
		break;
	case menu_jiaoDiZhuItem:	//叫地主
	case menu_qiangDiZhuItem://抢地主
		{

			CClandLord* landord =CClandLord::create(msgToLogc_LandLord);
			landord->m_isLand = true;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,landord);
		}
		break;
	case menu_buJiaoItem://不叫地主e
	case menu_passQiangDiZhuItem://不抢地主
		{
			CCLOG("ViewHandCard recive menu_passQiangDiZhuItem click");
			CClandLord* landord =CClandLord::create(msgToLogc_LandLord);
			landord->m_isLand = false;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,landord);
		}
		break;
	case menu_visibleCardItem://明牌
		{

		}
		break;
	case menu_doubleItem://加倍
		{

		}
		break;
	case menu_unDoubleItem://不加倍

		break;
	case menu_settlementDetial://结算详情
		{
			//if (m_settlementNode->isVisible())
			//{
			//	m_settlementNode->setVisible(false);
			//}
			//else
			//{
			//	m_settlementNode->setVisible(true);
			//}
		}
		break;
	case menu_again://再来一局
		{
			//m_settlementNode->setVisible(false);
			//CCLOG("SceneView recive menu_again click");
			//NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(msgToLogic_MoreGame));
		}
		break;
	case menu_MenuBarPullItem://菜单栏呼出拉环
		CCLOG("SceneView recive menu_MenuBarPullItem click");
		break;
	case menu_menuBarRobotItem://菜单栏托管按钮
		{
			CCLOG("SceneView recive menu_menuBarRobotItem click");
			//if(m_gameStae>=Game_Start)//达到抢地主的状态后才能开始进入托管
			//{
			m_isAgency = true;
			m_AgencyNum=0;
			m_effectView->runRobot(true);
			//schedule(schedule_selector(SceneView::timeAgencyDeal),2.0f);//--机器人代打
			//}
		}
		break;
	case menu_menuBarTalkItem://菜单栏聊天按钮
		{
			//if (m_gameStae>=Game_Start)
			//{
			//m_face->runOutOrIn();
			//}
		}
		break;
	case menu_menuBarOptionsItem://菜单栏设置按钮
		{
			CCLOG("SceneView recive menu_menuBarOptionsItem click");
			SetUpLayer * setUpLayer = SetUpLayer::create();	//设置
			m_dialogLayer->addChild(setUpLayer);

		}
		break;
	case menu_menuBarTaskItem://菜单栏任务按钮
		{
			m_gameTask->runOutOrIn();
			m_effectView->runEffect(Effect_task,1);
		}
		break;
	case menu_menuBarBackItem://菜单栏返回按钮
		CCLOG("SceneView recive menu_menuBarBackItem click");
		{
			if(m_dialogLayer->getChildrenCount()>0)
			{
				m_dialogLayer->removeAllChildren();
			}else{
				DialogLayer * dialog = DialogLayer::create(dialog_back);
				m_dialogLayer->addChild(dialog);

			}
		}
		break;
	case menu_number:	//明牌开始按钮上的数字
		CCLOG("SceneView recive menu_number click");
		break;
	case menu_visibleCardStartItem:	//发牌过程中的明牌开始按钮
		{
		}
		break;
	case menu_OutAgency:				//--取消托管
		{
			m_isAgency =false;
		}
		break;
	}
}

void SceneView::timeAgencyDeal(float ft)
{
	unschedule(schedule_selector(SceneView::timeAgencyDeal));
	CCtimeCallBack* _timeCallBack=CCtimeCallBack::create(0);
	_timeCallBack->m_gameState =m_gameStae;
	_timeCallBack->m_timeKey =m_timeKey;
	timeCallBackDeal(_timeCallBack);  //--机器人代打
}

void SceneView::onLlinkOFF( float ft )
{
	unschedule(schedule_selector(SceneView::onLlinkOFF));
	BJLSceneControl::sharedSceneControl()->getScene(SCENE_endAgain,false);
}

void SceneView::onLoginFaild(Object* obj)
{
    CCLOG("SceneView::onLoginFaild");
    tagRoomLogonFailObj* pObj = (tagRoomLogonFailObj*)obj;
	int DataSize=128;//wcslen((wchar_t *)pObj->szDescribeString)*2+2;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
#if 1
    PromptBox* promptBox = PromptBox::PromptBoxWith(ccp(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
	promptBox->setPromptText(str);
	this->addChild(promptBox,1);
#else
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str, ccp(SCREEN_HEIGHT/2,SCREEN_WIDTH/2));
   this->addChild(layer,20);
#endif
}

void SceneView::onSitFaild(Object* obj)
{
    CCLOG("SceneView::onSitFaild");
    tagRequestFailureObj* pObj = (tagRequestFailureObj*)obj;
	int DataSize=256;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	PromptBox* box = PromptBox::PromptBoxWith(Vec2(sVSize.width*0.5,sVSize.height * 0.5), mPromptTypeGetError);
	box->setPromptText(ConfigMgr::instance()->text("t7"));
	this->addChild(box, 20);

/*
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str, ccp(SCREEN_HEIGHT/2,SCREEN_WIDTH/2));
    this->addChild(layer,20);
*/
}

void SceneView::CloseNetWorkShut(Object *obj)
{
	PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeGameServerShut);
	addChild(promptBox,1000);
}

void SceneView::OnBankerListAdd(Object *obj)
{
	WORD wChairID = ((Integer *)obj)->getValue();
	CUserItem* pUserItem=(CUserItem*)EntityMgr::instance()->roomFrame()->getUserItemByChairID(wChairID);
	if (!pUserItem)
	{
		return;
	}
// 	DWORD dwUserID=EntityMgr::instance()->getDispatch()->m_dwUserID;
	DWORD dwUserID = pUserItem->getUserID();
	bool bIsApply =m_gameTask->IsInApplyBankerList(dwUserID);
	if(!bIsApply)
	{
		vector<DWORD> v=m_gameTask->m_ApplyBankerList;
		v.push_back(dwUserID);
	    m_gameTask->setData(v);
	}
}

void SceneView::OnBankerListDelete(Object *obj)
{
	DWORD wChairID = ((Integer *)obj)->getValue();
	if(wChairID==INVALID_CHAIR)
	{
	   	vector<DWORD> v;
	    m_gameTask->setData(v);
		return;
	}
	CUserItem* pUserItem=(CUserItem*)EntityMgr::instance()->roomFrame()->getUserItemByChairID(wChairID);
	if (!pUserItem)
	{
		return;
	}
// 	DWORD dwUserID=EntityMgr::instance()->getDispatch()->m_dwUserID;
	DWORD dwUserID = pUserItem->getUserID();
	bool bIsApply =m_gameTask->IsInApplyBankerList(dwUserID);
	if(bIsApply)
	{
		vector<DWORD> v=m_gameTask->m_ApplyBankerList;
//#ifdef WIN32
#if 0
		v.erase(remove(v.begin(),v.end(),dwUserID),v.end());
#else
		//vector<DWORD>::iterator p=std::find(v.begin(),v.end(),dwUserID);
		vector<DWORD>::iterator p=v.begin();
		for(;p!=v.end();++p)
		{
			if(*p==dwUserID)
			{
				break;
			}
		}
		if(p!=v.end())
		{
			vector<DWORD>::iterator vIt =v.erase(p);
		}
#endif
	    m_gameTask->setData(v);
	}
}

void SceneView::OnMeApplyBankerSuc(Object *obj)
{
	ShowCurrentMenu(2);
}

void SceneView::OnMeUpBankerSuc(Object *obj)
{
	//((ViewHandCard*)m_handCardView)->newRound();
	ShowCurrentMenu(3);
}

void SceneView::OnMeCancelUpBanker(Object *obj)
{
	ShowCurrentMenu(1);
}

void SceneView::OnMeDownBanker(Object *obj)
{
	ShowCurrentMenu(1);
}

void SceneView::Strameobjet( Object *obj )
{
	int numbers = ((Integer *)obj)->getValue();
	m_menuLayer->setTimer(1,Game_Start,numbers,true,0);
}

void SceneView::StrameNumberbjet( Object *obj )
{
	int wBankerUser = ((Integer *)obj)->getValue();
	m_wBankerUser = wBankerUser;
}






