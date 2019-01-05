#ifndef __LotteryHappyHouse__
#define __LotteryHappyHouse__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryHappyHouse: public Layer
{

public:
	LotteryHappyHouse();
	~LotteryHappyHouse();
	CREATE_FUNC(LotteryHappyHouse);
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