#ifndef __BRNN_GAME_SCENE_H__
#define __BRNN_GAME_SCENE_H__

#include "cocos2d.h"
#include "BRNNDefine.h"
#include "BRNNGameMessage.h"
#include "BRNNGameLayer.h"
#include "BRNNClock.h"
#include "BRNNInfoBar.h"
#include "BRNNSettlement.h"
#include "BRNNPopupApplyList.h"
#include "BRNNPopupTravel.h"
#include "BRNNPopupSetting.h"

USING_NS_CC;

/*
**	@brief	百人牛牛——游戏场景
*/
class BRNNGameScene: public Scene
{
public:
	BRNNGameScene();
	~BRNNGameScene();
	CREATE_FUNC(BRNNGameScene);

	//	获取当前游戏场景
	static BRNNGameScene* getBRNNGameScene();

	//	预加载资源
	void preLoadRes();

	//	清除缓存
	void clear();

	//	重置游戏数据
	void resetGameScene();

	//	游戏表现层
	BRNNGameLayer* m_pGameLayer;

	//	信息栏
	BRNNInfoBar* m_pInfoBar;

	//	时钟
	BRNNClock* m_pClock;

	//	结算界面
	BRNNSettlement* m_pSettlement;

 	//	上庄列表
	BRNNPopupApplyList* m_pPopupApplyList;

	//	路单
	BRNNPopupTravel* m_pPopupTravel;

	//	设置
	BRNNPopupSetting* m_pPopupSetting;

	//	通讯消息层
	BRNNGameMessage* m_pGameMessage;

	//游戏状态
	E_BRNNProcessType m_eGameStatue;

private:
	bool init();
	void initUI();
	void onEnter();
};

#endif  // __BRNN_GAME_SCENE_H__