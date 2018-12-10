#include "HNMJGameScence.h"
#include "GameLib.h"

#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"
#include "HNMJSoundFun.h"
#include "JniFun.h"

void HNMJGameScence::initButton()
{
	WidgetManager::addButtonCB("HNMJButton_Ready",this,button_selector(HNMJGameScence::HNMJButton_Ready));
	WidgetManager::addButtonCB("HNMJButton_GuoAction",this,button_selector(HNMJGameScence::HNMJButton_GuoAction));
	WidgetManager::addButtonCB("HNMJButton_GangAction",this,button_selector(HNMJGameScence::HNMJButton_GangAction));
	WidgetManager::addButtonCB("HNMJButton_GangAction_Card",this,button_selector(HNMJGameScence::HNMJButton_GangAction));
	WidgetManager::addButtonCB("HNMJButtonCS_GangCS",this,button_selector(HNMJGameScence::HNMJButtonCS_GangCS));
	WidgetManager::addButtonCB("HNMJButtonCS_BuAction",this,button_selector(HNMJGameScence::HNMJButtonCS_BuAction));
	WidgetManager::addButtonCB("HNMJButton_PengAction",this,button_selector(HNMJGameScence::HNMJButton_PengAction));
	WidgetManager::addButtonCB("HNMJButton_PengAction_Card",this,button_selector(HNMJGameScence::HNMJButton_PengAction));
	WidgetManager::addButtonCB("HNMJButton_ChiAction",this,button_selector(HNMJGameScence::HNMJButton_ChiAction));
	WidgetManager::addButtonCB("HNMJButton_ChiAction_Card",this,button_selector(HNMJGameScence::HNMJButton_ChiAction));
	WidgetManager::addButtonCB("HNMJButton_HuAction",this,button_selector(HNMJGameScence::HNMJButton_HuAction));
	WidgetManager::addButtonCB("HNMJButton_HuAction_Card",this,button_selector(HNMJGameScence::HNMJButton_HuAction));
	WidgetManager::addButtonCB("HNMJButton_XiaoHuAction",this,button_selector(HNMJGameScence::HNMJButton_XiaoHuAction));
	WidgetManager::addButtonCB("HNMJButton_GangCS",this,button_selector(HNMJGameScence::HNMJButton_GangCS));
	WidgetManager::addButtonCB("HNMJButton_GangCS_Card",this,button_selector(HNMJGameScence::HNMJButton_GangCS));
	WidgetManager::addButtonCB("HNMJButton_BuAction",this,button_selector(HNMJGameScence::HNMJButton_BuAction));
	WidgetManager::addButtonCB("HNMJButton_ChiCard0",this,button_selector(HNMJGameScence::HNMJButton_ChiCardList));
	WidgetManager::addButtonCB("HNMJButton_ChiCard1",this,button_selector(HNMJGameScence::HNMJButton_ChiCardList));
	WidgetManager::addButtonCB("HNMJButton_ChiCard2",this,button_selector(HNMJGameScence::HNMJButton_ChiCardList));
	WidgetManager::addButtonCB("HNMJButton_QuXiaoTuoGuan",this,button_selector(HNMJGameScence::HNMJButton_QuXiaoTuoGuan));
	WidgetManager::addButtonCB("HNMJButton_HaiDiYao",this,button_selector(HNMJGameScence::HNMJButton_HaiDiYao));
	WidgetManager::addButtonCB("HNMJButton_HaiDiGuo",this,button_selector(HNMJGameScence::HNMJButton_HaiDiGuo));
	WidgetManager::addButtonCB("HNMJButton_GangActionNode0",this,button_selector(HNMJGameScence::HNMJButton_GangActionNode));
	WidgetManager::addButtonCB("HNMJButton_GangActionNode1",this,button_selector(HNMJGameScence::HNMJButton_GangActionNode));
	
	WidgetManager::addButtonCB("HNMJButtonAction_ShowCard",this,button_selector(HNMJGameScence::HNMJButtonAction_ShowCard));
	WidgetManager::addButtonCB("HNMJButtonAction_ShowNext",this,button_selector(HNMJGameScence::HNMJButtonAction_ShowNext));
	
	WidgetManager::addButtonCB("HNMJButton_JieSuanShare",this,button_selector(HNMJGameScence::HNMJButton_JieSuanShare));
	WidgetManager::addButtonCB("HNMJButton_JieSuanStart",this,button_selector(HNMJGameScence::HNMJButton_JieSuanStart));
	WidgetManager::addButtonCB("HNMJButton_JieSuanClose",this,button_selector(HNMJGameScence::HNMJButton_JieSuanClose));

	WidgetManager::addButtonCBBegin("HNMJButton_Talk",this,button_selector(HNMJGameScence::HNMJButton_TalkBegin));
	WidgetManager::addButtonCB("HNMJButton_Talk",this,button_selector(HNMJGameScence::HNMJButton_TalkEnd));

	WidgetManager::addButtonCB("Button_Master",this,button_selector(HNMJGameScence::HNMJButton_Master));
	WidgetManager::addButtonCB("Button_MasterClose",this,button_selector(HNMJGameScence::Button_MasterClose));
	WidgetManager::addButtonCB("NHMJ_MASTER_LEFTCARD",this,button_selector(HNMJGameScence::NHMJ_MASTER_LEFTCARD));

	WidgetManager::addButtonCB("MasterZhaNiao1",this,button_selector(HNMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao2",this,button_selector(HNMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao3",this,button_selector(HNMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao4",this,button_selector(HNMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao5",this,button_selector(HNMJGameScence::MasterZhaNiao));
	WidgetManager::addButtonCB("MasterZhaNiao6",this,button_selector(HNMJGameScence::MasterZhaNiao));

	WidgetManager::addButtonCB("Button_TalkDefine",this,button_selector(HNMJGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(HNMJGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(HNMJGameScence::HNMJButton_BiaoQing));

	WidgetManager::addButtonCB("HNMJButton_PiaoBei",this,button_selector(HNMJGameScence::HNMJButton_PiaoBei));
}
void HNMJGameScence::HNMJButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}


void HNMJGameScence::sendOperateCMD(int nCode,int nCard,int iCSGangIdex)
{
	WidgetFun::setVisible(this,"SelfActionNode",false);

	CMD_C_OperateCard OperateCard;
	zeromemory(&OperateCard,sizeof(OperateCard));
	OperateCard.cbCSGangIdex = iCSGangIdex;
	OperateCard.cbOperateCode=nCode;
	OperateCard.cbOperateCard=nCard;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	remmoveAutoAction();
}

void HNMJGameScence::HNMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (pUserInfo == NULL)
	{
		sendOperateCMD(WIK_NULL,0,0);
		return;
	}
	int iCSGangIdex = WidgetFun::getUserInfoInt(pUserInfo,"CSGangIdex");
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
		sendOperateCMD(WIK_NULL,0,iCSGangIdex);
	}
}
void HNMJGameScence::HNMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iCSGangIdex = WidgetFun::getUserInfoInt(pUserInfo,"CSGangIdex");
	sendOperateCMD(WIK_PENG,0,iCSGangIdex);
}
void HNMJGameScence::HNMJButton_ChiAction(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	int iCSGangIdex = WidgetFun::getUserInfoInt(pUserInfo,"CSGangIdex");
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
		sendOperateCMD(nSendChiAction,0,iCSGangIdex);
	}
	else if (nChiCount >=2)
	{
		cocos2d::Node* pCheckNode = WidgetFun::getChildWidget((cocos2d::Node*)pNode,"ChiCardBg");
		pCheckNode->setVisible(true);
		showChiCheckNode(pCheckNode,nChiAction,nChiCard,nChiCount);
		
	}
}

