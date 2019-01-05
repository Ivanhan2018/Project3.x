#ifndef __LotteryMemberShip__
#define __LotteryMemberShip__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

#define EDITBOX_TAG  100
#define MAX_SIZE 20

class LotteryMemberShip:public Layer
{

public:
	LotteryMemberShip();
	~LotteryMemberShip();
	CREATE_FUNC(LotteryMemberShip);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

	void refreshLayer(Object *obj);
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	void sendPacket();
	void getHyShjRet(Object *obj);

	void refreshValue();
		
	//≤È—Ø∞¥≈•
	void pressConfirm(Object *obj);

	cocos2d::Size winSize;	
	float startY;
	string username;
	
	string s_t_start;
	string s_t_end;

	std::vector<Object*> recordData;
};

#endif