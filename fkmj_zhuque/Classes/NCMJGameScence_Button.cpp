#include "NCMJGameScence.h"
#include "GameLib.h"

#include "CMD_NCMJ.h"
#include "NCMJPlayer.h"
#include "NCMJGameLogic.h"
#include "JniFun.h"
#include "NCMJSoundFun.h"

void NCMJGameScence::initButton()
{
	WidgetManager::addButtonCB("NCMJButton_Ready",this,button_selector(NCMJGameScence::NCMJButton_Ready));
	WidgetManager::addButtonCB("NCMJButton_GuoAction",this,button_selector(NCMJGameScence::NCMJButton_GuoAction));
	WidgetManager::addButtonCB("NCMJButton_GangAction",this,button_selector(NCMJGameScence::NCMJButton_GangAction));
	WidgetManager::addButtonCB("NCMJButton_PengAction",this,button_selector(NCMJGameScence::NCMJButton_PengAction));
	WidgetManager::addButtonCB("NCMJButton_ChiAction",this,button_selector(NCMJGameScence::NCMJButton_ChiAction));
	WidgetManager::addButtonCB("NCMJButton_HuAction",this,button_selector(NCMJGameScence::NCMJButton_HuAction));
	WidgetManager::addButtonCB("NCMJButton_XiaoHuAction",this,button_selector(NCMJGameScence::NCMJButton_XiaoHuAction));

	WidgetManager::addButtonCB("NCMJButtonAction_ShowCard",this,button_selector(NCMJGameScence::NCMJButtonAction_ShowCard));
	WidgetManager::addButtonCB("NCMJButtonAction_ShowNext",this,button_selector(NCMJGameScence::NCMJButtonAction_ShowNext));

	WidgetManager::addButtonCB("NCMJButton_JieSuanShare",this,button_selector(NCMJGameScence::NCMJButton_JieSuanShare));
	WidgetManager::addButtonCB("NCMJButton_JieSuanStart",this,button_selector(NCMJGameScence::NCMJButton_JieSuanStart));

	WidgetManager::addButtonCBBegin("NCMJButton_TalkBegin",this,button_selector(NCMJGameScence::NCMJButton_TalkBegin));
	WidgetManager::addButtonCB("NCMJButton_TalkBegin",this,button_selector(NCMJGameScence::NCMJButton_TalkEnd));

	WidgetManager::addButtonCB("NCMJButton_Master",this,button_selector(NCMJGameScence::NCMJButton_Master));
	WidgetManager::addButtonCB("Button_MasterClose",this,button_selector(NCMJGameScence::Button_MasterClose));
	WidgetManager::addButtonCB("NCMJ_MASTER_LEFTCARD",this,button_selector(NCMJGameScence::NCMJ_MASTER_LEFTCARD));

	WidgetManager::addButtonCB("Button_TalkDefine",this,button_selector(NCMJGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(NCMJGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing0",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing1",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing2",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing3",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing4",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing5",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing6",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NCMJButton_BiaoQing7",this,button_selector(NCMJGameScence::HNMJButton_BiaoQing));
}
void NCMJGameScence::NCMJButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}
void NCMJGameScence::NCMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_NULL;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void NCMJGameScence::NCMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_GANG;
	OperateCard.cbOperateCard = m_pLocal->getGangCard(utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard")));
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void NCMJGameScence::NCMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_PENG;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void NCMJGameScence::NCMJButton_ChiAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	BYTE nChiAction  = (BYTE)utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Chi_Type"));
	CMD_C_OperateCard OperateCard;
	if (nChiAction & WIK_LEFT)
	{
		OperateCard.cbOperateCode=WIK_LEFT;
	}
	if (nChiAction & WIK_CENTER)
	{
		OperateCard.cbOperateCode=WIK_CENTER;
	}
	if (nChiAction & WIK_RIGHT)
	{
		OperateCard.cbOperateCode=WIK_RIGHT;
	}
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}

void NCMJGameScence::NCMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_CHI_HU;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}

void NCMJGameScence::NCMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_XiaoHu kXiaoHu;
	kXiaoHu.cbOperateCode=WIK_XIAO_HU;
	kXiaoHu.cbOperateCard=0;
	SendSocketData(SUB_C_XIAOHU,&kXiaoHu,sizeof(kXiaoHu));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}

void NCMJGameScence::NCMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}
void NCMJGameScence::NCMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
	}
	setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}

void NCMJGameScence::NCMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{
	NCMJButton_WeiXinImagic(NULL,NULL);
}
void NCMJGameScence::NCMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	NCMJButton_Ready(NULL,NULL);
}

void NCMJGameScence::NCMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
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
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NCMJGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void NCMJGameScence::endButtonTalk()
{
	NCMJButton_TalkEnd(NULL,NULL);
}
void NCMJGameScence::NCMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
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
void NCMJGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (!IsInGame())
	{
		return;
	}
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = NCMJSoundFun::getDefineSound(m_pLocal->GetGender(),utility::toString(iUserIdex));
	sendTalkDefine(m_pLocal->GetChairID(),utility::toString(kTxt,":",kTaskStr));

	WidgetFun::setVisible(this,"GameTalkPlane",false);
}
void NCMJGameScence::Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (!IsInGame())
	{
		return;
	}
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(m_pLocal->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}
void NCMJGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (!IsInGame())
	{
		return;
	}
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(m_pLocal->GetChairID(),kFile);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
}