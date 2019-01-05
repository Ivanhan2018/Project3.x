#ifndef __LotteryRechargeDetail__
#define __LotteryRechargeDetail__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;


#define DETAIL_INFO_LABEL_TAG		100
class LotteryRechargeDetail: public Layer
{

public:
	LotteryRechargeDetail();
	~LotteryRechargeDetail();
	CREATE_FUNC(LotteryRechargeDetail);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
	void setBank(int id);

void setInfoData(Dictionary *dic);
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);

	void getChongzhiTypeRet(Object *obj);

	void openUrl(Object *obj);

	cocos2d::Size winSize;
	int bankID;
	ccColor3B blackColor;
	ccColor3B redColor;
	float posX1;
	float posX2;
	float startY;
	float gapY;
	Dictionary *infoData;
};


#endif