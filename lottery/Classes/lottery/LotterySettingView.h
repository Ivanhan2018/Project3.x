#ifndef __LotterySettingView__
#define __LotterySettingView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BaseLayer.h"

#define SETTING_ITEM_TAG   100

USING_NS_CC;
using namespace std;

class LotterySettingView : public BaseLayer
{
private:
	cocos2d::Size winSize;
	float startX;
	float startY1;
	float startY2;
	float lineGap;
public:
    LotterySettingView();
    ~LotterySettingView();
    CREATE_FUNC(LotterySettingView);
    virtual bool init();
    static Scene* scene();
    virtual void onEnter();
    virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);		
private:
	void initView();	
	void initLabel();
	void initButton();	
	//top menu
	void back(Object *obj);
	void loginOut(Object *obj);
	void settingAction(Object *obj);
};

#endif // __HELLOWORLD_SCENE_H__