#ifndef __LotteryRetBack__
#define __LotteryRetBack__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryRetBack: public Layer
{

public:
	LotteryRetBack();
	~LotteryRetBack();
	CREATE_FUNC(LotteryRetBack);
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