#include "NNGameScence.h"
#include "GameLib.h"

#include "CMD_Ox.h"
#include "NNPlayer.h"
#include "NNGameLogic.h"
#include "JniFun.h"

void NNGameScence::initButton()
{
	WidgetManager::addButtonCB("NNButton_Ready",this,button_selector(NNGameScence::NNButton_Ready));
	WidgetManager::addButtonCB("NNButton_QiangZhuang",this,button_selector(NNGameScence::NNButton_QiangZhuang));
	WidgetManager::addButtonCB("NNButton_BuQiang",this,button_selector(NNGameScence::NNButton_BuQiang));
	WidgetManager::addButtonCB("NNButton_Bet",this,button_selector(NNGameScence::NNButton_Bet));
	WidgetManager::addButtonCB("NNButton_YouNiu",this,button_selector(NNGameScence::NNButton_YouNiu));
	WidgetManager::addButtonCB("NNButton_MeiYou",this,button_selector(NNGameScence::NNButton_MeiYou));
	WidgetManager::addButtonCB("NNButton_SendCardEnd",this,button_selector(NNGameScence::NNButton_SendCardEnd));
	WidgetManager::addButtonCB("NNButton_CheakZhuangEnd",this,button_selector(NNGameScence::NNButton_CheakZhuangEnd));


	WidgetManager::addButtonCBBegin("HNMJButton_Talk",this,button_selector(NNGameScence::HNMJButton_TalkBegin));
	WidgetManager::addButtonCB("HNMJButton_Talk",this,button_selector(NNGameScence::HNMJButton_TalkEnd));
}
void NNGameScence::NNButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}
void NNGameScence::NNButton_QiangZhuang(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iScore = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Socre"));
	CMD_C_CallBanker kNetInfo;
	kNetInfo.bBanker = iScore;
	SendSocketData(SUB_C_CALL_BANKER,&kNetInfo,sizeof(kNetInfo));
}
void NNGameScence::NNButton_BuQiang(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_CallBanker kNetInfo;
	kNetInfo.bBanker = 0;
	SendSocketData(SUB_C_CALL_BANKER,&kNetInfo,sizeof(kNetInfo));
}
void NNGameScence::NNButton_Bet(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iScore = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Gold"));
	CMD_C_AddScore kNetInfo;
	kNetInfo.lScore = iScore;
	SendSocketData(SUB_C_ADD_SCORE,&kNetInfo,sizeof(kNetInfo));
	WidgetFun::setVisible(this,"BetStateNode",false);
}
void NNGameScence::NNButton_YouNiu(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OxCard kNetInfo;
	kNetInfo.bOX = 1;
	SendSocketData(SUB_C_OPEN_CARD,&kNetInfo,sizeof(kNetInfo));
}
void NNGameScence::NNButton_MeiYou(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OxCard kNetInfo;
	kNetInfo.bOX = 0;
	SendSocketData(SUB_C_OPEN_CARD,&kNetInfo,sizeof(kNetInfo));
}
void NNGameScence::NNButton_SendCardEnd(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->showClock(15);
	WidgetFun::setVisible(this,"QiangZhuangStateNode",true);
}
void NNGameScence::NNButton_CheakZhuangEnd(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i]->showQiangZhuangIcon(false);
		m_pPlayer[i]->showJiaoZhuang(false);
	}

	NNPlayer* pPlayer = getPlayerByChairID(m_cbBankerChairID);
	if (pPlayer)
	{
		pPlayer->runQiangZhuangAni(false);
		pPlayer->setZhuang();
	}

	if (isSelfInGame())
	{
		m_pLocal->showClock(15);
	}
	WidgetFun::setVisible(this,"QiangZhuangStateNode",false);
	WidgetFun::setVisible(this,"BetStateNode",!isSelfBanker());

}
void NNGameScence::HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
{
	if (WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		JniFun::stopSoundRecord();
		WidgetFun::setVisible(this,"TalkImagic",false);
		return;
	}
	SoundFun::Instance().PaseBackMusic();
	WidgetFun::setVisible(this,"TalkImagic",true);
	JniFun::startSoundRecord();
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NNGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void NNGameScence::endButtonTalk()
{
	HNMJButton_TalkEnd(NULL,NULL);
}
void NNGameScence::HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
{
	SoundFun::Instance().ResumeBackMusic();
	if (!WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		return;
	}
	int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(this,"TalkImagic","TimeID"));
	TimeManager::Instance().removeByID(iTimeID);
	WidgetFun::setVisible(this,"TalkImagic",false);
	std::string kFileName = JniFun::stopSoundRecord();
	sendTalkFile(m_pLocal->GetChairID(),kFileName);
}