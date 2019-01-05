#include "BRNNGameScene.h"
#include "configMgr.h"
#include "EntityMgr.h"
#include "CMD_GameServer.h"
#include "ToolKit.h""
#include "BRNNManageAudio.h"
#include "DDZTimeCheckControl.h"
#include "BRNNDialogQuit.h"

BRNNGameScene::BRNNGameScene():
	m_eGameStatue(E_BRNN_Null)
{
}

BRNNGameScene::~BRNNGameScene()
{
	clear();
}

BRNNGameScene* BRNNGameScene::getBRNNGameScene()
{
	return dynamic_cast<BRNNGameScene*>(Director::getInstance()->getRunningScene());
}

void BRNNGameScene::preLoadRes()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(BRNN_POKER_CARD_PLIST);	//	ÆË¿ËÅÆ
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(BRNN_RES_PLIST);
	BRNNManageAudio::getInstance()->preLoadAudio();
}

//	Çå³ý»º´æ
void BRNNGameScene::clear()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(BRNN_POKER_CARD_PLIST);
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(BRNN_RES_PLIST);
	BRNNManageAudio::getInstance()->clear();
}

void BRNNGameScene::resetGameScene()
{
	stopAllActions();
	BRNNManageAudio::getInstance()->stopBGMusic();
	BRNNManageAudio::getInstance()->stopAllSoundEffect();

	m_pSettlement->setVisible(false);
	m_pGameLayer->resetGameLayer();
	m_pInfoBar->resetInfoBar();
}

bool BRNNGameScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		preLoadRes();
		initUI();
		DDZTimeCheckControl::getInstance()->resetTimes();

		auto pKeyboardlistener = EventListenerKeyboard::create();
		pKeyboardlistener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){	

			if(keycode == EventKeyboard::KeyCode::KEY_BACK)
			{
				event->stopPropagation();
				BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
				if (!pScene)
				{
					return;
				}
				BRNNDialogQuit* pQuit = BRNNDialogQuit::create();
				if (pQuit)
				{
					pScene->addChild(pQuit);
				}
			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pKeyboardlistener, this);


		bRet = true;
	} while (0);

	return bRet;
}

void BRNNGameScene::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pGameLayer = BRNNGameLayer::create();
	addChild(m_pGameLayer);

	m_pInfoBar = BRNNInfoBar::create();
	addChild(m_pInfoBar);

	m_pClock = BRNNClock::create();
	addChild(m_pClock);
	m_pClock->setVisible(false);
	m_pClock->setPosition(Vec2(sVSize.width * 0.03, sVSize.height * 0.23));

	m_pSettlement = BRNNSettlement::create();
	addChild(m_pSettlement);
	m_pSettlement->setVisible(false);

	m_pPopupApplyList = BRNNPopupApplyList::create();
	addChild(m_pPopupApplyList);
	m_pPopupApplyList->setVisible(false);

	m_pPopupTravel = BRNNPopupTravel::create();
	addChild(m_pPopupTravel);
	m_pPopupTravel->setVisible(false);

	m_pPopupSetting = BRNNPopupSetting::create();
	addChild(m_pPopupSetting);
	m_pPopupSetting->setVisible(false);

	m_pGameMessage = BRNNGameMessage::create();
	addChild(m_pGameMessage);
}

void BRNNGameScene::onEnter()
{
	Scene::onEnter();
	int serverId=EntityMgr::instance()->getDispatch()->m_dwGameID;
	bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_PPC,serverId);
	if (!ret)
	{
		return;
	}
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
	EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(6);
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
	EntityMgr::instance()->getDispatch()->setStartType(true);
	EntityMgr::instance()->getDispatch()->setGameStatus(true);
}
