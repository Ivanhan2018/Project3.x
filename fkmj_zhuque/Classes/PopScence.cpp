#include "PopScence.h"

FV_SINGLETON_STORAGE(PopScence);

PopScence::PopScence()
	:m_pAccessPlanePoint(NULL)
	,m_pAccessPlaneAccessHander(NULL)
	,m_pAccessPlaneCancelHander(NULL)
{
	init();
}
PopScence::~PopScence()
{

}
bool PopScence::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/PopScence.xml",this);
	WidgetManager::addButtonCB("Button_PopCancel",this,button_selector(PopScence::Button_PopCancel));
	WidgetManager::addButtonCB("Button_PopAccess",this,button_selector(PopScence::Button_PopAccess));

	return true;
}
void PopScence::HideAll()
{
	WidgetFun::setVisible(this,"AccessPlane",false);
	WidgetFun::setVisible(this,"PlayerInfoPlane",false);
}
void PopScence::showAccessPlane(std::string kTitle,cocos2d::Ref* pPoint,CB_BUTTON_Handler pAccessHander)
{
	HideAll();
	showAccessPlane(kTitle,pPoint,pAccessHander,NULL);
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"AccessPlane");
	WidgetFun::setVisible(pRootNode,"ACPlane1",true);
	WidgetFun::setVisible(pRootNode,"ACPlane2",false);
}
void PopScence::showAccessPlane(std::string kTitle,cocos2d::Ref* pPoint,CB_BUTTON_Handler pAccessHander,CB_BUTTON_Handler pCancelHander)
{
	HideAll();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"AccessPlane");
	m_pAccessPlanePoint = pPoint;
	m_pAccessPlaneAccessHander = pAccessHander;
	m_pAccessPlaneCancelHander = pCancelHander;
	WidgetFun::setText(pRootNode,"AccessTxt",kTitle);
	WidgetFun::ActionStart(pRootNode);
	WidgetFun::setVisible(this,"ACPlane1",false);
	WidgetFun::setVisible(this,"ACPlane2",true);
}
void PopScence::Button_PopCancel(cocos2d::Ref*,WidgetUserInfo*)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"AccessPlane");
	WidgetFun::ActionEnd(pRootNode);
	if (m_pAccessPlaneCancelHander && m_pAccessPlanePoint)
	{
		(m_pAccessPlanePoint->*m_pAccessPlaneCancelHander)(NULL,NULL);
	}
}
void PopScence::Button_PopAccess(cocos2d::Ref*,WidgetUserInfo*)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"AccessPlane");
	WidgetFun::ActionEnd(pRootNode);
	if (m_pAccessPlaneAccessHander && m_pAccessPlanePoint)
	{
		(m_pAccessPlanePoint->*m_pAccessPlaneAccessHander)(NULL,NULL);
	}
}
void PopScence::showIPInfo(std::string strNickName,std::string strID,std::string strIP,std::string kImagicHttp)
{
	HideAll();
	WidgetFun::setVisible(this,"PlayerInfoPlane",true);
	WidgetFun::setText(this,"PopName",strNickName);
	WidgetFun::setText(this,"PopID",strID);
	WidgetFun::setText(this,"PopIP",strIP);

	static ScriptData<std::string> kHNMJHead("HNMJHeadImgic");
	WidgetFun::setImagic(this,"HeadImagic",kHNMJHead.Value());

	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),kImagicHttp,utility::parseInt(strIP));
}