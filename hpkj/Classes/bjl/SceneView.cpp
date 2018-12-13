#include "SceneView.h"
#include "SceneLogic.h"
#include "BJLRes.h"
#include "RoomLayer.h"
#include "BJLSceneControl.h"
#include "MovingLabelLayer.h"
#include "MyNSString.h"
#include "MyConfig.h"
#include "UpBankerList.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "../proj.android/jni/hellocpp/Jni_system.h"
//#include "../../../../cocos2dx/platform/android/jni/JniHelper.h"  //verysharp
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JniHelper.h"
#endif

SceneView::SceneView(void)
{
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMessage),MESSAGE_TOVIEW,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::receiveMenuClickCallfunc),MESSAGE_MENUCLICKED,NULL);//监听按钮点击消息
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnBankerListAdd),"OnBankerListAdd",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnBankerListDelete),"OnBankerListDelete",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeApplyBankerSuc),"OnMeApplyBankerSuc",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeUpBankerSuc),"OnMeUpBankerSuc",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeCancelUpBanker),"OnMeCancelUpBanker",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneView::OnMeDownBanker),"OnMeDownBanker",NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::Strameobjet), "Strameobjet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::onLoginFaild), MSG_UI_ANS_ROOMLOGINFAIL, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::CloseNetWorkShut), MSG_UI_ANS_NETWORKSHUT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneView::onSitFaild), MSG_UI_ANS_SITFAIL, NULL);
	m_userOutCardState = 3 ;//3为普通出牌
	longNumberuserInforID = 0;
}


SceneView::~SceneView(void)
{
	NotificationCenter::getInstance()->removeObserver(this, MESSAGE_TOVIEW);
	NotificationCenter::getInstance()->removeObserver(this,MESSAGE_MENUCLICKED);
	NotificationCenter::getInstance()->removeObserver(this,"OnBankerListAdd");
	NotificationCenter::getInstance()->removeObserver(this,"OnBankerListDelete");
	NotificationCenter::getInstance()->removeObserver(this,"OnMeApplyBankerSuc");
	NotificationCenter::getInstance()->removeObserver(this,"OnMeUpBankerSuc");
	NotificationCenter::getInstance()->removeObserver(this,"OnMeCancelUpBanker");
	NotificationCenter::getInstance()->removeObserver(this,"OnMeDownBanker");
	NotificationCenter::getInstance()->removeObserver(this, "Strameobjet");
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_ROOMLOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORKSHUT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_SITFAIL);
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
}

void SceneView::ToLandscape()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	MyConfig::Instance().LoadData("xml/MyConfig.xml");
	GLView* eglView = Director::getInstance()->getOpenGLView();
	eglView->setViewName("Baccarat");
	eglView->setFrameSize(SCREEN_HEIGHT,SCREEN_WIDTH);
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
}

bool SceneView::init()
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	bool ret =false;
	do 
	{
		winSize = Director::getInstance()->getWinSize();
		
		// 加载游戏场景资源到内存
		LoadRes();

		ScoreUseidbool = false;
		Sprite * background = Sprite::create(texture_name::s_background);
		background->setAnchorPoint(Vec2::ZERO);
		this->addChild(background);

		MyConfig& myConfig=MyConfig::Instance();

		MenuItemImage * CloseCardlayerItem = MenuItemImage::create("bjl/BJL/BJL_08.png","bjl/BJL/BJL_08_2.png",CC_CALLBACK_1(SceneView::SetCloseCall, this));
		Menu *CloseMen = Menu::create(CloseCardlayerItem,NULL);
		CloseMen->setAnchorPoint(Vec2::ZERO);
		CloseMen->setPosition(myConfig.m_ClosePos[0],myConfig.m_ClosePos[1]);
		this->addChild(CloseMen, 10);

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
		m_Menu1->setPosition(myConfig.m_UpBankerPos[0],myConfig.m_UpBankerPos[1]);
		m_Menu1->alignItemsHorizontallyWithPadding(myConfig.m_padding);
		this->addChild(m_Menu1, 10);

		m_Menu2 = Menu::create(Item1b,Item2b,NULL);
		m_Menu2->setPosition(myConfig.m_UpBankerPos[0],myConfig.m_UpBankerPos[1]);
		m_Menu2->alignItemsHorizontallyWithPadding(myConfig.m_padding);
		this->addChild(m_Menu2, 10);

		m_Menu3 = Menu::create(Item1c,Item2c,NULL);
		m_Menu3->setPosition(myConfig.m_UpBankerPos[0],myConfig.m_UpBankerPos[1]);
		m_Menu3->alignItemsHorizontallyWithPadding(myConfig.m_padding);
		this->addChild(m_Menu3, 10);
		ShowCurrentMenu(1);

		SceneLogic *logic= SceneLogic::create();//--逻辑命令
		this->addChild(logic);

		m_menuLayer=ViewMenuLayer::create();//--时钟 标志
		this->addChild(m_menuLayer);

		m_UpBankerList=UpBankerList::create();//上庄列表
		this->addChild(m_UpBankerList);

		m_dialogLayer = Layer::create();
		this->addChild(m_dialogLayer);

		m_isAgency =false;												//--机器人
		m_isLandLord=true;												//--叫地主按钮和表现
		m_AgencyNum=0;

		m_gameStae =1;//1没开始
		m_timeKey=0;

		ret = true;
	} while (0);

	return ret;
}

void SceneView::LoadRes()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bjl/cardtable.plist","bjl/cardtable.png");// 新的筹码图片
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bjl/myCardtable.plist","bjl/myCardtable.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bjl/face.plist","bjl/face.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bjl/BaiJiaoItem.plist","bjl/BaiJiaoItem.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bjl/gameNumber.plist","bjl/gameNumber.png");
}

