#ifndef __SGLY_GAME_SCENE_H__
#define __SGLY_GAME_SCENE_H__

#include "cocos2d.h"
#include "SGLYGameLayer.h"
#include "SGLYInfoCenter.h"
#include "SGLYMessageCenter.h"
#include "SGLYClock.h"
#include "SGLYPopupSupplyChip.h"
#include "SGLYPopupApplyList.h"
#include "SGLYHistory.h"

USING_NS_CC;

/*
**	@brief	水果乐园——游戏场景
*/
class SGLYGameScene: public Layer
{
public:
	SGLYGameScene();
	~SGLYGameScene();
	CREATE_FUNC(SGLYGameScene);

	static Scene* createScene();

	//	获取当前游戏场景
	static SGLYGameScene* getGameScene();

	//	预加载资源
	void preLoadRes();

	//	清除缓存
	void clear();

	//	重置游戏数据
	void resetGameScene();

	//	游戏层
	SGLYGameLayer* m_pGameLayer;

	//	历史记录
	SGLYHistory* m_pHistory;

	//	时钟
	SGLYClock* m_pClock;

	//	数据中心
	SGLYInfoCenter* m_pInfoCenter;

	//	补充筹码窗口
	SGLYPopupSupplyChip* m_pPopupSupplyChip;

	//	上庄列表窗口
	SGLYPopupApplyList* m_pPopupApplyList;

	//	消息处理中心
	SGLYMessageCenter* m_pMessageCenter;

private:

	bool init();
	void initUI();
	void onEnter();

};

#endif  // __SGLY_GAME_SCENE_H__