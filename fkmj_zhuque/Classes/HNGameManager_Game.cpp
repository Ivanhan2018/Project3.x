#include "HNGameManager.h"

#include "HNScenceManager.h"
#include "HNHomeScence.h"

#include "HNMJGameScence.h"

void HNGameManager::initGame()
{
	unsigned int iRuleOption = 0;
	FvMask::Add(iRuleOption,_MASK_(HNMJGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1));
	new HNMJGameScence(HNMJGameScence::GAME_OPTION_TYPE_THJ,iRuleOption);
	HNScenceManager::Instance().addGameNode(HNMJGameScence::pInstance());
}
void HNGameManager::StartXZDD()
{
}
void HNGameManager::StartXZDDRecord(datastream& kDataStream)
{
	HNScenceManager::Instance().HideAll(NULL);
	HNMJGameScence::Instance().EnterScence();
	if (!HNMJGameScence::Instance().StartRecord(kDataStream))
	{
		HNScenceManager::Instance().InHomeScence();
		return;
	}
}
void HNGameManager::hideAll()
{
	HNMJGameScence::Instance().HideAll();
}
IClientKernelSink* HNGameManager::CreateGame(word wKindID)
{
	if (!mGameServerItem)
	{
		return NULL;
	}
	HNScenceManager::Instance().HideAll(NULL);
	if (HNMJGameScence::KIND_ID == wKindID)
	{
		if (mGameServerItem->m_GameServer.wNodeID == 100)
		{
			WidgetFun::setText(HNMJGameScence::pInstance(),"GoldRoomGameRule0",utility::getScriptString("HNGameType0"));
			HNMJGameScence::Instance().setOptionRule(0);
		}
		else if (mGameServerItem->m_GameServer.wNodeID == 200)
		{
			WidgetFun::setText(HNMJGameScence::pInstance(),"GoldRoomGameRule0",utility::getScriptString("HNGameType1"));
			HNMJGameScence::Instance().setOptionRule(0);
		}
		else
		{
			unsigned int iRuleOption = 0;
			FvMask::Add(iRuleOption,_MASK_(HNMJGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1));
			HNMJGameScence::Instance().setOptionRule(iRuleOption);
		}
		WidgetFun::setText(HNMJGameScence::pInstance(),"GoldRoomGameRule1",
			utility::getScriptString(utility::toString("HNGameTypeLevel",HNHomeScence::Instance().getGameLevel())));
		WidgetFun::setPlaceTextKey(HNMJGameScence::pInstance(),"GoldRoomGameRule2","HNGameTypeDiFen",
			utility::toString(mGameServerItem->m_GameServer.lCellScore));
		HNMJGameScence::Instance().EnterScence();
		return HNMJGameScence::pInstance();
	}
	return NULL;
}
void HNGameManager::loadGameBaseData(word wKindID)
{
	if (HNMJGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,HNMJGameScence::MAX_PLAYER,HNMJGameScence::VERSION_CLIENT,"Game");
	}

}