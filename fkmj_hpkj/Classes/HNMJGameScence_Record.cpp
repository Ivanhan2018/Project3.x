#include "HNMJGameScence.h"
#include "GameLib.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"
#include "MissionWeiXin.h"

void HNMJGameScence::initRecord()
{
	WidgetScenceXMLparse kScence("Script/HNGameRecordChild.xml",this);

	WidgetManager::addButtonCB("Button_GameRecordPlay",this,button_selector(HNMJGameScence::Button_GameRecordPlay));
	WidgetManager::addButtonCB("Button_GameRecordPase",this,button_selector(HNMJGameScence::Button_GameRecordPase));
	WidgetManager::addButtonCB("Button_GameRecordLeft",this,button_selector(HNMJGameScence::Button_GameRecordLeft));
	WidgetManager::addButtonCB("Button_GameRecordRight",this,button_selector(HNMJGameScence::Button_GameRecordRight));
	WidgetManager::addButtonCB("Button_GameRecordExit",this,button_selector(HNMJGameScence::Button_GameRecordExit));
}
void HNMJGameScence::defaultRecordState()
{
	if (WidgetFun::getChildWidget(this,"GameRecordControlPlane"))
	{
		WidgetFun::setVisible(this,"GameRecordControlPlane",false);
	}
}
void HNMJGameScence::Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*)
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
	m_RecordTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::NextRecordAction,this),m_RecordTime)->iIdex;
}
void HNMJGameScence::Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
	WidgetFun::setVisible(this,"Button_GameRecordPase",false);
	TimeManager::Instance().removeByID(m_RecordTimeID);
}
void HNMJGameScence::Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime += 0.5f;
}
void HNMJGameScence::Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime -= 0.5f;
	if (m_RecordTime < 0.2f)
	{
		m_RecordTime = 0.5f;
	}
}
void HNMJGameScence::Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*)
{
	TimeManager::Instance().removeByID(m_RecordTimeID);
	ExitGame();
}
bool HNMJGameScence::StartRecord(datastream kDataStream)
{
	if (m_pGameRecord == NULL)
	{
		m_pGameRecord = new HNMJGameRecord;
		initRecord();
	}
	defaultState();
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"GameRecordControlPlane",true);
	WidgetFun::setText(this,"GameRecordPercentTxt","0%");
	*m_pGameRecord = HNMJGameRecord();
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
		HNMJGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
		if (kRecordPlayer.dwUserID == UserInfo::Instance().getUserID())
		{
			m_wRecordSelfChairID = i;
		}
	}
	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		HNMJGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];

		int iChairID = (m_wRecordSelfChairID-i+MAX_PLAYER)%MAX_PLAYER;
		HNMJPlayer* pPlayer = m_pPlayer[iChairID];
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
	WidgetFun::setVisible(this,"Button_Master",false);
	WidgetFun::setVisible(this,"GoldRoomInfo",false);
	return true;
}
void HNMJGameScence::NextRecordAction()
{
	if (m_iActRecordIdex >= (int)m_pGameRecord->kAction.size() || !isVisible())
	{
		Button_GameRecordPase(NULL,NULL);
		WidgetFun::setText(this,"GameRecordPercentTxt","100%");
		return;
	}
	HNMJGameRecordOperateResult& kAction = m_pGameRecord->kAction[m_iActRecordIdex];
	int iChairID = (m_wRecordSelfChairID-kAction.wOperateUser+MAX_PLAYER)%MAX_PLAYER;
	int iProvideUser = (m_wRecordSelfChairID-kAction.wProvideUser+MAX_PLAYER)%MAX_PLAYER;
	if (kAction.cbActionType == HNMJGameRecordOperateResult::TYPE_OperateResult)
	{
		HNMJPlayer* pPlayer = m_pPlayer[iChairID];
		CMD_S_OperateResult kTempCMD;
		kTempCMD.cbOperateCard = kAction.cbOperateCard;
		kTempCMD.cbOperateCode = kAction.cbOperateCode;
		kTempCMD.wOperateUser = kAction.wOperateUser;
		kTempCMD.wProvideUser = kAction.wProvideUser;
		HNMJPlayer* pProvidePlayer = m_pPlayer[iProvideUser];
		if (pProvidePlayer &&(kAction.cbOperateCode == WIK_PENG 
			|| kAction.cbOperateCode == WIK_LEFT
			|| kAction.cbOperateCode == WIK_CENTER
			||kAction.cbOperateCode == WIK_RIGHT
			|| (kAction.cbOperateCode == WIK_GANG && kAction.wOperateUser != kAction.wProvideUser )
			|| (kAction.cbOperateCode == WIK_CS_GANG && kAction.wOperateUser != kAction.wProvideUser )
			|| (kAction.cbOperateCode == WIK_BU_ZHANG && kAction.wOperateUser != kAction.wProvideUser )))
		{
			pProvidePlayer->removeHandOutCard(kAction.cbOperateCard);
			pProvidePlayer->setActOutCard(-1);
		}
		pPlayer->setOperateResoult(&kTempCMD);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == HNMJGameRecordOperateResult::TYPE_SendCard)
	{
		HNMJPlayer* pPlayer = m_pPlayer[iChairID];
		if (kAction.cbOperateCard != 0)
		{
			pPlayer->addNewInCard(kAction.cbOperateCard);
		}
		pPlayer->showCard();
	}
	if (kAction.cbActionType == HNMJGameRecordOperateResult::TYPE_OutCard)
	{
		HNMJPlayer* pPlayer = m_pPlayer[iChairID];
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			m_pPlayer[i]->setActOutCard(-1);
		}
		pPlayer->sendOutCard(kAction.cbOperateCard);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == HNMJGameRecordOperateResult::TYPE_ChiHu)
	{
		HNMJPlayer* pPlayer = m_pPlayer[iChairID];
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			m_pPlayer[i]->setActOutCard(-1);
		}
		pPlayer->showEffect("Hu");
		if (kAction.wOperateUser != kAction.wProvideUser)
		{
			HNMJPlayer* pDestPlayer = m_pPlayer[iChairID];
			pPlayer->addNewInCard(kAction.cbOperateCard);
			pPlayer->showHandCard();
			pPlayer->showHuPai(false);
			pPlayer->showStatusImagic("Hu");

			pDestPlayer->runAniDianPao();
			pDestPlayer->removeHandOutCard(kAction.cbOperateCard);
		}
		else
		{
			pPlayer->showHuPai(true);
			pPlayer->showStatusImagic("ZiMo");
			pPlayer->runAniZiMo();
		}
		pPlayer->showCard();
	}

	m_iActRecordIdex++;
	Button_GameRecordPlay(NULL,NULL);

	WidgetFun::setText(this,"GameRecordPercentTxt",utility::toString(m_iActRecordIdex*100/m_pGameRecord->kAction.size(),"%"));
}
void HNMJGameScence::onGPAccountInfoHttpIP(TAG_UserIndividual* pNetInfo)
{
	GameBase::onGPAccountInfoHttpIP(pNetInfo);

	
	for(int i = 0;i<MAX_PLAYER && HaveOptionRule(GAME_OPTION_RULE_GPS_COLOR);i++)
	{
		HNMJPlayer* pPlayer = m_pPlayer[i];
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
			HNMJPlayer* pTempPlayer = m_pPlayer[m];
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
		HNMJGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
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
	HNMJPlayer* pDestPlayer = m_pPlayer[iChairID];
	ImagicDownManager::Instance().addDown(
		WidgetFun::getChildWidget(
		pDestPlayer->getPlayerNode()
		,"HeadImagic"),
		pNetInfo->kHttp,pNetInfo->dwUserID);
}