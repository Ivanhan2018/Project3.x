#ifndef __LOTTERY__COVER__
#define __LOTTERY__COVER__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryCover: public Layer
{

public:
	LotteryCover();
	~LotteryCover();
	CREATE_FUNC(LotteryCover);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();	

	static void showModal(LayerColor* parent);
private:
	void initData();
	void initView();		
};

class MyScrollLayer:public Layer
{
public:
	MyScrollLayer();
	virtual ~MyScrollLayer();
	CREATE_FUNC(MyScrollLayer);
	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void setEndPos(Vec2 pos);
private:
	void initData();
	void initView();

	//สýื้
	std::vector<Vec2>  m_vecPot;
	bool m_bDrag;
	Vec2 m_endPos;
};

#endif