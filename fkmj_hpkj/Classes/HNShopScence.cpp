#include "HNShopScence.h"
#include "HNScenceManager.h"
#include "GameLib.h"
#include "SliderMenu.h"

FV_SINGLETON_STORAGE(HNShopScence);

HNShopScence::HNShopScence()
	:m_kShopInfoMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kShopInfoMission.setMissionSink(this);
}
HNShopScence::~HNShopScence()
{

}
bool HNShopScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNShopScence.xml",this);
	WidgetManager::addButtonCB("Button_Shop",this,button_selector(HNShopScence::Button_Shop));
	WidgetManager::addButtonCB("ShopButton_Buy",this,button_selector(HNShopScence::ShopButton_Buy));
	
	return true;
}
void HNShopScence::EnterScence()
{
}
void HNShopScence::hideAll()
{
	WidgetFun::setVisible(this,"ShopPlane",false);
}
void HNShopScence::Button_Shop(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"ShopPlane",true);
	m_kShopInfoMission.GetShopInfoList();
}

void HNShopScence::onGPShopInfoListResult(CMD_GP_BackShopInfo* pNetInfo)
{
	int nSize = pNetInfo->kItemID.size();
	if (nSize==0)
	{
		return;
	}
	cocos2d::ListViewEx* pShopList = WidgetFun::getListViewWidget(this,"THJShopList");
	pShopList->removeAllChildren();

	for (int i=0;i<nSize;i++)
	{
		cocos2d::Node* pItem = WidgetManager::Instance().createWidget(utility::toString("ShopListItem",pNetInfo->kItemID[i]),pShopList);
		WidgetFun::setWidgetUserInfo(pItem,"ShopButton_Buy","ShopID",utility::toString(pNetInfo->kItemID[i]));
	}
	pShopList->refreshView();
}

void HNShopScence::ShopButton_Buy(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	int nShopID = utility::parseInt(WidgetFun::getUserInfoValue(pInfo,"ShopID"));
}
