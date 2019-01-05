//  [3/26/2014 xieyi]
#ifndef DDZ___TIMER__
#define DDZ___TIMER__

#include "cocos2d.h"
#include "DDZDDZRes.h"
#include "DDZSceneData.h"
USING_NS_CC;

class DDZCCtimeCallBack:public Integer
{
public:
	DDZCCtimeCallBack(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCtimeCallBack);
	DDZGame_State m_gameState;
	int				m_timeKey;
};

class DDZTimer : public Node
{
public :
	DDZTimer();
	~DDZTimer();
	CREATE_FUNC(DDZTimer);
	virtual bool init();
	void time_schedule(float t);		//时间回调
	int m_time;
	void start(int userId, int t,DDZGame_State gameState,int key);	//运行计时器
	Label* m_pDDZTimeNum;		//	计时器数字
	DDZGame_State m_gameState;				//游戏状态
	int				m_timeKey;
	int				m_UserID;

	bool longTimeNoAction;				//是否进入长时间未操作流程

	void startCheckNoAction();			//启动玩家不操作检测

	void stopCheckCheckNoAction();		//关闭玩家不操作检测

	void onUserNoAction(float dt);		//用户长时间没有操作， 显示用户断线
private:
	Node * g_timerNode;

	int m_allCountTime;					//倒计时开始的总时间
};

#endif