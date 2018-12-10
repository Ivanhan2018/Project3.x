#include "HNHelpScence.h"
#include "HNScenceManager.h"
#include "GameLib.h"

FV_SINGLETON_STORAGE(HNHelpScence);

HNHelpScence::HNHelpScence()
{
	init();
}
HNHelpScence::~HNHelpScence()
{

}
bool HNHelpScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHelpScence.xml",this);

	WidgetManager::addButtonCB("Button_BangZhu",this,button_selector(HNHelpScence::Button_BangZhu));
	return true;
}
void HNHelpScence::EnterScence()
{
}
void HNHelpScence::hideAll()
{
	WidgetFun::setVisible(this,"HelpPlane",false);
}
void HNHelpScence::Button_BangZhu(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"HelpPlane",true);
}