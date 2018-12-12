#ifndef __Game__TabLayer__
#define __Game__TabLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include <string>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

#define TabLayerBaseTag  1

class TabLayer : public Layer
{
protected:
	int currentItem;				//用于标记tab的当前页 默认为第一页0
	Sprite * pBJ;					//背景图片
	Sprite *maskSprite;			//遮罩图片
private:
	float tabMenuWidth;				//tab标签宽度
	Menu* tabMenu;				//标签按钮
	int tabNum;						//标签数量
	float itemGap;
	Vector<MenuItem*> tabMenuItem;			//标签子菜单
	Array *normalName;			//标签名称
	Array *selectedName;
public:
	TabLayer();
	~TabLayer();
	static TabLayer *createWithTabItems(Array *itemNormalName, Array *itemSelectedName);
	bool initWithTabItems(Array *itemNormalName, Array *itemSelectedName);
	void setTabMenuPositionY(float yPos){tabMenu->setPositionY(yPos);};
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
public:
	void dismiss(Object *obj);
	void pressTabButton(Object *obj);
	virtual void doSomething();
	virtual void doSomethingBeforeClosed(){};
};
#endif  //__Game__WarningLayler__