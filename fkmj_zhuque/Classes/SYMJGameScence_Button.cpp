#include "SYMJGameScence.h"
#include "GameLib.h"

#include "CMD_SYMJ.h"
#include "SYMJPlayer.h"
#include "SYMJGameLogic.h"
#include "SYMJSoundFun.h"
#include "JniFun.h"

void SYMJGameScence::initButton()
{
	WidgetManager::addButtonCB("HNMJButton_Ready",this,button_selector(SYMJGameScence::HNMJButton_Ready));
	WidgetManager::addButtonCB("HNMJButton_GuoAction",this,button_selector(SYMJGameScence::HNMJButton_GuoAction));
	WidgetManager::addButtonCB("HNMJButton_GangAction",this,button_selector(SYMJGameScence::HNMJButton_GangAction));
	WidgetManager::addButtonCB("HNMJButton_GangAction_Card",this,button_selector(SYMJGameScence::HNMJButton_GangAction));
	WidgetManager::addButtonCB("HNMJButton_PengAction",this,button_selector(SYMJGameScence::HNMJButton_PengAction));
	WidgetManager::addButtonCB("HNMJButton_PengAction_Card",this,button_selector(SYMJGameScence::HNMJButton_PengAction));
	WidgetManager::addButtonCB("HNMJButton_ChiAction",this,button_selector(SYMJGameScence::HNMJButton_ChiAction));
	WidgetManager::addButtonCB("HNMJButton_ChiAction_Card",this,button_selector(SYMJGameScence::HNMJButton_ChiAction));
	WidgetManager::addButtonCB("HNMJButton_HuAction",this,button_selector(SYMJGameScence::HNMJButton_HuAction));
	WidgetManager::addButtonCB("HNMJButton_HuAction_Card",this,button_selector(SYMJGameScence::HNMJButton_HuAction));
	WidgetManager::addButtonCB("HNMJButton_XiaoHuAction",this,button_selector(SYMJGameScence::HNMJButton_XiaoHuAction));
	WidgetManager::addButtonCB("HNMJButton_GangCS",this,button_selector(SYMJGameScence::HNMJButton_GangCS));
	WidgetManager::addButtonCB("HNMJButton_GangCS_Card",this,button_selector(SYMJGameScence::HNMJButton_GangCS));
	WidgetManager::addButtonCB("HNMJButton_BuAction",this,button_selector(SYMJGameScence::HNMJButton_BuAction));
	WidgetManager::addButtonCB("HNMJButton_ChiCard0",this,button_selector(SYMJGameScence::HNMJButton_ChiCardList));
	WidgetManager::addButtonCB("HNMJButton_ChiCard1",this,button_selector(SYMJGameScence::HNMJButton_ChiCardList));
	WidgetManager::addButtonCB("HNMJButton_ChiCard2",this,button_selector(SYMJGameScence::HNMJButton_ChiCardList));
	WidgetManager::addButtonCB("HNMJButton_QuXiaoTuoGuan",this,button_selector(SYMJGameScence::HNMJButton_QuXiaoTuoGuan));

	WidgetManager::addButtonCB("HNMJButtonAction_ShowCard",this,button_selector(SYMJGameScence::HNMJButtonAction_ShowCard));
	WidgetManager::addButtonCB("HNMJButtonAction_ShowNext",this,button_selector(SYMJGameScence::HNMJButtonAction_ShowNext));
	
	WidgetManager::addButtonCB("HNMJButton_JieSuanShare",this,button_selector(SYMJGameScence::HNMJButton_JieSuanShare));
	WidgetManager::addButtonCB("HNMJButton_JieSuanStart",this,button_selector(SYMJGameScence::HNMJButton_JieSuanStart));
	WidgetManager::addButtonCB("HNMJButton_JieSuanClose",this,button_selector(SYMJGameScence::HNMJButton_JieSuanClose));

	WidgetManager::addButtonCBBegin("HNMJButton_Talk",this,button_selector(SYMJGameScence::HNMJButton_TalkBegin));
	WidgetManager::addButtonCB("HNMJButton_Talk",this,button_selector(SYMJGameScence::HNMJButton_TalkEnd));

	WidgetManager::addButtonCB("Button_Master",this,button_selector(SYMJGameScence::HNMJButton_Master));
	WidgetManager::addButtonCB("Button_MasterClose",this,button_selector(SYMJGameScence::Button_MasterClose));
	WidgetManager::addButtonCB("NHMJ_MASTER_LEFTCARD",this,button_selector(SYMJGameScence::NHMJ_MASTER_LEFTCARD));

	WidgetManager::addButtonCB("MasterZhaNiao1",this,button_selector(SYMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao2",this,button_selector(SYMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao3",this,button_selector(SYMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao4",this,button_selector(SYMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao5",this,button_selector(SYMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao6",this,button_selector(SYMJGameScence::MasterZhaNiao));

	WidgetManager::addButtonCB("Button_TalkDefine",this,button_selector(SYMJGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(SYMJGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(SYMJGameScence::HNMJButton_BiaoQing));

	WidgetManager::addButtonCB("Button_LiaoTian",this,button_selector(SYMJGameScence::Button_LiaoTian));

	WidgetManager::addButtonCB("HNMJButton_ChuiJu",this,button_selector(SYMJGameScence::HNMJButton_ChuiJu));
	WidgetManager::addButtonCB("HNMJButton_BuChui",this,button_selector(SYMJGameScence::HNMJButton_BuChui));
}
void SYMJGameScence::HNMJButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}


void SYMJGameScence::sendOperateCMD(int nCode,int nCard)
{
	WidgetFun::setVisible(this,"SelfActionNode",false);

	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=nCode;
	OperateCard.cbOperateCard=nCard;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	remmoveAutoAction();
}

void SYMJGameScence::HNMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (pUserInfo == NULL)
	{
		sendOperateCMD(WIK_NULL,0);
		return;
	}
	std::string kXiaoHu = WidgetFun::getUserInfoValue(pUserInfo,"IsXiaoHu");
	if (kXiaoHu == "XiaoHu")
	{
		WidgetFun::setVisible(this,"SelfActionNode",false);
		CMD_C_XiaoHu OperateCard;
		OperateCard.cbOperateCode=WIK_NULL;
		OperateCard.cbOperateCard=0;
		SendSocketData(SUB_C_XIAOHU,&OperateCard,sizeof(OperateCard));
	}
	else
	{
		sendOperateCMD(WIK_NULL,0);
	}
}
void SYMJGameScence::HNMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*)
{
	int nCard = m_pLocal->getGangCard(utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard")));
	sendOperateCMD(WIK_GANG,nCard);
}
void SYMJGameScence::HNMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*)
{
	sendOperateCMD(WIK_PENG,0);
}
void SYMJGameScence::HNMJButton_ChiAction(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	int nChiAction  = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Chi_Type"));
	BYTE nChiCard  = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Chi_Card"));

	int nChiCount =0;
	int nSendChiAction = 0;
	if (nChiAction & WIK_LEFT)
	{
		nChiCount++;
		nSendChiAction = WIK_LEFT;
	}
	if (nChiAction & WIK_CENTER)
	{
		nChiCount++;
		nSendChiAction = WIK_CENTER;
	}
	if (nChiAction & WIK_RIGHT)
	{
		nChiCount++;
		nSendChiAction = WIK_RIGHT;
	}
	
	if (nChiCount ==1)
	{	
		sendOperateCMD(nSendChiAction,0);
	}
	else if (nChiCount >=2)
	{
		cocos2d::Node* pCheckNode = WidgetFun::getChildWidget(this,"ChiCardBg");
		pCheckNode->setVisible(true);
		showChiCheckNode(pCheckNode,nChiAction,nChiCard,nChiCount);
		
	}
}

void SYMJGameScence::showChiCheckNode(cocos2d::Node* pCheckNode, BYTE cbAction,BYTE cbCard,int nChiCount)
{
	ASSERT(pCheckNode&&nChiCount>=2&&nChiCount<=3);
	std::string kImagicFront = WidgetFun::getWidgetUserInfo(pCheckNode,"ImagicFront");
	if (nChiCount==2)
	{
		cocos2d::Size kTowSize = utility::parseSize(WidgetFun::getWidgetUserInfo(pCheckNode,"TowSize"));
		WidgetFun::set9Size(pCheckNode,kTowSize);
		WidgetFun::setVisible(pCheckNode,"Chi_Card_Temp3",false);

		cocos2d::Node* pChi_Card_Temp1 = WidgetFun::getChildWidget(pCheckNode,"Chi_Card_Temp1");
		cocos2d::Node* pChi_Card_Temp2 = WidgetFun::getChildWidget(pCheckNode,"Chi_Card_Temp2");
		if (cbAction&WIK_LEFT && cbAction&WIK_CENTER)
		{
			WidgetFun::setWidgetUserInfo(pChi_Card_Temp1,"ChiIndex",utility::toString((int)WIK_LEFT));
			WidgetFun::setWidgetUserInfo(pChi_Card_Temp2,"ChiIndex",utility::toString((int)WIK_CENTER));

			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard0"),cbCard+1,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard1"),cbCard,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard2"),cbCard+2,kImagicFront);

			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard0"),cbCard-1,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard1"),cbCard,  kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard2"),cbCard+1,kImagicFront);
		}
		if (cbAction&WIK_CENTER && cbAction&WIK_RIGHT)
		{
			WidgetFun::setWidgetUserInfo(pChi_Card_Temp1,"ChiIndex",utility::toString((int)WIK_CENTER));
			WidgetFun::setWidgetUserInfo(pChi_Card_Temp2,"ChiIndex",utility::toString((int)WIK_RIGHT));

			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard0"),cbCard-1,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard1"),cbCard,  kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard2"),cbCard+1,kImagicFront);

			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard0"),cbCard-2,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard1"),cbCard,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard2"),cbCard-1,kImagicFront);

		}
		if (cbAction&WIK_LEFT && cbAction&WIK_RIGHT)
		{
			WidgetFun::setWidgetUserInfo(pChi_Card_Temp1,"ChiIndex",utility::toString((int)WIK_LEFT));
			WidgetFun::setWidgetUserInfo(pChi_Card_Temp2,"ChiIndex",utility::toString((int)WIK_RIGHT));

			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard0"),cbCard+1,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard1"),cbCard,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard2"),cbCard+2,kImagicFront);

			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard0"),cbCard-2,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard1"),cbCard,kImagicFront);
			setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard2"),cbCard-1,kImagicFront);
		}
	}
	else
	{
		cocos2d::Size kThreeSize = utility::parseSize(WidgetFun::getWidgetUserInfo(pCheckNode,"ThreeSize"));
		WidgetFun::set9Size(pCheckNode,kThreeSize);
		WidgetFun::setVisible(pCheckNode,"Chi_Card_Temp3",true);


		cocos2d::Node* pChi_Card_Temp1 = WidgetFun::getChildWidget(pCheckNode,"Chi_Card_Temp1");
		WidgetFun::setWidgetUserInfo(pChi_Card_Temp1,"ChiIndex",utility::toString((int)WIK_LEFT));

		cocos2d::Node* pChi_Card_Temp2 = WidgetFun::getChildWidget(pCheckNode,"Chi_Card_Temp2");
		WidgetFun::setWidgetUserInfo(pChi_Card_Temp2,"ChiIndex",utility::toString((int)WIK_CENTER));

		cocos2d::Node* pChi_Card_Temp3 = WidgetFun::getChildWidget(pCheckNode,"Chi_Card_Temp3");
		WidgetFun::setWidgetUserInfo(pChi_Card_Temp3,"ChiIndex",utility::toString((int)WIK_RIGHT));

		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard0"),cbCard+1,kImagicFront);
		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard1"),cbCard,kImagicFront);
		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp1,"HNMJButton_ChiCard2"),cbCard+2,kImagicFront);

		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard0"),cbCard-1,kImagicFront);
		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard1"),cbCard,  kImagicFront);
		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp2,"HNMJButton_ChiCard2"),cbCard+1,kImagicFront);

		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp3,"HNMJButton_ChiCard0"),cbCard-2,kImagicFront);
		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp3,"HNMJButton_ChiCard1"),cbCard,kImagicFront);
		setActionBtnCard(WidgetFun::getChildWidget(pChi_Card_Temp3,"HNMJButton_ChiCard2"),cbCard-1,kImagicFront);
	}
}

