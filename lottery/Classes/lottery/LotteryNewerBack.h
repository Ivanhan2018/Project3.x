#ifndef __LotteryNewerBack__
#define __LotteryNewerBack__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryNewerBack: public Layer
{

public:
	LotteryNewerBack();
	~LotteryNewerBack();
	CREATE_FUNC(LotteryNewerBack);
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

	void qianDaoRet(Object *obj);
	void qianDaoInfoRet(Object *obj);

	//≥‰÷µÀÕ
	void chongZhiSongGetRet(Object *obj);
	void chongZhiSongSetRet(Object *obj);

	cocos2d::Size winSize;
};

#endif