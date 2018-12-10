#include "XZDDGameScence.h"
#include "XZDDGameLogic.h"
#include "GameLib.h"
#include "XZDDPlayer.h"
#include "MissionWeiXin.h"

void XZDDGameScence::initRecord()
{
	WidgetScenceXMLparse kScence("Script/HNGameRecordChild.xml",this);

	WidgetManager::addButtonCB("Button_GameRecordPlay",this,button_selector(XZDDGameScence::Button_GameRecordPlay));
	WidgetManager::addButtonCB("Button_GameRecordPase",this,button_selector(XZDDGameScence::Button_GameRecordPase));
	WidgetManager::addButtonCB("Button_GameRecordLeft",this,button_selector(XZDDGameScence::Button_GameRecordLeft));
	WidgetManager::addButtonCB("Button_GameRecordRight",this,button_selector(XZDDGameScence::Button_GameRecordRight));
	WidgetManager::addButtonCB("Button_GameRecordExit",this,button_selector(XZDDGameScence::Button_GameRecordExit));
}
void XZDDGameScence::defaultRecordState()
{
	if (WidgetFun::getChildWidget(this,"GameRecordControlPlane"))
	{
		WidgetFun::setVisible(this,"GameRecordControlPlane",false);
		WidgetFun::setVisible(this,"MenuNode",false);
	}
}
void XZDDGameScence::Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*)
{
	if (m_iActRecordIdex >= (int)m_pGameRecord->kAction.size())
	{
		WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
		WidgetFun::setVisible(this,"Button_GameRecordPase",false);
		return;
	}
	WidgetFun::setVisible(this,"Button_GameRecordPlay",false);
	WidgetFun::setVisible(this,"Button_GameRecordPase",true);
	TimeManager::Instance().removeByID(m_RecordTimeID);
	m_RecordTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::NextRecordAction,this),m_RecordTime)->iIdex;
}
void XZDDGameScence::Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
	WidgetFun::setVisible(this,"Button_GameRecordPase",false);
	TimeManager::Instance().removeByID(m_RecordTimeID);
}
void XZDDGameScence::Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime += 0.5f;
}
void XZDDGameScence::Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime -= 0.5f;
	if (m_RecordTime < 0.2f)
	{
		m_RecordTime = 0.5f;
	}
}
void XZDDGameScence::Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*)
{
	TimeManager::Instance().removeByID(m_RecordTimeID);
	ExitGame();
}
bool XZDDGameScence::StartRecord(datastream kDataStream)
{
	if (m_pGameRecord == NULL)
	{
		m_pGameRecord = new XZDDGameRecord;
		initRecord();
	}
	defaultState();
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"GameRecordControlPlane",true);
	WidgetFun::setText(this,"GameRecordPercentTxt","0%");
	*m_pGameRecord = XZDDGameRecord();
	WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
	WidgetFun::setVisible(this,"Button_GameRecordPase",false);
	m_iActRecordIdex = 0;
	m_RecordTime = 2.0f;
	m_pGameRecord->StreamValue(kDataStream,false);
	if (m_pGameRecord->kPlayers.size() != MAX_PLAYER)
	{
		ExitGame();
		return false;
	}	
	m_wRecordSelfChairID = 0;

	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		XZDDGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
		if (kRecordPlayer.dwUserID == UserInfo::Instance().getUserID())
		{
			m_wRecordSelfChairID = i;
		}
	}
	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		XZDDGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];

		int iChairID = (m_wRecordSelfChairID-i+MAX_PLAYER)%MAX_PLAYER;
		XZDDPlayer* pPlayer = m_pPlayer[iChairID];
		if (!pPlayer)
		{
			return false;
		}
		m_kReqPlayerInfo.query(kRecordPlayer.dwUserID);
		pPlayer->defaultState();
		pPlayer->setGameRecord(true);
		pPlayer->PlayerEnter();
		pPlayer->startGameNoTime();
		BYTE cbCardData[MAX_COUNT];
		for (int i = 0;i<MAX_COUNT;i++)
		{
			cbCardData[i] = kRecordPlayer.cbCardData[i];
		}
		pPlayer->setHandCard(cbCardData,13);
		pPlayer->showHandCard();
		pPlayer->showCard();
		WidgetFun::setText(pPlayer->getPlayerNode(),"Name",utility::a_u8(kRecordPlayer.kNickName));
		WidgetFun::setVisible(pPlayer->getPlayerNode(),"GoldImagic",false);
	}
	Button_GameRecordPlay(NULL,NULL);
	return true;
}
void XZDDGameScence::NextRecordAction()
{
	if (m_iActRecordIdex >= (int)m_pGameRecord->kAction.size() || !isVisible())
	{
		Button_GameRecordPase(NULL,NULL);
		WidgetFun::setText(this,"GameRecordPercentTxt","100%");
		return;
	}
	XZDDGameRecordOperateResult& kAction = m_pGameRecord->kAction[m_iActRecordIdex];
	int iChairID = (m_wRecordSelfChairID-kAction.wOperateUser+MAX_PLAYER)%MAX_PLAYER;
	int iProvideUser = (m_wRecordSelfChairID-kAction.wProvideUser+MAX_PLAYER)%MAX_PLAYER;
	if (kAction.cbActionType == XZDDGameRecordOperateResult::TYPE_OperateResult)
	{
		XZDDPlayer* pPlayer = m_pPlayer[iChairID];
		XZDD_CMD_S_OperateResult kTempCMD;
		kTempCMD.cbOperateCard = kAction.cbOperateCard;
		kTempCMD.cbOperateCode = kAction.cbOperateCode;
		kTempCMD.wOperateUser = kAction.wOperateUser;
		kTempCMD.wProvideUser = kAction.wProvideUser;
		XZDDPlayer* pProvidePlayer = m_pPlayer[iProvideUser];
		if (pProvidePlayer &&(kAction.cbOperateCode == WIK_PENG 
			|| kAction.cbOperateCode == WIK_LEFT
			|| kAction.cbOperateCode == WIK_CENTER
			||kAction.cbOperateCode == WIK_RIGHT
			|| (kAction.cbOperateCode == WIK_GANG && kAction.wOperateUser != kAction.wProvideUser )))
		{
			pProvidePlayer->removeHandOutCard(kAction.cbOperateCard);
			pProvidePlayer->setActOutCard(-1);
		}

		pPlayer->setOperateResoult(&kTempCMD);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == XZDDGameRecordOperateResult::TYPE_SendCard)
	{
		XZDDPlayer* pPlayer = m_pPlayer[iChairID];
		if (kAction.cbOperateCard != 0)
		{
			pPlayer->addNewInCard(kAction.cbOperateCard);
		}
		pPlayer->showCard();
	}
	if (kAction.cbActionType == XZDDGameRecordOperateResult::TYPE_OutCard)
	{
		XZDDPlayer* pPlayer = m_pPlayer[iChairID];
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			m_pPlayer[i]->setActOutCard(-1);
		}
		pPlayer->sendOutCard(kAction.cbOperateCard);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == XZDDGameRecordOperateResult::TYPE_ChiHu)
	{
		XZDDPlayer* pPlayer = m_pPlayer[iChairID];
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			m_pPlayer[i]->setActOutCard(-1);
		}

		pPlayer->setChiHuCard(kAction.cbOperateCard);
		pPlayer->showEffect("Hu");
		if (kAction.wOperateUser != kAction.wProvideUser)
		{
			XZDDPlayer* pDestPlayer = m_pPlayer[iChairID];
			pPlayer->showHandCard();
			pPlayer->showStatusImagic("Hu");

			pDestPlayer->runAniDianPao();
		}
		else
		{
			pPlayer->showStatusImagic("ZiMo");
			pPlayer->runAniZiMo();
		}
		pPlayer->showCard();
	}

	m_iActRecordIdex++;
	Button_GameRecordPlay(NULL,NULL);

	WidgetFun::setText(this,"GameRecordPercentTxt",utility::toString(m_iActRecordIdex*100/m_pGameRecord->kAction.size(),"%"));
}
void XZDDGameScence::onGPAccountInfoHttpIP(TAG_UserIndividual* pNetInfo)
{
	GameBase::onGPAccountInfoHttpIP(pNetInfo);

	for(int i = 0;i<MAX_PLAYER && HaveOptionRule(GAME_OPTION_RULE_GPS_COLOR);i++)
	{
		XZDDPlayer* pPlayer = m_pPlayer[i];
		if (pPlayer->GetUserInfo() == NULL)
		{
			continue;
		}
		std::string kPlayerGPS = pPlayer->GetGPS();
		bool bVaild = true;
		if (kPlayerGPS == "")
		{
			bVaild = false;
		}
		for(int m = 0;m<MAX_PLAYER && bVaild;m++)
		{
			XZDDPlayer* pTempPlayer = m_pPlayer[m];
			if (pTempPlayer == pPlayer)
			{
				continue;
			}
			if (pTempPlayer->GetUserInfo() == NULL)
			{
				continue;
			}
			std::string kTempGPS = pTempPlayer->GetGPS();
			if (utility::isGPS100In(kTempGPS,kPlayerGPS))
			{
				bVaild = false;
				break;
			}
		}
		if (bVaild)
		{
			WidgetFun::setTextColor(pPlayer->getPlayerNode(),"Name","255 255 255");
		}
		else
		{
			WidgetFun::setTextColor(pPlayer->getPlayerNode(),"Name","255 0 0");
		}
	}


	if (!m_pGameRecord)
	{
		return;
	}
	if (!WidgetFun::isWidgetVisble(this,"GameRecordControlPlane"))
	{
		return;
	}
	int iChairID = -1;
	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		XZDDGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
		if (kRecordPlayer.dwUserID == pNetInfo->dwUserID)
		{
			iChairID = i;
		}
	}
	if (iChairID == -1)
	{
		return;
	}
	iChairID = (m_wRecordSelfChairID-iChairID+MAX_PLAYER)%MAX_PLAYER;
	XZDDPlayer* pDestPlayer = m_pPlayer[iChairID];

	ImagicDownManager::Instance().addDown(
		WidgetFun::getChildWidget(
		pDestPlayer->getPlayerNode()
		,"HeadImagic"),
		pNetInfo->kHttp,pNetInfo->dwUserID);
}