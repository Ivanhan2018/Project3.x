#ifndef __PPC_GAME_MESSAGE_H__
#define __PPC_GAME_MESSAGE_H__

#include "cocos2d.h"
#include "BaseGameLayer.h"
#include "GlobalDef.h"

//	游戏消息处理
class PPCGameMessage: public BaseGameLayer
{
public:
	PPCGameMessage();
	~PPCGameMessage();
	CREATE_FUNC(PPCGameMessage);

public://继承的子类方法
	//	处理进入游戏
	void dealEnterGame(Ref* pSender);

	//	处理玩家信息
	void dealPlayerInfo(Ref* pSender);

	//	处理游戏消息
	void dealMessageGame(Ref* pSender);

private:
	bool init();

	//	更新状态
	void dealGameState(tagUserData eData);
	
private://通讯接口

	//	实现游戏消息
	bool OnEventGameMessage(WORD wSubCmdID, void* pData, WORD wDataSize);

	//	实现游戏场景
	void onMessageScene(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize);

	//	中途入桌——空闲状态
	void OnGameSceneFree(const void *pBuffer, WORD wDataSize);

	//	中途入桌——下注阶段
	void  OnGameScenePlay(const void *pBuffer, WORD wDataSize);

	//	中途入桌——结算阶段
	void  OnGameScenePlayEnd(const void *pBuffer, WORD wDataSize);

	//游戏空闲
	bool OnSubGameFree(void *pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(void *pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(void *pBuffer, WORD wDataSize, bool bGameMes);
	//申请做庄
	bool OnSubUserApplyBanker(void *pBuffer, WORD wDataSize);

	//取消做庄
	bool OnSubUserCancelBanker(void *pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(void *pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(void *pBuffer, WORD wDataSize);
	//游戏记录
	bool OnSubGameRecord(void *pBuffer, WORD wDataSize);
	//	上庄列表
	bool OnApplyBankerList(void *pBuffer, WORD wDataSize);

	//下注失败
	bool OnSubPlaceJettonFail(void *pBuffer, WORD wDataSize);
	//SUB_S_CHECK_IMAGE
	bool OnSubCheckImageIndex(void *pBuffer, WORD wDataSize);
	//设置
	bool OnSubAdminControl(void *pBuffer, WORD wDataSize);
	//SUB_S_SCORE_RESULT
	bool OnSubScoreResult(void *pBuffer, WORD wDataSize);
	//SUB_S_ACCOUNT_RESULT
	bool OnSubAccountResult(void *pBuffer, WORD wDataSize);

};

#endif  // __PPC_GAME_MESSAGE_H__