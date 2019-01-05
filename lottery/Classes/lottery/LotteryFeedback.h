#ifndef __LotteryFeedback__
#define __LotteryFeedback__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryFeedback: public Layer
{

public:
	LotteryFeedback();
	~LotteryFeedback();
	CREATE_FUNC(LotteryFeedback);
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