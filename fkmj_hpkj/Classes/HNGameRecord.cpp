#include "HNGameRecord.h"
#include "HNScenceManager.h"
#include "GameLib.h"
#include "HNGameManager.h"

FV_SINGLETON_STORAGE(HNGameRecord);

HNGameRecord::HNGameRecord()
	:m_kGameRecordMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kGameRecordMission.setMissionSink(this);
}
HNGameRecord::~HNGameRecord()
{

}
bool HNGameRecord::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNGameRecordScence.xml",this);
	WidgetManager::addButtonCB("Button_zhanji",this,button_selector(HNGameRecord::Button_zhanji));
	WidgetManager::addButtonCB("Button_ShowTotal",this,button_selector(HNGameRecord::Button_ShowTotal));
	WidgetManager::addButtonCB("Button_GameRecordBack",this,button_selector(HNGameRecord::Button_GameRecordBack));
	WidgetManager::addButtonCB("Button_StartGameRecord",this,button_selector(HNGameRecord::Button_StartGameRecord));
	
	return true;
}
void HNGameRecord::EnterScence()
{
	//m_kGameRecordMission.GetGameRecordList(0);
	//m_kGameRecordMission.GetPrivateRandTotalRecord(UserInfo::Instance().getUserID());
}
void HNGameRecord::hideAll()
{
	setVisible(false);
	WidgetFun::setVisible(this,"RecordListPlane",false);
	WidgetFun::setVisible(this,"RecordTotalPlane",false);
}
std::string HNGameRecord::timeToString(systemtime kSystem)
{
	std::string kStr;
	kStr = utility::toString(kStr,kSystem.wYear);
	kStr = utility::toString(kStr,"-");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wMonth,2));
	kStr = utility::toString(kStr,"-");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wDay,2));
	kStr = utility::toString(kStr,'\n');
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wHour,2));
	kStr = utility::toString(kStr,":");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wMinute,2));
	kStr = utility::toString(kStr,":");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wSecond,2));
	return kStr;
}
std::string HNGameRecord::scoreToString(int iScore)
{
	if (iScore > 0)
	{
		return utility::toString("+",iScore);
	}
	return utility::toString(iScore);
}
void HNGameRecord::onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo)
{
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"RecordList0");
	pList->removeAllChildren();
	for (int i = 0;i<(int)pNetInfo->kList.size();i++)
	{
		tagPrivateRandTotalRecord& kInfo = pNetInfo->kList[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdListItem",pList);
		WidgetFun::setWidgetUserInfo(pItemNode,"Button_ShowTotal","RecordID",utility::toString(kInfo.iRecordID));
		WidgetFun::setText(pItemNode,"PlayTime",timeToString(kInfo.kPlayTime));
		WidgetFun::setText(pItemNode,"RoomNum",utility::toString(kInfo.iRoomNum));
		if (kInfo.kNickName.size() != kInfo.kScore.size())
		{
			continue;
		}
		for (int m = 0;m<(int)kInfo.kNickName.size();m++)
		{
			if (kInfo.kUserID[m] == UserInfo::Instance().getUserID())
			{
				if (kInfo.kScore[m] >= 0)
				{
					WidgetFun::setVisible(pItemNode,"WinImagic",true);
					WidgetFun::setVisible(pItemNode,"LoseImagic",false);
				}
				else
				{
					WidgetFun::setVisible(pItemNode,"WinImagic",false);
					WidgetFun::setVisible(pItemNode,"LoseImagic",true);
				}
			}
		}
		for (int m = 0;m<(int)kInfo.kNickName.size();m++)
		{
			if (m >= 4)
			{
				continue;
			}
			WidgetFun::setText(pItemNode,utility::toString("PlayerTxt",m),
				utility::toString(utility::a_u8(kInfo.kNickName[m]),":",scoreToString(kInfo.kScore[m])));
		}

	}
	pList->forceDoLayout();
}
void HNGameRecord::onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo)
{
	m_kPrivateRandTotalRecord = *pNetInfo;
	cocos2d::Node* pRootPlane = WidgetFun::getChildWidget(this,"RecordTotalPlane");
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(pRootPlane,"RecordList1");
	pList->removeAllChildren();
	for (int i = 0;i<(int)pNetInfo->kNickName.size();i++)
	{
		if (i < 4)
		{
			WidgetFun::setText(pRootPlane,utility::toString("PlayerName",i),utility::a_u8(pNetInfo->kNickName[i]));
		}
	}
	for (int i = 0;i<(int)pNetInfo->kRecordChild.size();i++)
	{
		tagPrivateRandRecordChild& kInfo = pNetInfo->kRecordChild[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdTotalItem",pList);
		WidgetFun::setText(pItemNode,"IdexTxt",utility::toString(i+1));
		WidgetFun::setText(pItemNode,"PlayTime",timeToString(kInfo.kPlayTime));
		WidgetFun::setWidgetUserInfo(pItemNode,"Button_StartGameRecord","RecordChildID",utility::toString(kInfo.iRecordChildID));
		for (int m = 0;m<(int)kInfo.kScore.size();m++)
		{
			if (m >= 4)
			{
				continue;
			}
			WidgetFun::setText(pItemNode,utility::toString("PlayerTxt",m),scoreToString(kInfo.kScore[m]));
		}

	}
	pList->forceDoLayout();
}
void HNGameRecord::Button_zhanji(cocos2d::Ref*,WidgetUserInfo*)
{
	setVisible(true);
	WidgetFun::setVisible(this,"RecordListPlane",true);
	m_kGameRecordMission.GetGameRecordList(UserInfo::Instance().getUserID());
}
void HNGameRecord::Button_ShowTotal(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	WidgetFun::setVisible(this,"RecordTotalPlane",true);
	WidgetFun::setVisible(this,"RecordListPlane",false);
	m_kGameRecordMission.GetPrivateRandTotalRecord(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RecordID")));
}
void HNGameRecord::Button_GameRecordBack(cocos2d::Ref*,WidgetUserInfo*)
{
	if (WidgetFun::isWidgetVisble(this,"RecordTotalPlane"))
	{
		WidgetFun::setVisible(this,"RecordTotalPlane",false);
		WidgetFun::setVisible(this,"RecordListPlane",true);
	}
	else
	{
		HNScenceManager::Instance().GameBackScence();
	}
}
void HNGameRecord::Button_StartGameRecord(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RecordChildID"));
	for (int i = 0;i<(int)m_kPrivateRandTotalRecord.kRecordChild.size();i++)
	{
		tagPrivateRandRecordChild& kChild = m_kPrivateRandTotalRecord.kRecordChild[i];
		if (kChild.iRecordChildID == iRecordChildID)
		{
			HNGameManager::Instance().StartXZDDRecord(kChild.kRecordGame);
		}
	}
}