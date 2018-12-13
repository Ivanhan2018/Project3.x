#ifndef _SceneBackDialogLayer_H_
#define _SceneBackDialogLayer_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"//3.x
#include"cocostudio/CocoStudio.h"//3.x
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::ui;
USING_NS_CC_EXT;

class SceneBackDialogLayer :  public Layer
{
private:
	Widget*	      m_widgetUI;
	Button*		  server_close;
	Button*		  btn_go;
	Button*		  btn_leave;
public:
	SceneBackDialogLayer(void);
	~SceneBackDialogLayer(void);

	virtual bool init(float fScale);
	virtual void onEnter();
	virtual void onExit();
	static SceneBackDialogLayer *create(float fScale);
	//void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void OnCallBack(cocos2d::CCObject* sender,cocos2d::ui::TouchEventType type);
};

#endif // !_SceneBackDialogLayer_H_
