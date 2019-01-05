#ifndef _CharityLayer_H_
#define _CharityLayer_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;

class CharityLayer :  public Layer
{
private:
	Widget*	      m_widgetUI;
	Button*		  server_close;
	Button*       btn_ok;
	Text*         text_get;
public:
	CharityLayer(void);
	~CharityLayer(void);

	virtual bool init(float fScale);
	static CharityLayer *create(float fScale);
	void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void OnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
};

#endif // !_CharityLayer_H_
