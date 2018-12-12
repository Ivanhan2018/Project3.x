#ifndef __Game__VIPSystemLayer__
#define __Game__VIPSystemLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"

class VIPSystemLayer : public Layer
{
private:

public:
	VIPSystemLayer();
	~VIPSystemLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(VIPSystemLayer);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
public:
	void dismiss(Object *obj);
	void onDiscount(Object* obj);
	void onBecomeVIP(Object* obj);
};
#endif  //__Game__WarningLayler__