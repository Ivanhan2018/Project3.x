#include "HNSetScence.h"
#include "HNScenceManager.h"
#include "GameLib.h"
#include "SliderMenu.h"

FV_SINGLETON_STORAGE(HNSetScence);

HNSetScence::HNSetScence(){
	init();
}
HNSetScence::~HNSetScence()
{

}
bool HNSetScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNSetScence.xml",this);
	WidgetManager::addButtonCB("SetBarSider1",this,button_selector(HNSetScence::SetBarSider1));
	WidgetManager::addButtonCB("SetBarSider2",this,button_selector(HNSetScence::SetBarSider2));
	WidgetManager::addButtonCB("Button_HomeSet",this,button_selector(HNSetScence::Button_HomeSet));
	WidgetManager::addButtonCB("Button_HideSet",this,button_selector(HNSetScence::Button_HideSet));
	WidgetManager::addButtonCB("Button_GameSet",this,button_selector(HNSetScence::Button_GameSet));
	
	return true;
}
void HNSetScence::EnterScence()
{
}
void HNSetScence::hideAll()
{
	WidgetFun::setVisible(this,"SetPlane",false);
}
void HNSetScence::SetBarSider1(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>((Node*)pNode);
	SoundFun::Instance().SetSoundEffect(pSlider->getValue()/pSlider->getMaxValue());
}
void HNSetScence::SetBarSider2(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>((Node*)pNode);
	SoundFun::Instance().SetSoundBackground(pSlider->getValue()/pSlider->getMaxValue());
}
void HNSetScence::Button_HideSet(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::Node* pRootNode = WidgetManager::Instance().getBaseNode((cocos2d::Node*)pNode);
	{
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider1"));
		cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar1",pSlider->getValue()/100.0f);
	}
	{
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider2"));
		cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar2",pSlider->getValue()/100.0f);
	}
}
void HNSetScence::Button_HomeSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	WidgetFun::setVisible(this,"SetPlane",true);
	Button_GameSet(this,pUserInfo);
}
void HNSetScence::Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::Node* pRootNode = WidgetManager::Instance().getBaseNode((cocos2d::Node*)pNode);
	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar1",0.7f);
		fTime =fTime*100;
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider1"));
		pSlider->setActValue(fTime);
		WidgetFun::setProgress(pRootNode,"SetBar1",fTime,100);
		SoundFun::Instance().SetSoundEffect(fTime/pSlider->getMaxValue());
	}

	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar2",0.7f);
		fTime =fTime*100;
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider2"));
		pSlider->setActValue(fTime);
		WidgetFun::setProgress(pRootNode,"SetBar2",fTime,100);
		SoundFun::Instance().SetSoundBackground(fTime/pSlider->getMaxValue());
	}
}