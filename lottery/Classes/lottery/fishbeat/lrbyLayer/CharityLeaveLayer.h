#ifndef _CharityLeaveLayer_H_
#define _CharityLeaveLayer_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;

class CharityLeaveLayer :  public Layer
{
private:
	Widget*	      m_widgetUI;
	Button*		  server_close;
	Button*		  btn_go;
	Button*		  btn_leave;
public:
	CharityLeaveLayer(void);
	~CharityLeaveLayer(void);

	virtual bool init(float fScale);
	static CharityLeaveLayer *create(float fScale);
	//void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void OnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
};

#endif // !_CharityLeaveLayer_H_
