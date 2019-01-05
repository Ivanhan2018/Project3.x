#ifndef __LotteryAbout__
#define __LotteryAbout__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryAbout: public Layer
{

public:
	LotteryAbout();
	~LotteryAbout();
	CREATE_FUNC(LotteryAbout);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);

	cocos2d::Size winSize;
};

#endif