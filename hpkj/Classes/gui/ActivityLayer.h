#ifndef __Game__ActivityLayer__
#define __Game__ActivityLayer__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class ActivityLayer : public Layer
{

public:
	ActivityLayer();
	~ActivityLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(ActivityLayer);

	void changeActivity(int type);

	void leftActivity(Object* obj);
	void rightActivity(Object* obj);

	void enterActivity(Object* obj);

public:
	
	void dismiss(Object *obj);
	void toFreeLayer(Object* obj);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

private:
	Sprite* m_pUpArrow;			//指向上面的箭头
	Sprite* m_pWhiteBox;			//白框
	Array* m_ActivitySpriteArray;	//活动图片
	float m_cDistanceSize;
	int m_nSelectActivity;
	cocos2d::Size winSize;
	Array* m_ActivityContextArray;//活动内容
	LabelTTF* activityTips;

	Menu* m_EnterMenu;
};
#endif  //__Game__HelpLayer__