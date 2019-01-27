//  [3/26/2014 xieyi]
#ifndef __TIMER__
#define __TIMER__

#include "cocos2d.h"
#include "BJLRes.h"
#include "SceneData.h"
USING_NS_CC;
using namespace cocos2d;
class CCtimeCallBack:public Integer
{
public:
	CCtimeCallBack(int v):Integer(v){};
	CREATE_FUNC_MSG(CCtimeCallBack);
	Game_State m_gameState;
	int				m_timeKey;
};
class MyTimer : public Node
{
public :
	MyTimer();
	~MyTimer();
	CREATE_FUNC(MyTimer);
	virtual bool init();
	void time_schedule(float t);		//时间回调
	int m_time;
	void start(int userId, int t,Game_State gameState,int key);	//运行计时器
	LabelAtlas * m_number;			//计时器数字
	Game_State m_gameState;				//游戏状态
	int				m_timeKey;
	int				m_UserID;
	Sprite *m_nikeSprite;

	void AddObject();
	void RemoveObject();

	void setCCteefTag(Object *obj); //空闲
	void setCardTalayer(Object *obj); //开牌  
	void setPlaceScore(Object *obj);  //下注
private:
	Node * g_timerNode;
};

#endif