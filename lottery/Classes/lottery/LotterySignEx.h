//#pragma once
#ifndef __LotterySignEx__
#define __LotterySignEx__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotterySignEx : public Layer
{

public:
	LotterySignEx();
	~LotterySignEx();
	CREATE_FUNC(LotterySignEx);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
private:
	void initData();
	void initView();

	void back(Object *obj);
	//top menu
	//void back(Object *obj);
	//·µ»Ø
	//void signUpRet(Object *obj);
	//void huanLeSongRet(Object *obj);

	cocos2d::Size winSize;
};

#endif