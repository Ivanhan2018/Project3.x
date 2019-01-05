#include "SGLYGameScene.h"
#include "SGLYDefine.h"
#include "CMD_GameServer.h"
#include "Define.h"
#include "EntityMgr.h"
#include "ToolKit.h"
#include "SGLYRes.h"
#include "SGLYManageAudio.h"
#include "DDZTimeCheckControl.h"
#include "SGLYPopupQuit.h"


SGLYGameScene::SGLYGameScene()
{
}

SGLYGameScene::~SGLYGameScene()
{
}

Scene* SGLYGameScene::createScene()
{
	Scene* pScene = Scene::create();
	SGLYGameScene* pGameScene = SGLYGameScene::create();
	pScene->addChild(pGameScene);
	pGameScene->setName("SGLYGameScene");

	return pScene;
}

SGLYGameScene* SGLYGameScene::getGameScene()
{
	Scene* pScene = Director::getInstance()->getRunningScene();
	if (!pScene)
	{
		return nullptr;
	}
	return dynamic_cast<SGLYGameScene*>(pScene->getChildByName("SGLYGameScene"));

// 	return dynamic_cast<SGLYGameScene*>(Director::getInstance()->getRunningScene());
}

// 	预加载资源
void SGLYGameScene::preLoadRes()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SGLY_RES_PLIST);
	SGLYManageAudio::getInstance()->preLoadAudio();
}

// 	清除缓存
void SGLYGameScene::clear()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(SGLY_RES_PLIST);
	SGLYManageAudio::getInstance()->clear();
}

// 	重置游戏数据
void SGLYGameScene::resetGameScene()
{
	m_pGameLayer->reset();
	m_pInfoCenter->reset();
	m_pClock->reset();
	stopAllActions();
}

bool SGLYGameScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		preLoadRes();
		initUI();

		DDZTimeCheckControl::getInstance()->resetTimes();

		auto pKeyboardlistener = EventListenerKeyboard::create();
		pKeyboardlistener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){	

			if(keycode == EventKeyboard::KeyCode::KEY_BACK)
			{
				event->stopPropagation();
				Scene* pScene = Director::getInstance()->getRunningScene();
				if (!pScene)
				{
					return;
				}
				SGLYPopupQuit* pPopupQuit = SGLYPopupQuit::create();
				pScene->addChild(pPopupQuit);
			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pKeyboardlistener, this);


		bRet = true;
	} while (0);

	return bRet;
}

void SGLYGameScene::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pGameLayer = SGLYGameLayer::create();
	addChild(m_pGameLayer);

	m_pHistory = SGLYHistory::create();
	addChild(m_pHistory);
	m_pHistory->setPosition(Vec2(sVSize.width * 0.555, sVSize.height * 0.805));

	m_pInfoCenter = SGLYInfoCenter::create();
	addChild(m_pInfoCenter);

	m_pClock = SGLYClock::create();
	addChild(m_pClock);
	m_pClock->setPosition(Vec2(sVSize.width * 0.5, sVSize.height * 0.42));
	m_pClock->setVisible(false);

	m_pPopupSupplyChip = SGLYPopupSupplyChip::create();
	addChild(m_pPopupSupplyChip);

	m_pPopupApplyList = SGLYPopupApplyList::create();
	addChild(m_pPopupApplyList);
	m_pPopupApplyList->setVisible(false);

	m_pMessageCenter = SGLYMessageCenter::create();
	addChild(m_pMessageCenter);
}

void SGLYGameScene::onEnter()
{
	Layer::onEnter();

	int serverId=EntityMgr::instance()->getDispatch()->m_dwGameID;
	bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_SGLY,serverId);
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
