#ifndef __LotteryBasicUserInfo__
#define __LotteryBasicUserInfo__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryBasicUserInfo: public Layer
{

public:
	LotteryBasicUserInfo();
	~LotteryBasicUserInfo();
	CREATE_FUNC(LotteryBasicUserInfo);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

	void refreshLayer();
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	void getUserInfoRet(Object *obj);

	cocos2d::Size winSize;
	float posX1;
	float posX2;
	float gapY;
	float startY;
	string username;
	DOUBLE consumeMoney;
	DOUBLE consumeTotalMoney;
	DOUBLE earnedMoney;
	DOUBLE earnedTotalMoney;

	LabelTTF *myQQLabel;
	LabelTTF *shangJiQQLabel;
	LabelTTF *consumeLabel;
	LabelTTF *earnedLabel;
};

#endif