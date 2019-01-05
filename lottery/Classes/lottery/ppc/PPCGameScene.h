#ifndef __PPC_GAME_SCENE_H__
#define __PPC_GAME_SCENE_H__

#include "cocos2d.h"
#include "PPCGameLayer.h"
#include "PPCGameMessage.h"
#include "PPCControl.h"
#include "PPCInfoBar.h"
#include "PPCClock.h"
#include "ToolKit.h"
#include "PPCEnding.h"
#include "PPCPopupApplyList.h"

//	游戏场景
class PPCGameScene: public Scene
{
public:
	PPCGameScene();
	~PPCGameScene();
	CREATE_FUNC(PPCGameScene);

	//	重置
	void resetGameScene();

	//	游戏主场景层
	PPCGameLayer* m_pPPCGameLayer;

	//	控制台
	PPCControl* m_pPPCControl;

	//	信息栏
	PPCInfoBar* m_pPPCInfoBar;

	//	时钟
	PPCClock* m_pPPCClock;

	//	上庄列表框
	PPCPopupApplyList* m_pApplyListView;

	//	游戏结算框
	PPCEnding* m_pPPCEnding;

	//	信息处理层
	PPCGameMessage* m_pPPCGameMessage;

public:
	EPPCProcessType m_GameStatue;//游戏状态

	//	申请条件——持有金币数
	SCORE m_lApplyBankerCondition;

private:
	bool init();
	void initUI();
	void onEnter();

	//	预加载资源
	void addRes();

};

#endif  // __PPC_GAME_SCENE_H__