void SYMJGameScence::HNMJButton_ChiCardList(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::Node* pParent = ((cocos2d::Node*)pNode)->getParent();
	int nChiIndex = utility::parseInt(WidgetFun::getWidgetUserInfo(pParent,"ChiIndex"));
	sendOperateCMD(nChiIndex,0);
}
void SYMJGameScence::HNMJButton_QuXiaoTuoGuan(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_Trustee Trustee;
	Trustee.bTrustee=false;
	SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
}

void SYMJGameScence::HNMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	sendOperateCMD(WIK_CHI_HU,0);
}

void SYMJGameScence::HNMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	
}

void SYMJGameScence::HNMJButton_GangCS(cocos2d::Ref*,WidgetUserInfo*)
{
}
void SYMJGameScence::HNMJButton_BuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	int nCard = m_pLocal->getGangCard(utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard")));
	sendOperateCMD(WIK_CS_GANG,nCard);
}
void SYMJGameScence::HNMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}
void SYMJGameScence::HNMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
	}
	setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}

void SYMJGameScence::HNMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{
	Button_WeiXinImagic(NULL,NULL);
}
void SYMJGameScence::HNMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	HNMJButton_Ready(NULL,NULL);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}

void SYMJGameScence::HNMJButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);

}

void SYMJGameScence::HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
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
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SYMJGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void SYMJGameScence::endButtonTalk()
{
	HNMJButton_TalkEnd(NULL,NULL);
}
void SYMJGameScence::HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
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
void SYMJGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = SYMJSoundFun::getDefineSound(m_pLocal->GetGender(),utility::toString(iUserIdex));
	sendTalkDefine(m_pLocal->GetChairID(),utility::toString(kTxt,":",kTaskStr));

	WidgetFun::setVisible(this,"GameTalkPlane",false);
}
void SYMJGameScence::Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(m_pLocal->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}

void SYMJGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(m_pLocal->GetChairID(),kFile);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
}
void SYMJGameScence::Button_LiaoTian(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}

void SYMJGameScence::HNMJButton_ChuiJu(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_FANBEI kFanBei;
	kFanBei.cbFan = 2;
	SendSocketData(SUB_C_FANBEI,&kFanBei,sizeof(kFanBei));
	WidgetFun::setVisible(this,"GameFanBeiNode",false);
}
void SYMJGameScence::HNMJButton_BuChui(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_FANBEI kFanBei;
	kFanBei.cbFan = 1;
	SendSocketData(SUB_C_FANBEI,&kFanBei,sizeof(kFanBei));
	WidgetFun::setVisible(this,"GameFanBeiNode",false);
}