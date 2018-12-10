#include "HNHomeScence.h"

#include "WidgetManager.h"
#include "MissionWeiXin.h"

#include "GameLib.h"

#include "HNScenceManager.h"

void HNHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(HNHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_HomeHead",this,button_selector(HNHomeScence::Button_HomeHead));
	WidgetManager::addButtonCB("Button_HomeAddGold",this,button_selector(HNHomeScence::Button_HomeAddGold));
	WidgetManager::addButtonCB("Button_HomeExit",this,button_selector(HNHomeScence::Button_HomeExit));
	WidgetManager::addButtonCB("Button_sharePengYou",this,button_selector(HNHomeScence::Button_sharePengYou));
	WidgetManager::addButtonCB("Button_shareQuan",this,button_selector(HNHomeScence::Button_shareQuan));
	WidgetManager::addButtonCB("Button_In_Public_Room_ZZ",this,button_selector(HNHomeScence::Button_In_Public_Room_ZZ));
	WidgetManager::addButtonCB("Button_In_Public_Room_CS",this,button_selector(HNHomeScence::Button_In_Public_Room_CS));
	WidgetManager::addButtonCB("Button_Select_Room",this,button_selector(HNHomeScence::Button_Select_Room));
	
}
bool RoomListSort(CGameServerItem* n1, CGameServerItem* n2)
{
	return n1->m_GameServer.lCellScore < n2->m_GameServer.lCellScore;
}
void getRoomList(int iKindID,std::vector<CGameServerItem*>& kGameServerID,int iNodeID)
{
	kGameServerID.clear();
	CServerListData* pListData = CServerListData::shared();
	{
		CGameServerItemMap::iterator itor = pListData->GetServerItemMapBegin();
		CGameServerItem* pListItem = pListData->EmunGameServerItem(itor);
		int iIdex1 = 0;
		cocos2d::Node* pItemRootNode = NULL;
		while (pListItem != NULL)
		{
			CGameServerItem* pActListItem = pListItem;
			if (pActListItem->m_GameServer.wKindID != iKindID
//				|| pActListItem->m_GameServer.wServerType != GAME_GENRE_GOLD
				/*|| pActListItem->m_GameServer.wNodeID != iNodeID*/ )
			{
				pListItem = pListData->EmunGameServerItem(itor);
				continue;
			}
			kGameServerID.push_back(pActListItem);
			pListItem = pListData->EmunGameServerItem(itor);
		}
	}
	std::sort(std::begin(kGameServerID), std::end(kGameServerID), RoomListSort);

}
void HNHomeScence::Button_In_Public_Room_ZZ(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iKindID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"KindID"));
	WidgetFun::setVisible(this,"SelectRoomType",true);
	getRoomList(303,m_kRoomList,100);
	for (int i = 0;i<m_kRoomList.size() && i < 3;i++)
	{
		CGameServerItem* pListItem = m_kRoomList[i];
		WidgetFun::setPlaceTextKey(this,utility::toString("DiFenTxt",i),"ShowTxt",utility::toString((int)pListItem->m_GameServer.lCellScore));
		WidgetFun::setPlaceTextKey(this,utility::toString("NeedFenTxt",i),"ShowTxt",utility::toString((int)pListItem->m_GameServer.lEnterScore));
	}
}
void HNHomeScence::Button_In_Public_Room_CS(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iKindID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"KindID"));
	WidgetFun::setVisible(this,"SelectRoomType",true);
	getRoomList(iKindID,m_kRoomList,200);
}
void HNHomeScence::Button_Select_Room(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	if (iIdex >= (int)m_kRoomList.size())
	{
		return NoticeMsg::Instance().ShowTopMsg("Error");
	}
	GameManagerBase::InstanceBase().connectGameServer(m_kRoomList[iIdex]);
	WidgetFun::setVisible(this,"SelectRoomType",false);
	m_iGameLevel = iIdex;
}
void HNHomeScence::Button_BackHome(cocos2d::Ref*,WidgetUserInfo*)
{
	GameManagerBase::InstanceBase().disconnectServer();
	HNScenceManager::Instance().InHomeScence();
}
void HNHomeScence::Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*)
{
	PopScence::Instance().showIPInfo(
		UserInfo::Instance().getUserNicName(),
		utility::paseInt(UserInfo::Instance().getUserID(),6),
		UserInfo::Instance().getUserIP(),
		UserInfo::Instance().getHeadHttp());
}
void HNHomeScence::Button_HomeAddGold(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"BuyPlane",true);
}
void HNHomeScence::Button_sharePengYou(cocos2d::Ref*,WidgetUserInfo*)
{
	MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
		utility::getScriptString("WeiXinSharTitle"),
		utility::getScriptString("WeiXinSharDes"),MissionWeiXin::SHARE_SESSION);
}
void HNHomeScence::Button_shareQuan(cocos2d::Ref*,WidgetUserInfo*)
{
	MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
		utility::getScriptString("WeiXinSharTitle"),
		utility::getScriptString("WeiXinSharDes"),MissionWeiXin::SHARE_MOMENTS);
}
void HNHomeScence::Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*)
{
	cocos2d::Director::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}