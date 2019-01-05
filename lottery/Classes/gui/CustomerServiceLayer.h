#ifndef __Game__CustomerServiceLayer__
#define __Game__CustomerServiceLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class CustomerServiceLayer : public Layer
{


public:
	CustomerServiceLayer();
	~CustomerServiceLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(CustomerServiceLayer);

public:
	void dismiss(Object *obj);
	void customerOnLine(Object *obj);
	void customerPhone(Object *obj);
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
	
	


};
#endif  //__Game__CustomerServiceLayer__