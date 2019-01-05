#ifndef __LotterySign__
#define __LotterySign__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotterySign: public Layer
{

public:
	LotterySign();
	~LotterySign();
	CREATE_FUNC(LotterySign);
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
	//·µ»Ø
	void signUpRet(Object *obj);
	void huanLeSongRet(Object *obj);

	cocos2d::Size winSize;
};

#endif