void HNMJGameScence::showChiCheckNode(cocos2d::Node* pCheckNode, BYTE cbAction,BYTE cbCard,int nChiCount)
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

void HNMJGameScence::HNMJButton_ChiCardList(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::Node* pParent = ((cocos2d::Node*)pNode)->getParent();
	int nChiIndex = utility::parseInt(WidgetFun::getWidgetUserInfo(pParent,"ChiIndex"));
	int iCSGangIdex = utility::parseInt(WidgetFun::getWidgetUserInfo(pParent,"CSGangIdex"));
	sendOperateCMD(nChiIndex,0,iCSGangIdex);
}
void HNMJGameScence::HNMJButton_QuXiaoTuoGuan(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_Trustee Trustee;
	Trustee.bTrustee=false;
	SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
}
void HNMJGameScence::HNMJButton_HaiDiYao(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_Use_HaiDi kNetInfo;
	kNetInfo.cbUseHaiDi = 1;
	SendSocketData(SUB_C_USE_HAIDI,&kNetInfo,sizeof(kNetInfo));
	WidgetFun::setVisible(this,"GameHaiDiNode",false);
}
void HNMJGameScence::HNMJButton_HaiDiGuo(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_Use_HaiDi kNetInfo;
	kNetInfo.cbUseHaiDi = 0;
	SendSocketData(SUB_C_USE_HAIDI,&kNetInfo,sizeof(kNetInfo));
	WidgetFun::setVisible(this,"GameHaiDiNode",false);
}

void HNMJGameScence::HNMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iCSGangIdex = 0;
	if (pUserInfo)
	{
		iCSGangIdex = WidgetFun::getUserInfoInt(pUserInfo,"CSGangIdex");
	}
	sendOperateCMD(WIK_CHI_HU,0,iCSGangIdex);
}

void HNMJGameScence::HNMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"SelfActionNode",false);
	CMD_C_XiaoHu OperateCard;
	OperateCard.cbOperateCode=WIK_XIAO_HU;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_XIAOHU,&OperateCard,sizeof(OperateCard));
}

