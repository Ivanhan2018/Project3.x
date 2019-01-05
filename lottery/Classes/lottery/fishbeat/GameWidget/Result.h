#ifndef __RESULT_H__
#define __RESULT_H__

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
enum ResultEnum
{
	Enum_ResultBack = 90,
	Enum_ResultOk
};
class Result : public cocos2d::Layer
{
public:
	ui::Widget*         m_uiWidget;
	ui::Button*         m_ResultBack;
	ui::Button*         m_ResultOk;
	ui::Text*			m_MyLabel;
	ui::Text*			m_BankLabel;
public:
	Result(void);
	~Result(void);
	virtual bool init();
	CREATE_FUNC(Result);
	void onCallback(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
private:
	void onBtnFace(Ref* sender, TouchEventType type);
	void updateView();
	
};


#endif 
