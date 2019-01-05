#include "GameScene.h"
#include "VisibleRect.h"
#include <sys/stat.h>
#include <sys/types.h>


#include <math.h>
#include "RoomLayer.h"
#include "PopupLayer.h"

#include "CMD_Fish.h"
#include "LrbyClientEngine.h"
#include "CMD_GameServer.h"
#include "MovingLabelLayer.h"
#include "FishSettingLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;
using namespace NS_lrbyFish;

extern time_t  g_uPayTime;

#define LoadINGGAMERESOURCE 1111

//extern int g_nLoading;
//静态变量
CGameScene * CGameScene::m_pGameScene=NULL;					//对象指针

CGameScene::CGameScene(void)
{
	m_pGameScene = this;
	m_WaitDistribute=nullptr;
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&CGameScene::onGameSinkUpdate), "ongamesinkupdate", nullptr);
}

CGameScene::~CGameScene(void)
{
	m_pGameScene = NULL;
	NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene *CGameScene::createScene()
{
	//Scene *scene = Scene::create();
	auto *layer = CGameScene::create();
	//scene->addChild(layer);
	return layer;
}

bool CGameScene::init()
{
	if(!cocos2d::Scene::init())	return false;

	//加载loading animation
 	auto uiWidget = GUIReader::getInstance()->widgetFromJsonFile("709/gameLoadAnimation/loadAnimation_1.ExportJson"); 
 	this->addChild(uiWidget, 8, LoadINGGAMERESOURCE);
 
 	auto uiBg = dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget, "Image_loadBg"));
 	//uiBg->loadTexture("lobby_img_bg.png");
 	//uiBg->setScale(1.25f);
	uiBg->setVisible(false);
 
 	//滑动条
 	m_sldLoad = dynamic_cast<Slider*>(Helper::seekWidgetByName(uiWidget, "Slider_loading"));
 	m_sldLoad->addEventListenerSlider(this, SEL_SlidPercentChangedEvent(&CGameScene::onSliderCallBack));
	// 资源异步加载
	m_nNumOfLoad = 0;

	anycLoadPicture(0.1f);	

	return true;
}
void CGameScene::onGameSinkUpdate(cocos2d::Ref *pRef)
{
	
}

//加载资源回调函数
void CGameScene::loadingTextureCallBack(Texture2D* texture)
{
	++ m_nNumOfLoad;
	if((m_nNumOfLoad > 0) && (m_nNumOfLoad < 9))
	{
		
		if (m_nNumOfLoad <= 7)
		{
			string str = String::createWithFormat("709/fish/fish_image%d.plist", m_nNumOfLoad)->getCString();
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(str, texture);
			
		}
		else if(m_nNumOfLoad == 8)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("709/fish/lock_fish_flag.plist", texture);
		}
		m_sldLoad->setPercent(12 * m_nNumOfLoad);
		if(m_nNumOfLoad == 8)
		{
			this->StartGame();
			//移除登陆动画场景
			auto uiWidget = (Node*)this->getChildByTag(LoadINGGAMERESOURCE);
			if(nullptr != uiWidget)
			{
				uiWidget->removeFromParentAndCleanup(true);
			}
		}
	}
	
}

void CGameScene::onSliderCallBack(Ref* sender, cocos2d::ui::SliderEventType type)
{

}

void CGameScene::anycLoadPicture(float dt)
{
	for(int i = 1; i < 9; i ++)
	{
		string str = String::createWithFormat("709/fish/fish_image%d.png", i)->getCString();
		if (i == 8)
		{
			str = "709/fish/lock_fish_flag.png";
		}
		//string str = String::createWithFormat("fish/fish_image%d.png", i)->getCString();
		Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(CGameScene::loadingTextureCallBack, this));
	}

	//加载其它资源
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("face/face.plist","face/face.png");

}

void CGameScene::StartGame()
{
	//进游戏房间，
	//游戏界面
	LrbyClientEngine* pGameSink = LrbyClientEngine::create();
	pGameSink->InitGameView();
	this->addChild(pGameSink);
	this->scheduleOnce(schedule_selector(CGameScene::sendEnterGameMsg), 0.1f);
}

void CGameScene::MessageBox(const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag)
{
	PopupLayer* msgBox = PopupLayer::create("msgbox_bg.png");
	msgBox->setPosition(VisibleRect::leftTop());
	msgBox->setTitle("");
	msgBox->setContentText(text, 24, 50, 150);
	msgBox->setCallBackFunc(callBack);
	msgBox->addButton("msgbox_btn_ok_nor.png","msgbox_btn_ok_sel.png",tag);
	this->addChild(msgBox,99);
}

void CGameScene::sendEnterGameMsg(float dt)
{
	int serverId=EntityMgr::instance()->getDispatch()->m_dwGameID;
	bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_FISH,serverId);
	if (!ret)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("Mytext.xml","t27"), ccp(SCREEN_HEIGHT/2,SCREEN_WIDTH/2));
		this->addChild(layer,20);
		return;
	}
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
	EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(6);
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
	EntityMgr::instance()->getDispatch()->setStartType(true);
}
