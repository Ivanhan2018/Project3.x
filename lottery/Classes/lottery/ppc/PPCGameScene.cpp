#include "PPCGameScene.h"
#include "configMgr.h"
#include "EntityMgr.h"
#include "CMD_GameServer.h"

PPCGameScene::PPCGameScene():
	m_GameStatue(EPPC_Null)
{
}

PPCGameScene::~PPCGameScene()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ppc_res.plist");
}

void PPCGameScene::resetGameScene()
{
	m_pPPCGameLayer->resetGameLayer();
	m_pPPCGameLayer->stopAllActions();
	m_pPPCControl->resetPPCControl();
	m_pPPCInfoBar->resetPPCInfoBar();
	m_pPPCEnding->setVisible(false);
}

bool PPCGameScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		addRes();
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void PPCGameScene::initUI()
{
	EntityMgr::instance()->getDispatch()->setGameStatus(true);

	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pPPCGameLayer = PPCGameLayer::create();
	addChild(m_pPPCGameLayer);

	m_pPPCControl = PPCControl::create();
	addChild(m_pPPCControl);

	m_pPPCInfoBar = PPCInfoBar::create();
	addChild(m_pPPCInfoBar);

	m_pPPCClock = PPCClock::create();
	addChild(m_pPPCClock);
	m_pPPCClock->setVisible(false);
	m_pPPCClock->setPosition(Vec2(sVSize.width * 0.05, sVSize.height * 0.25));

	m_pApplyListView = PPCPopupApplyList::create();
	addChild(m_pApplyListView);
	m_pApplyListView->setVisible(false);

	m_pPPCEnding = PPCEnding::create();
	addChild(m_pPPCEnding);
	m_pPPCEnding->setVisible(false);

	m_pPPCGameMessage = PPCGameMessage::create();
	addChild(m_pPPCGameMessage);
}

void PPCGameScene::onEnter()
{
	Scene::onEnter();
	int serverId=EntityMgr::instance()->getDispatch()->m_dwGameID;
	bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_PPC,serverId);
	if (!ret)
	{
		/*MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("Mytext.xml","t27"), ccp(SCREEN_WIDTH/2,SCREEN_WIDTH/2));
		this->addChild(layer,20);*/		
		log("xin:ccccccccccccccccccccccccccccccccccccc connect error");
		return;
	}
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
	EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(6);
	EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
	EntityMgr::instance()->getDispatch()->setStartType(true);
}

void PPCGameScene::addRes()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ppc_res.plist");
}