void SceneView::SetCloseCall( Object *obj )
{
	DialogLayer * dialog = DialogLayer::create(dialog_back);
	this->addChild(dialog,10);
}

void SceneView::OnUpBanker( Object *obj )
{
	BYTE m_cbStatus=pMsgDispatch->m_cbStatus;
	WORD m_wChairID=pMsgDispatch->m_wChairID;
	if(m_wChairID!=INVALID_CHAIR && m_cbStatus==US_PLAYING){
	     pMsgDispatch->sendSocketDate(SUB_C_APPLY_BANKER);
	}
}


void SceneView::OnCancelUpBanker( Object *obj )
{
	BYTE m_cbStatus=pMsgDispatch->m_cbStatus;
	WORD m_wChairID=pMsgDispatch->m_wChairID;
	if(m_wChairID!=INVALID_CHAIR && m_cbStatus==US_PLAYING){
	    pMsgDispatch->sendSocketDate(SUB_C_CANCEL_BANKER);
	}
}

void SceneView::OnDownBanker( Object *obj )
{
	BYTE m_cbStatus=pMsgDispatch->m_cbStatus;
	WORD m_wChairID=pMsgDispatch->m_wChairID;
	if(m_wChairID!=INVALID_CHAIR && m_cbStatus==US_PLAYING){
	    pMsgDispatch->sendSocketDate(SUB_C_CANCEL_BANKER);
	}
}

void SceneView::OnBankerList( Object *obj )
{
	DWORD m_dwUserID=pMsgDispatch->m_dwUserID;
	BYTE m_cbStatus=pMsgDispatch->m_cbStatus;
	WORD m_wChairID=pMsgDispatch->m_wChairID;
	WORD m_wTableID=pMsgDispatch->m_wTableID;
	NotificationCenter::getInstance()->postNotification("PrintTest2", String::createWithFormat("m_dwUserID=%d,m_wTableID=%d,m_wChairID=%d,m_cbStatus=%d",m_dwUserID,m_wTableID,m_wChairID,m_cbStatus));
	m_UpBankerList->runOutOrIn();
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
	Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_start,false));
}

void SceneView::OnMessage( Object* obj )
{
	Integer* _int=(Integer*)obj;
	switch(_int->getValue())
	{
	case msgToView_time:						//--时钟变化
		timeConreolDral(_int);
		break;
	case msgToView_TimeCallBack:				//--时钟回调
		timeCallBackDeal(_int);
		break;
	}
}

void SceneView::startGameDeal()
{
}

void SceneView::startGameViewDeal( Integer* _data )
{

}

void SceneView::onBackTaskDeal( Integer* _data )
{

}

void SceneView::addViewDeal( Integer* _data )
{

}

void SceneView::sendCardDeal( Integer* _data )
{

}

void SceneView::openCardDeal( Integer* _data )
{

}

void SceneView::landLordDeal( Integer* _data )
{

}

void SceneView::timeConreolDral( Integer* _data )
{
}

void SceneView::outCardDeal( Integer* _data )
{
}

void SceneView::passCardDeal( Integer* _data )
{

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

	}
}

void SceneView::timeAgencyDeal(float ft)
{

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
	int DataSize=wcslen((wchar_t *)pObj->szDescribeString)*2+2;
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
	int DataSize=wcslen((wchar_t *)pObj->szDescribeString)*2+2;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str, ccp(SCREEN_HEIGHT/2,SCREEN_WIDTH/2));
   this->addChild(layer,20);
}

void SceneView::CloseNetWorkShut(Object *obj)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_DuangDong.xml","t180"),
		ccp(winSize.width * 0.5,winSize.height * 0.5));
	this->addChild(layer, 255);
}

void SceneView::OnBankerListAdd(Object *obj)
{
	WORD wChairID = ((Integer *)obj)->getValue();
	CUserItem* pUserItem=(CUserItem*)EntityMgr::instance()->roomFrame()->getUserItemByChairID(wChairID);
	if (!pUserItem)
	{
		return;
	}
	DWORD dwUserID=pMsgDispatch->m_dwUserID;
	bool bIsApply =m_UpBankerList->IsInApplyBankerList(dwUserID);
	if(!bIsApply)
	{
		vector<DWORD> v=m_UpBankerList->m_ApplyBankerList;
		v.push_back(dwUserID);
	    m_UpBankerList->setData(v);
	}
}

void SceneView::OnBankerListDelete(Object *obj)
{
	DWORD wChairID = ((Integer *)obj)->getValue();
	if(wChairID==INVALID_CHAIR)
	{
	   	vector<DWORD> v;
	    m_UpBankerList->setData(v);
		return;
	}
	CUserItem* pUserItem=(CUserItem*)EntityMgr::instance()->roomFrame()->getUserItemByChairID(wChairID);
	if (!pUserItem)
	{
		return;
	}
	DWORD dwUserID=pMsgDispatch->m_dwUserID;
	bool bIsApply =m_UpBankerList->IsInApplyBankerList(dwUserID);
	if(bIsApply)
	{
		vector<DWORD> v=m_UpBankerList->m_ApplyBankerList;
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
	    m_UpBankerList->setData(v);
	}
}

void SceneView::OnMeApplyBankerSuc(Object *obj)
{
	ShowCurrentMenu(2);
}

void SceneView::OnMeUpBankerSuc(Object *obj)
{
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
	int NumberUsed = ((Integer *)obj)->getValue();
	longNumberuserInforID = NumberUsed;
}




