#pragma once
#include "cocos2d.h"

#include "GameLib.h"

class PopScence
	:public cocos2d::Node
	,public FvSingleton<PopScence> 
{
public:
	PopScence();
	~PopScence();
public:
	virtual bool init();

	void HideAll();
public:
	void showAccessPlane(std::string kTitle,cocos2d::Ref* pPoint,CB_BUTTON_Handler pAccessHander);
	void showAccessPlane(std::string kTitle,cocos2d::Ref* pPoint,CB_BUTTON_Handler pAccessHander,CB_BUTTON_Handler pCancelHander);
	void Button_PopCancel(cocos2d::Ref*,WidgetUserInfo*);
	void Button_PopAccess(cocos2d::Ref*,WidgetUserInfo*);
public:
	void showIPInfo(std::string strNickName,std::string strID,std::string strIP,std::string kImagicHttp);
private:
	cocos2d::Ref* m_pAccessPlanePoint;
	CB_BUTTON_Handler m_pAccessPlaneAccessHander;
	CB_BUTTON_Handler m_pAccessPlaneCancelHander;
};