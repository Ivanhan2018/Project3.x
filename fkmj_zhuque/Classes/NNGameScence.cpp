#include "NNGameScence.h"
#include "NNPlayer.h"
#include "GameLib.h"
#include "CMD_Ox.h"
#include "NNPlayer.h"
#include "NNGameLogic.h"
#include "NNSoundFun.h"
#include "ScriptXMLparse.h"

FV_SINGLETON_STORAGE(NNGameScence);

NNGameScence::NNGameScence()
	:m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_cbBankerChairID(0)
	,GameBase(0,0)
{

	init();
}
NNGameScence::~NNGameScence()
{

}
bool NNGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("GameNN/xml/GameScence.xml",this);
	cocos2d::ScriptXMLparse kScriptXml1("GameNN/xml/ScriptValueStr.xml");

	initPrivate();

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new NNPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("NNButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	return true;
}
void NNGameScence::EnterScence()
{
	WidgetFun::setVisible(this,"GameHNNNPlane",true);
	if (WidgetFun::getChildWidget(this,"PrivateEndPlane"))
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	defaultState();
	NNSoundFun::playBackMusic(NNSoundFun::EX_BACKGRUAND);
}
bool NNGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameHNNNPlane");
}
void NNGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameHNNNPlane",false);
	defaultPrivateState();
}
void NNGameScence::defaultState()
{
	m_pTouchCardNode = NULL;
	m_kUpCardList.clear();
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_cbPlayStatus[i] = 0;
	}
	WidgetFun::setVisible(this,"QiangZhuangStateNode",false);
	WidgetFun::setVisible(this,"BetStateNode",false);

	showBoxNumNode(false);
	defaultPrivateState();
}
bool NNGameScence::isSelfBanker()
{
	return m_pLocal->GetChairID() == m_cbBankerChairID;
}
bool NNGameScence::isSelfInGame()
{
	return m_cbPlayStatus[m_pLocal->GetChairID()%GAME_PLAYER];
}

void NNGameScence::showBoxNumNode( bool bShow )
{
	WidgetFun::setVisible(this,"OpenCardStateNode",bShow);
	for (int i=0;i<4;i++)
	{
		WidgetFun::setText(this,utility::toString("BoxNum",i),"");
	}
	WidgetFun::setText(this,"ReroultText","");

	WidgetFun::setButtonEnabled(this,"NNButton_YouNiu",false);
	WidgetFun::setButtonEnabled(this,"NNButton_MeiYou",true);

}

void NNGameScence::checkZhuang(CMD_S_GameStart * pNetInfo)
{
	WidgetFun::setVisible(this,"QiangZhuangStateNode",false);
	for (int i=0;i<MAX_PLAYER;i++)
	{
		if (m_cbPlayStatus[i] == 0)
		{
			continue;
		}
		if (pNetInfo->bQiangPlayer[i])
		{
			getPlayerByChairID(i)->showQiangZhuangIcon(true);
			getPlayerByChairID(i)->runQiangZhuangAni(true);
		}
	}

}

void NNGameScence::showJieSuanInfo(LONGLONG lScore)
{
	WidgetFun::setVisible(this,"JieSuanNodeWin",false);
	WidgetFun::setVisible(this,"JieSuanNodeLose",false);
	if (lScore>=0)
	{
		WidgetFun::setVisible(this,"JieSuanNodeWin",true);
		WidgetFun::setAtlasTxt(this,"JieSuanGoldWin",utility::toString("+",int(lScore)));
		WidgetFun::runWidgetAction(this,"JieSuanNodeWin","ShowAni");
		NNSoundFun::playEffect("win.mp3");
	}
	else
	{
		WidgetFun::setVisible(this,"JieSuanNodeLose",true);
		WidgetFun::setAtlasTxt(this,"JieSuanGoldLose",utility::toString(int(lScore)));
		WidgetFun::runWidgetAction(this,"JieSuanNodeLose","ShowAni");
		NNSoundFun::playEffect("lose.mp3");
	}
}


