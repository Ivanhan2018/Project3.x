#include "NCMJGameScence.h"
#include "GameLib.h"
#include "NCMJPlayer.h"
#include "MissionWeiXin.h"

void NCMJGameScence::initRecord()
{
	WidgetScenceXMLparse kScence("Script/HNGameRecordChild.xml",this);

	WidgetManager::addButtonCB("Button_GameRecordPlay",this,button_selector(NCMJGameScence::Button_GameRecordPlay));
	WidgetManager::addButtonCB("Button_GameRecordPase",this,button_selector(NCMJGameScence::Button_GameRecordPase));
	WidgetManager::addButtonCB("Button_GameRecordLeft",this,button_selector(NCMJGameScence::Button_GameRecordLeft));
	WidgetManager::addButtonCB("Button_GameRecordRight",this,button_selector(NCMJGameScence::Button_GameRecordRight));
	WidgetManager::addButtonCB("Button_GameRecordExit",this,button_selector(NCMJGameScence::Button_GameRecordExit));
}
void NCMJGameScence::defaultRecordState()
{
	if (WidgetFun::getChildWidget(this,"GameRecordControlPlane"))
	{
		WidgetFun::setVisible(this,"GameRecordControlPlane",false);
	}
}
void NCMJGameScence::Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*)
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
	m_RecordTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NCMJGameScence::NextRecordAction,this),m_RecordTime)->iIdex;
}
void NCMJGameScence::Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
	WidgetFun::setVisible(this,"Button_GameRecordPase",false);
	TimeManager::Instance().removeByID(m_RecordTimeID);
}
void NCMJGameScence::Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime += 0.5f;
}
void NCMJGameScence::Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime -= 0.5f;
	if (m_RecordTime < 0.2f)
	{
		m_RecordTime = 0.5f;
	}
}
void NCMJGameScence::Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*)
{
	TimeManager::Instance().removeByID(m_RecordTimeID);
	ExitGame();
}
bool NCMJGameScence::StartRecord(datastream kDataStream)
{
	if (m_pGameRecord == NULL)
	{
		m_pGameRecord = new NCMJGameRecord;
		initRecord();
	}
	defaultState();
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"GameRecordControlPlane",true);
	WidgetFun::setText(this,"GameRecordPercentTxt","0%");
	*m_pGameRecord = NCMJGameRecord();
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
		NCMJGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
		if (kRecordPlayer.dwUserID == UserInfo::Instance().getUserID())
		{
			m_wRecordSelfChairID = i;
		}
	}
	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		NCMJGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];

		int iChairID = (m_wRecordSelfChairID-i+MAX_PLAYER)%MAX_PLAYER;
		NCMJPlayer* pPlayer = m_pPlayer[iChairID];
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
void NCMJGameScence::NextRecordAction()
{
	if (m_iActRecordIdex >= (int)m_pGameRecord->kAction.size() || !isVisible())
	{
		Button_GameRecordPase(NULL,NULL);
		WidgetFun::setText(this,"GameRecordPercentTxt","100%");
		return;
	}
	WidgetFun::setVisible(this,"SelfActionNode",false);
	NCMJGameRecordOperateResult& kAction = m_pGameRecord->kAction[m_iActRecordIdex];
	int iChairID = (m_wRecordSelfChairID-kAction.wOperateUser+MAX_PLAYER)%MAX_PLAYER;
	if (kAction.cbActionType == NCMJGameRecordOperateResult::TYPE_OperateResult)
	{
		NCMJPlayer* pPlayer = m_pPlayer[iChairID];
		CMD_S_OperateResult kTempCMD;
		kTempCMD.cbOperateCard = kAction.cbOperateCard;
		kTempCMD.cbOperateCode = kAction.cbOperateCode;
		kTempCMD.wOperateUser = kAction.wOperateUser;
		kTempCMD.wProvideUser = kAction.wProvideUser;
		pPlayer->setOperateResoult(&kTempCMD);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == NCMJGameRecordOperateResult::TYPE_SendCard)
	{
		NCMJPlayer* pPlayer = m_pPlayer[iChairID];
		if (kAction.cbOperateCard != 0)
		{
			pPlayer->addNewInCard(kAction.cbOperateCard);
		}
		pPlayer->showCard();
		if (kAction.cbOperateCode != 0)
		{
			showUserAction(kAction.cbOperateCode);
		}
	}
	if (kAction.cbActionType == NCMJGameRecordOperateResult::TYPE_OutCard)
	{
		NCMJPlayer* pPlayer = m_pPlayer[iChairID];
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			m_pPlayer[i]->setActOutCard(-1);
		}
		pPlayer->sendOutCard(kAction.cbOperateCard);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == NCMJGameRecordOperateResult::TYPE_ChiHu)
	{
		NCMJPlayer* pPlayer = m_pPlayer[iChairID];
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			m_pPlayer[i]->setActOutCard(-1);
		}
		pPlayer->showEffect("Hu");
		if (kAction.wOperateUser != kAction.wProvideUser)
		{
			NCMJPlayer* pDestPlayer = m_pPlayer[iChairID];
			pPlayer->addNewInCard(kAction.cbOperateCard);
			pPlayer->showHandCard();
			pPlayer->showHuPai(false);
			pPlayer->showStatusImagic("Hu");

			pDestPlayer->runAniDianPao();
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
void NCMJGameScence::onGPAccountInfoHttpIP(TAG_UserIndividual* pNetInfo)
{
	GameBase::onGPAccountInfoHttpIP(pNetInfo);

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
		NCMJGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
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
	NCMJPlayer* pDestPlayer = m_pPlayer[iChairID];

	ImagicDownManager::Instance().addDown(
		WidgetFun::getChildWidget(
		pDestPlayer->getPlayerNode()
		,"HeadImagic"),
		pNetInfo->kHttp,pNetInfo->dwUserID);
}