void HNMJGameScence::HNMJButton_GangAction(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::Node* pNodeActionNode = WidgetFun::getChildWidget((cocos2d::Node*)pNode,"GangActionNode");
	if (m_kcbGangCardCount > 1 && pNodeActionNode)
	{
		WidgetFun::setVisible((cocos2d::Node*)pNode,"GangActionNode",true);
	}
	else
	{
		sendOperateCMD(WIK_GANG,m_kcbGangCardData[0],0);
	}
}
void HNMJGameScence::HNMJButton_GangCS(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::Node* pNodeActionNode = WidgetFun::getChildWidget((cocos2d::Node*)pNode,"GangActionNode");
	if (m_kcbGangCardCount > 1 && pNodeActionNode)
	{
		WidgetFun::setVisible((cocos2d::Node*)pNode,"GangActionNode",true);
	}
	else
	{
		sendOperateCMD(WIK_CS_GANG,m_kcbGangCardData[0],0);
	}
}
void HNMJGameScence::HNMJButton_BuAction(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::Node* pNodeActionNode = WidgetFun::getChildWidget((cocos2d::Node*)pNode,"GangActionNode");
	if (m_kcbGangCardCount > 1 && pNodeActionNode)
	{
		WidgetFun::setVisible((cocos2d::Node*)pNode,"GangActionNode",true);
	}
	else
	{
		sendOperateCMD(WIK_BU_ZHANG,m_kcbGangCardData[0],0);
	}
}
void HNMJGameScence::HNMJButtonCS_GangCS(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iCSGangIdex = WidgetFun::getUserInfoInt(pUserInfo,"CSGangIdex");
	if (iCSGangIdex == 0)
	{
		sendOperateCMD(WIK_CS_GANG,m_kcbGangCardData[0],iCSGangIdex);
	}
	else
	{
		sendOperateCMD(WIK_CS_GANG,m_kcbGangCardData[1],iCSGangIdex);
	}

}
void HNMJGameScence::HNMJButtonCS_BuAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iCSGangIdex = WidgetFun::getUserInfoInt(pUserInfo,"CSGangIdex");
	if (iCSGangIdex == 0)
	{
		sendOperateCMD(WIK_BU_ZHANG,m_kcbGangCardData[0],iCSGangIdex);
	}
	else
	{
		sendOperateCMD(WIK_BU_ZHANG,m_kcbGangCardData[1],iCSGangIdex);
	}
}
void HNMJGameScence::HNMJButton_GangActionNode(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iAction = WidgetFun::getUserInfoInt(pUserInfo,"Action");
	int iCard = WidgetFun::getUserInfoInt(pUserInfo,"Card");
	if (iAction == 0 || iCard == 0)
	{
		NoticeMsg::Instance().ShowTopMsg("HNMJButton_GangActionNode Error");
		return;
	}
	sendOperateCMD(iAction,iCard,0);
}
void HNMJGameScence::HNMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}
void HNMJGameScence::HNMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
	}
	setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}

void HNMJGameScence::HNMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{
	Button_WeiXinImagic(NULL,NULL);
}
void HNMJGameScence::HNMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	HNMJButton_Ready(NULL,NULL);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}

void HNMJGameScence::HNMJButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);

}

void HNMJGameScence::HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
{
	if (WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		JniFun::stopSoundRecord();
		WidgetFun::setVisible(this,"TalkImagic",false);
		return;
	}
	SoundFun::Instance().PaseBackMusic();
	SoundFun::Instance().PaseEffectMusic();
	WidgetFun::setVisible(this,"TalkImagic",true);
	JniFun::startSoundRecord();
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::endButtonTalk,this),8.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void HNMJGameScence::endButtonTalk()
{
	HNMJButton_TalkEnd(NULL,NULL);
}
void HNMJGameScence::HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
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
void HNMJGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (!IsInGame())
	{
		return;
	}
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = HNMJSoundFun::getDefineSound(m_pLocal->GetGender(),utility::toString(iUserIdex));
	sendTalkDefine(m_pLocal->GetChairID(),utility::toString(kTxt,":",kTaskStr));
   
    WidgetFun::setVisible(this,"GameTalkPlane",false);
}
void HNMJGameScence::Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
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
void HNMJGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (!IsInGame())
	{
		return;
	}
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(m_pLocal->GetChairID(),kFile);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
}

void HNMJGameScence::HNMJButton_PiaoBei(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int nIndex = WidgetFun::getUserInfoInt(pUserInfo,"Index");

	CMD_C_PiaoBei kInfo;
	kInfo.cbPiaoBei = nIndex;
	SendSocketData(SUB_C_PIAO_BEI,&kInfo,sizeof(kInfo));

	WidgetFun::setVisible(this,"GamePiaoNode",false);
}