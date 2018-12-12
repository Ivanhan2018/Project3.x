#ifndef __Game__MenuLayler__
#define __Game__MenuLayler__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"

#include "RegistLayer.h"
class MenuLayer : public Layer
{
private:
	MenuItemSprite *musicItem;
	MenuItemSprite *effectItem;
	bool hasEffect;
	bool hasMusic;
public:
	Menu *pButtonMenu;
public:
	MenuLayer();
	~MenuLayer();
	virtual bool init();
	static Scene* scene();

	CREATE_FUNC(MenuLayer);

public:
	void permitButtonClick();							//允许按钮按
	void forbidButtonClick();							//禁止按钮按
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
	void pressMusicItem(Object* obj);					//游戏背景音乐
	void pressEffectItem(Object* obj);					//游戏背景音乐
};
#endif  //__Game__MenuLayler__